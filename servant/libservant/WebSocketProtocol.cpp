#include "servant/WebSocketProtocol.h"
#include "servant/RemoteLogger.h"
#include "util/tc_base64.h"
#include "util/tc_sha.h"
#include <stdint.h>

namespace tars
{

const int WebSocketProtocol::iValidVersion;
const unsigned char  WebSocketProtocol::ucPayloadFlag;
const int WebSocketProtocol::eIncompletePacket;
const int WebSocketProtocol::eUndefinedFailure;
const int WebSocketProtocol::eErrorVersion;
const int WebSocketProtocol::eSuccess;
const int WebSocketProtocol::eExceptionErr;

const string WebSocketProtocol::Sec_WebSocket_Key = "Sec-WebSocket-Key";
const string WebSocketProtocol::Sec_WebSocket_Version = "Sec-WebSocket-Version";
const string WebSocketProtocol::Server_Key_Padding = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const string WebSocketProtocol::WS_Handshake_Header = "HTTP/1.1 101 Switching Protocols\r\n"
                                                      "Upgrade: websocket\r\n"
                                                      "Connection: Upgrade\r\n"
                                                      "Sec-WebSocket-Accept: ";


const string WebSocketProtocol::WS_Handshake_Tail = "\r\n\r\n";

WebSocketProtocol::FuncValidator WebSocketProtocol::_funcValidator = nullptr;

void WebSocketProtocol::setValidator(FuncValidator validator)
{
    WebSocketProtocol::_funcValidator = validator;
}

int WebSocketProtocol::parseDataHeader(TC_NetWorkBuffer &in, unsigned int &nMinExpectedSize, uint64_t &ullPayloadSize, int &iMasksOffset,
                                   int &iStartLen, bool &bMultiFrame)
{
    string sIn;
    in.getHeader(in.getBufferLength(), sIn);
    if (sIn.length() == 0)
    {
        return WebSocketProtocol::eIncompletePacket;
    }
    
    
    /*
     * 针对大包分帧问题，每次解析只保留之前解析出来应用数据，去掉包头
     * 所以需要对sIn做一些修改，如果第一个字节==0x00, 则时做了分数据帧，那么接下来4个字节用作保存本次接收到到帧在sIn的开始位置，这边做了特殊逻辑，否则就是默认websocket数据包
     * 0x00|4字节（当前帧开始位置）|之前所有帧的应用层数据|当前帧数据
     * */
    unsigned char cFlag = sIn[0];
    iStartLen = 0;
    if (cFlag == 0x00)
    {
        //采用分帧的兼容逻辑
        iStartLen = *((int *) &sIn[1]);
        bMultiFrame = true;
    }
    
    string sTmp = sIn.substr(iStartLen);
    
    /**
     * msg_opcode含义：
     *  %x0 表示连续消息片断
     *  %x1 表示文本消息片断
     *  %x2 表未二进制消息片断
     *  %x3-7 为将来的非控制消息片断保留的操作码
     *  %x8 表示连接关闭
     *  %x9 表示心跳检查的ping
     *  %xA 表示心跳检查的pong
     *  %xB-F 为将来的控制消息片断的保留操作码
     */
    unsigned char msg_opcode = sTmp[0] & 0x0F;
    unsigned char msg_fin = (sTmp[0] >> 7) & 0x01;
//    unsigned char msg_masked = (sTmp[1] >> 7) & 0x01;
    
    if (!bMultiFrame)
    {
        if (msg_fin == 0)
        {
            bMultiFrame = true;
        }
    }
    
    nMinExpectedSize = 6;
    
    if (sTmp.length() < nMinExpectedSize)
    {
        return WebSocketProtocol::eIncompletePacket;
    }
    
    if (!bMultiFrame)
    {
        if (msg_opcode != 0x0 && msg_opcode != 0x1
            && msg_opcode != 0x2 && msg_opcode != 0x8
            && msg_opcode != 0x9 && msg_opcode != 0xA)
        {
            return WebSocketProtocol::eUndefinedFailure;
        }
    }
    
    unsigned char ucBasicSize = sTmp[1] & 0x7F;
    if (ucBasicSize <= 125)
    {
        ullPayloadSize = ucBasicSize;
        
        iMasksOffset = 2;
    }
    else if (ucBasicSize == 126)
    {
        nMinExpectedSize += 2;
        if (sTmp.length() < nMinExpectedSize)
        {
            return WebSocketProtocol::eIncompletePacket;
        }
        
        ullPayloadSize = ntohs(*(u_short * )(sTmp.c_str() + 2));
        iMasksOffset = 4;
    }
    else if (ucBasicSize == 127)
    {
        nMinExpectedSize += 8;
        if (sTmp.length() < nMinExpectedSize)
        {
            return WebSocketProtocol::eIncompletePacket;
        }
        
        //ullPayloadSize = be64toh(*(u_long * )(sTmp.c_str() + 2));
        ullPayloadSize = tars_ntohll(*(u_long * )(sTmp.c_str() + 2));
        iMasksOffset = 10;
    }
    else
    {
        TLOGERROR( "[WebSocketProtocol::parseDataHeader] error, packet error:" << TC_Common::bin2str(sIn, " ", 16) << endl);
        
        return WebSocketProtocol::eUndefinedFailure;
    }
    nMinExpectedSize += ullPayloadSize;
    
    TLOGTARS(" fin: " << (unsigned int) msg_fin
                        << " ,starlen:" << iStartLen
                        << " ,multi frame: " << (int) bMultiFrame
                        << " ,msg_opcode=" << (unsigned int) msg_opcode
                        << " ,all data size: " << sIn.size()
                        << " ,all data first 5 unsigned char: " << TC_Common::bin2str(string(sIn.c_str(), 5), "", 16)
                        << " ,frame data size: " << sTmp.size()
                        << ", payload size: " << ullPayloadSize
                        << ", nMinExpectedSize:" << nMinExpectedSize
                        << " ,frame first unsigned char : " << (unsigned int) cFlag
                        << " ,frame first unsigned char: " << TC_Common::bin2str(string(sTmp.c_str(), 2), "", 16));
    
    
    if (sTmp.length() < nMinExpectedSize)
    {
        return WebSocketProtocol::eIncompletePacket;
    }
    
    if (msg_opcode == 0x8 || msg_opcode == 0x9 || msg_opcode == 0xA)
    {
        return WebSocketProtocol::eIgnoreFrame;
    }
    if (msg_fin == 0)
    {
        //分帧了
        return WebSocketProtocol::eCompleteFrame;
    }
    else
    {
        return WebSocketProtocol::eSuccess;
    }
}

TC_NetWorkBuffer::PACKET_TYPE WebSocketProtocol::parseBin(TC_NetWorkBuffer &in, vector<char> &out)
{
    try
    {
        unsigned int nMinExpectedSize = 0;
        uint64_t ullPayloadSize = 0;
        int iMasksOffset = 0;
        
        bool bMultiFrame = false;
        int iStartLen = 0;
        
        int iRet = parseDataHeader(in, nMinExpectedSize, ullPayloadSize, iMasksOffset, iStartLen, bMultiFrame);
        
        if (WebSocketProtocol::eIncompletePacket == iRet)
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }
        else if (WebSocketProtocol::eIgnoreFrame == iRet)
        {
            in.moveHeader(nMinExpectedSize);
            return TC_NetWorkBuffer::PACKET_LESS;
        }
        else if (WebSocketProtocol::eSuccess != iRet && WebSocketProtocol::eCompleteFrame != iRet)
        {
            return TC_NetWorkBuffer::PACKET_ERR;
        }
        
        if (!bMultiFrame)
        {
            //未分帧
            in.moveHeader(iMasksOffset);
            
            vector<char> masks;
            in.getHeader(4, masks);
            in.moveHeader(4);
            
            vector<char> cpPayload;
            in.getHeader(ullPayloadSize, cpPayload);
            in.moveHeader(ullPayloadSize);
            for (uint64_t i = 0;
                 i < ullPayloadSize;
                 i++)
            {
                cpPayload[i] = (cpPayload[i] ^ masks[i % 4]);
            }
            
            out.swap(cpPayload);
            
            //TLOGDEBUG("data : " << out.size() << endl);
            
            return TC_NetWorkBuffer::PACKET_FULL;
        }
        else
        {
            /////////startLen（可能包含startLen-5字节preFrame） + iMasksOffset + 4字节mask + payload
            vector<char> vPreFrame;
            if (iStartLen >= 5)
            {
                in.moveHeader(5);
                in.getHeader(iStartLen - 5, vPreFrame);
                in.moveHeader(iStartLen - 5);
            }
            else
            {
                in.moveHeader(iStartLen);
            }
            in.moveHeader(iMasksOffset);
            
            vector<char> masks;
            in.getHeader(4, masks);
            in.moveHeader(4);
            
            vector<char> vFrameData;
            in.getHeader(ullPayloadSize, vFrameData);
            in.moveHeader(ullPayloadSize);
            
            for (uint64_t i = 0;
                 i < ullPayloadSize;
                 i++)
            {
                vFrameData[i] = (vFrameData[i] ^ masks[i % 4]);
            }
            
            if (iRet == WebSocketProtocol::eCompleteFrame)
            {
                //数据帧接收结束
                vector<char> vLeftStr = in.getBuffers();
                if (iStartLen == 0)
                {
                    iStartLen += 5;
                }
                iStartLen += vFrameData.size();
                
                string sHead(5, 0);
                sHead[1] = *((char *) &iStartLen);
                sHead[2] = *((char *) &iStartLen + 1);
                sHead[3] = *((char *) &iStartLen + 2);
                sHead[4] = *((char *) &iStartLen + 3);
                
                int iTmpLen = *(int *) &sHead[1];
                
                TLOGTARS("sNewData " << TC_Common::bin2str(sHead, "", 16)
                                       << " start len : " << iStartLen
                                       << " iTmpLen: " << iTmpLen
                                       << " prestr : " << vPreFrame.size()
                                       << " frame data: " << vFrameData.size() << endl);
                
                in.clearBuffers();
                in.addBuffer(sHead.c_str(), sHead.length());
                if (vPreFrame.size())
                {
                    in.addBuffer(vPreFrame);
                }
                in.addBuffer(vFrameData);
                in.addBuffer(vLeftStr);
                return TC_NetWorkBuffer::PACKET_LESS;
            }
            else
            {
                //数据包接收结束
                out.insert(out.end(), vPreFrame.begin(), vPreFrame.end());
                out.insert(out.end(), vFrameData.begin(), vFrameData.end());
                
                return TC_NetWorkBuffer::PACKET_FULL;
            }
        }
        
    }
    catch (exception &ex)
    {
        TLOGERROR( "[WebSocketProtocol::parseBin]" << "WebSocketProtocol parse catch exception: " << ex.what() << endl);
        return TC_NetWorkBuffer::PACKET_ERR;
    }
    
    return TC_NetWorkBuffer::PACKET_ERR;
}


bool WebSocketProtocol::isHttp(const vector<char> &requestBuf)
{
    if (requestBuf.size() < 1)
    {
        return false;
    }
    static string header = "GET /";
    size_t complen = requestBuf.size() >= header.size() ? header.size() : requestBuf.size();
    return strncmp(&requestBuf[0], header.c_str(), complen) == 0;
}

string WebSocketProtocol::buildErrResponse(int status)
{
    string errInfo, errContent;
    if (status == 403)
    {
        errInfo = "Forbidden";
        errContent = "<html>403 Forbidden</html>";
    }
    
    TC_HttpResponse response;
    response.setResponse(status, errInfo, errContent);
    response.setContentType("text/html;charset=utf-8");
    response.setConnection("close");
    return response.encode();
}

TC_NetWorkBuffer::PACKET_TYPE WebSocketProtocol::doHttpHandshake(TC_NetWorkBuffer &in, vector<char> &out)
{
    TC_HttpRequest httpRequest;
    vector<char> reqbuf = in.getBuffers();
    try
    {
        if (false == httpRequest.decode(&reqbuf[0], reqbuf.size()))
        {
            return TC_NetWorkBuffer::PACKET_ERR;
        }

        if (_funcValidator && (!_funcValidator(httpRequest)))
        {
            //不允许的数据源，回403
            TC_EpollServer::Connection *connection = (TC_EpollServer::Connection *) in.getConnection();
            if (connection)
            {
                connection->sendBufferDirect(buildErrResponse(403));
                in.clearBuffers();
            }
            return TC_NetWorkBuffer::PACKET_ERR;
        }
    }
    catch (const std::exception &e)
    {
        return TC_NetWorkBuffer::PACKET_ERR;
    }
    
    //直接对http回包进行握手
    string sResponse;
    int res = WebSocketProtocol::processHandshake(httpRequest, sResponse);
    if (res == 0)
    {
        TC_EpollServer::Connection *connection = (TC_EpollServer::Connection *) in.getConnection();
        if (connection)
        {
            connection->sendBufferDirect(sResponse);
            
            in.clearBuffers();
            return TC_NetWorkBuffer::PACKET_LESS;
        }
    }
    return TC_NetWorkBuffer::PACKET_ERR;
    
}


TC_NetWorkBuffer::PACKET_TYPE WebSocketProtocol::parse(TC_NetWorkBuffer &in, vector<char> &out)
{
    if (in.getBufferLength() == 0)
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }
    vector<char> tmp;
    in.getHeader(in.getBufferLength() >= 5 ? 5 : 1, tmp);
    
    if (isHttp(tmp))
    {
        return doHttpHandshake(in, out);
    }
    else
    {
        return parseBin(in, out);
    }
    
}


int WebSocketProtocol::decodeData(const string &sIn, string &sOut)
{
    /*
    unsigned int nMinExpectedSize = 0;
    uint64_t ullPayloadSize = 0;
    int iMasksOffset = 0;

    int iRet = parseDataHeader(sIn, nMinExpectedSize, ullPayloadSize, iMasksOffset);

    if(WebSocketProtocol::eSuccess != iRet)
    {
        LOG->error() << "[WebSocketProtocol::decodeData] error, parseDataHeader ret:" << iRet << endl;
        return iRet;
    }

    unsigned char masks[4];
    memcpy(masks, sIn.c_str() + iMasksOffset, 4);

    char* cpPayload = new char[ullPayloadSize + 1];
    memcpy(cpPayload, sIn.c_str() + iMasksOffset + 4, ullPayloadSize);
    for (uint64_t i = 0; i < ullPayloadSize; i++) 
    {
        cpPayload[i] = (cpPayload[i] ^ masks[i%4]);
    }

    sOut = string(cpPayload, ullPayloadSize);
    */
    
    sOut = sIn;
    
    return WebSocketProtocol::eSuccess;
}

void WebSocketProtocol::encodeData(const string &sIn, string &sOut)
{
    sOut.clear();
    
    uint64_t ullPayloadSize = sIn.length();
    
    char cFlag = WebSocketProtocol::ucBinPayloadFlag;
    
    sOut.append(&cFlag, 1);
    
    char cBasicSize = 0;
    if (ullPayloadSize <= 125)
    {
        cBasicSize = ullPayloadSize;
        sOut.append(&cBasicSize, 1);
    }
    else if (ullPayloadSize > 125 && ullPayloadSize <= 65535)
    {
        cBasicSize = 126;
        sOut.append(&cBasicSize, 1);
        
        char len[2];
        len[0] = (ullPayloadSize >> 8) & 255;
        len[1] = (ullPayloadSize) & 255;
        sOut.append(len, 2);
    }
    else
    {
        cBasicSize = 127;
        sOut.append(&cBasicSize, 1);
        
        char len[8];
        len[0] = (ullPayloadSize >> 56) & 255;
        len[1] = (ullPayloadSize >> 48) & 255;
        len[2] = (ullPayloadSize >> 40) & 255;
        len[3] = (ullPayloadSize >> 32) & 255;
        len[4] = (ullPayloadSize >> 24) & 255;
        len[5] = (ullPayloadSize >> 16) & 255;
        len[6] = (ullPayloadSize >> 8) & 255;
        len[7] = (ullPayloadSize) & 255;
        sOut.append(len, 8);
    }
    
    sOut.append(sIn);
}

int WebSocketProtocol::processHandshake(const TC_HttpRequest &request, string &response)
{
    try
    {
        response.clear();
        
        int iVersion = TC_Common::strto<int>(request.getHeader(Sec_WebSocket_Version));
        if (iValidVersion != iVersion)
        {
            TLOGERROR( "[WebSocketProtocol::processHandshake] error version:" << iVersion << endl);
            return WebSocketProtocol::eErrorVersion;
        }
        
        string sServerKey = request.getHeader(Sec_WebSocket_Key);
        
        sServerKey += Server_Key_Padding;
        
        char cServerKey[30] = {0};
        vector<char> shaSign = TC_SHA::sha1bin(sServerKey.c_str(),sServerKey.length());
        int iKeyLen = TC_Base64::encode( (const unsigned char *)(&shaSign[0]), shaSign.size() > 20 ? 20 :shaSign.size() , cServerKey);
        response = WS_Handshake_Header + string(cServerKey, iKeyLen) + WS_Handshake_Tail;
        
        //LOG->debug() << "[WebSocketProtocol::processHandshake]response:\r\n" << response<< endl;
        
        return WebSocketProtocol::eSuccess;
        
    }
    catch (exception &ex)
    {
        TLOGERROR( "[WebSocketProtocol::processHandshake] error:" << ex.what() << endl);
    }
    catch (...)
    {
        TLOGERROR( "[WebSocketProtocol::processHandshake] error: unknow error." << endl);
    }
    
    return WebSocketProtocol::eExceptionErr;
}

int WebSocketProtocol::encodeAndSend(const string &sData, CurrentPtr current)
{
    try
    {
        string sBuff;
        encodeData(sData, sBuff);
        
        if (sBuff.length() > 0)
        {
            current->sendResponse(sBuff.c_str(), sBuff.length());
        }
        
        return WebSocketProtocol::eSuccess;
        
    }
    catch (exception &ex)
    {
        TLOGERROR( "[WebSocketProtocol::sendData] error:" << ex.what() << endl );
    }
    catch (...)
    {
        TLOGERROR( "[WebSocketProtocol::sendData] error: unknow error." << endl);
    }
    
    return WebSocketProtocol::eExceptionErr;
}


}
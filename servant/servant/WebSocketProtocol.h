#pragma once

#include "util/tc_epoll_server.h"
#include "util/tc_http.h"
#include "servant/Global.h"

// enum WebSocketFrameType {
// 	ERROR_FRAME=0xFF00,
// 	INCOMPLETE_FRAME=0xFE00,

// 	OPENING_FRAME=0x3300,
// 	CLOSING_FRAME=0x3400,

// 	INCOMPLETE_TEXT_FRAME=0x01,
// 	INCOMPLETE_BINARY_FRAME=0x02,

// 	TEXT_FRAME=0x81,
// 	BINARY_FRAME=0x82,

// 	PING_FRAME=0x19,
// 	PONG_FRAME=0x1A
// };

namespace tars
{

class WebSocketProtocol
{
public:
    typedef std::function<bool(const TC_HttpRequest &)> FuncValidator;
    
public:
    const static    int iValidVersion       = 13;

    const static    unsigned char ucPayloadFlag      = 129;

    const static    unsigned char ucBinPayloadFlag   = 130;

    const static    int eIncompletePacket   = 1;
    
    const static    int eUndefinedFailure   = 2;

    const static    int eErrorVersion       = 3;
    
    const static    int eCompleteFrame	    = 4;

    const static    int eIgnoreFrame        = 5;

    const static    int eSuccess            = 0;

    const static    int eExceptionErr       = -1;
    
public:
    static  TC_NetWorkBuffer::PACKET_TYPE parse(TC_NetWorkBuffer &in, vector<char> &out);

    static  bool isHttp(const vector<char> &requestBuf);

    static  int decodeData(const string& sIn, string& sOut);

    static  void encodeData(const string& sIn, string& sOut);
    
    static  int processHandshake(const TC_HttpRequest& request, string& response);

    static  int encodeAndSend(const string& strData, CurrentPtr current);

    static void setValidator(FuncValidator validator);
protected:
    static  TC_NetWorkBuffer::PACKET_TYPE parseBin(TC_NetWorkBuffer &in, vector<char> &out);

    static  int parseDataHeader(TC_NetWorkBuffer &in, unsigned int& nMinExpectedSize, uint64_t& ullPayloadSize, int& iMasksOffset, int& iStartLen, bool& bMultiFrame);

    static TC_NetWorkBuffer::PACKET_TYPE doHttpHandshake(TC_NetWorkBuffer &in, vector<char> &out);

    static string buildErrResponse(int status);
protected:
    const static    string Sec_WebSocket_Key;
    const static    string Sec_WebSocket_Version;
    const static    string Server_Key_Padding;
    const static    string WS_Handshake_Header;
    const static    string WS_Handshake_Tail;
    
    static FuncValidator _funcValidator;
};

}
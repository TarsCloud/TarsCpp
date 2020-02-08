/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "servant/TarsCurrent.h"
#include "servant/ServantHandle.h"
#include "servant/BaseF.h"
#include "servant/Application.h"
#include "tup/tup.h"
#include <cerrno>

namespace tars
{
//////////////////////////////////////////////////////////////////
TarsCurrent::TarsCurrent(ServantHandle *pServantHandle)
: _servantHandle(pServantHandle)
// , _bindAdapter(NULL)
// , _uid(0)
// , _ip("NULL")
// , _port(0)
// , _fd(-1)
, _response(true)
// , _begintime(0)
, _ret(0)
, _reportStat(true)
// , _closeType(-1)
{
}

TarsCurrent::~TarsCurrent()
{
    //TUP调用或单向调用，从服务端上报调用信息
    if(_reportStat)
    {
        if(_request.iVersion == TUPVERSION )
        {
            reportToStat("tup_client");
        }
        else if(_request.cPacketType == TARSONEWAY)
        {
            reportToStat("one_way_client");
        }
        else if(!_data->adapter()->isTarsProtocol() && ServerConfig::ReportFlow)
        {
            //非tars客户端 从服务端上报调用信息
            reportToStat("not_tars_client");
        }
    }
}

const string &TarsCurrent::getIp() const
{
    return _data->ip();
    // return _ip;
}

int TarsCurrent::getPort() const
{
    return _data->port();
    // return _port;
}

uint32_t TarsCurrent::getUId() const
{
    return _data->uid();
    // return _uid;
}

string TarsCurrent::getServantName() const
{
    return _request.sServantName;
}

short TarsCurrent::getRequestVersion() const
{
    return _request.iVersion;
}

map<string, string>& TarsCurrent::getContext()
{
    return _request.context;
}

const map<string, string>& TarsCurrent::getRequestStatus() const
{
    return _request.status;
}

string TarsCurrent::getFuncName() const
{
    return _request.sFuncName;
}

uint32_t TarsCurrent::getRequestId() const
{
    return _request.iRequestId;
}

char TarsCurrent::getPacketType() const
{
    return _request.cPacketType;
}

tars::Int32 TarsCurrent::getMessageType() const
{
    return _request.iMessageType;
}

struct timeval TarsCurrent::getRecvTime() const
{
    timeval tm;
    tm.tv_sec  = _data->recvTimeStamp()/1000;
    tm.tv_usec = (_data->recvTimeStamp()%1000)*1000;

    return tm;
}

void TarsCurrent::setReportStat(bool bReport)
{
    _reportStat = bReport;
}

const vector<char>& TarsCurrent::getRequestBuffer() const
{
	if (_data->adapter()->isTarsProtocol())
	{
		return _request.sBuffer;
	}
	else
	{
		return _data->buffer();
	}

    // return _request.sBuffer;
}

bool TarsCurrent::isResponse() const
{
    return _response;
}

void TarsCurrent::setCloseType(int type)
{
    _data->setCloseType(type);
}

int TarsCurrent::getCloseType() const
{
    return _data->closeType();
}

void TarsCurrent::initialize(const shared_ptr<TC_EpollServer::RecvContext> &data)
// void TarsCurrent::initialize(const TC_EpollServer::tagRecvData &stRecvData)
{
	_data = data;

    _request.sServantName = ServantHelperManager::getInstance()->getAdapterServant(_data->adapter()->getName());

    if (_data->adapter()->isTarsProtocol())
    {
        initialize(_data->buffer());
    }
    // initialize(stRecvData, begintime);
}


void TarsCurrent::initializeClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
	_data = data;

    _request.sServantName = ServantHelperManager::getInstance()->getAdapterServant(_data->adapter()->getName());

}

void TarsCurrent::initialize(const vector<char>& sRecvBuffer)
{
    TarsInputStream<BufferReader> is;

    is.setBuffer(sRecvBuffer.data(), sRecvBuffer.size());

    _request.readFrom(is);
}

// void TarsCurrent::initialize(const TC_EpollServer::tagRecvData &stRecvData, int64_t beginTime)
// {
//      _ip         = stRecvData.ip;

//     _port        = stRecvData.port;

//     _uid         = stRecvData.uid;

//     _fd          = stRecvData.fd;

//     _bindAdapter = stRecvData.adapter.get();

//     _begintime   = beginTime;

//     _request.sServantName = ServantHelperManager::getInstance()->getAdapterServant(stRecvData.adapter->getName());

//     if (_bindAdapter->isTarsProtocol())
//     {
//         initialize(stRecvData.buffer);
//     }
//     else
//     {
//         _request.sBuffer.reserve(stRecvData.buffer.length());

//         _request.sBuffer.resize(stRecvData.buffer.length());

//         ::memcpy(&_request.sBuffer[0], stRecvData.buffer.c_str(), stRecvData.buffer.length());
//     }
// }

// void TarsCurrent::initializeClose(const TC_EpollServer::tagRecvData &stRecvData)
// {
//     _ip   = stRecvData.ip;

//     _port = stRecvData.port;

//     _uid  = stRecvData.uid;

//     _fd   = stRecvData.fd;

//     _bindAdapter = stRecvData.adapter.get();

//     _request.sServantName = ServantHelperManager::getInstance()->getAdapterServant(stRecvData.adapter->getName());

//     _begintime = TNOWMS;
// }

// void TarsCurrent::initialize(const string &sRecvBuffer)
// {
//     TarsInputStream<BufferReader> is;

//     is.setBuffer(sRecvBuffer.c_str(), sRecvBuffer.length());

//     _request.readFrom(is);
// }

void TarsCurrent::sendResponse(const char* buff, uint32_t len)
{
    // _servantHandle->sendResponse(_uid, string(buff, len), _ip, _port, _fd);
	shared_ptr<TC_EpollServer::SendContext> send = _data->createSendContext();
	send->buffer().assign(buff, buff + len);
	_servantHandle->sendResponse(send);
}


void TarsCurrent::sendResponse(int iRet, const vector<char> &buff)
{
	//单向调用不需要返回
	if (_request.cPacketType == TARSONEWAY)
	{
		return;
	}

	// ResponsePacket response;
	// response.sBuffer = buff;
	sendResponse(iRet, buff, TARS_STATUS(), "");
}

void TarsCurrent::sendResponse(int iRet)
{
	// ResponsePacket response;
	sendResponse(iRet, vector<char>(), TARS_STATUS(), "");
}

void TarsCurrent::sendResponse(int iRet, tars::TarsOutputStream<tars::BufferWriterVector>& os)
{
	// ResponsePacket response;
	// os.swap(response.sBuffer);
	sendResponse(iRet, os.getByteBuffer(), TARS_STATUS(), "");
}

void TarsCurrent::sendResponse(int iRet, tup::UniAttribute<tars::BufferWriterVector, tars::BufferReader>& attr)
{
	ResponsePacket response;
	attr.encode(response.sBuffer);
	sendResponse(iRet, response.sBuffer, TARS_STATUS(), "");
}

void TarsCurrent::sendResponse(int iRet, const vector<char> &buffer,  const map<string, string>& status, const string & sResultDesc)
{
    _ret = iRet;

    //单向调用不需要返回
    if (_request.cPacketType == TARSONEWAY)
    {
        return;
    }

	shared_ptr<TC_EpollServer::SendContext> send = _data->createSendContext();

	tars::Int32 iHeaderLen = 0;

	TarsOutputStream<BufferWriterVector> os;

	//先预留4个字节长度
	os.writeBuf((const char *)&iHeaderLen, sizeof(iHeaderLen));

    // TarsOutputStream<BufferWriter> os;

    if (_request.iVersion != TUPVERSION)
    {
        ResponsePacket response;

        response.iRequestId     = _request.iRequestId;
        response.iMessageType   = _request.iMessageType;
		response.cPacketType    = TARSNORMAL;

        response.iVersion       = _request.iVersion;
        response.status         = status;
        response.sBuffer        = std::move(buffer);
        response.sResultDesc    = sResultDesc;
        response.context        = _responseContext;
        response.iRet           = iRet;

        TLOGTARS("[TARS]TarsCurrent::sendResponse :"
                   << response.iMessageType << "|"
                   << _request.sServantName << "|"
                   << _request.sFuncName << "|"
                   << response.iRequestId << endl);

        response.writeTo(os);
    }
    else
    {
        //tup回应包用请求包的结构(这里和新版本TAF是有区别的)
        RequestPacket response;

        response.iRequestId     = _request.iRequestId;
        response.iMessageType   = _request.iMessageType;
		response.cPacketType    = TARSNORMAL;

        response.iVersion       = _request.iVersion;
        response.status         = status;
        response.context        = _responseContext;
        response.sBuffer        = std::move(buffer);
        response.sServantName   = _request.sServantName;
        response.sFuncName      = _request.sFuncName;

        //异常的情况下buffer可能为空，要保证有一个空UniAttribute的编码内容
        if(response.sBuffer.size() == 0)
        {
            tup::UniAttribute<> tarsAttr;
            tarsAttr.setVersion(_request.iVersion);
            tarsAttr.encode(response.sBuffer);
        }
        //iRet为0时,不记录在status里面,节省空间
        if(iRet != 0)
        {
            response.status[ServantProxy::STATUS_RESULT_CODE] = TC_Common::tostr(iRet);
        }
        //sResultDesc为空时,不记录在status里面,节省空间
        if(!sResultDesc.empty())
        {
            response.status[ServantProxy::STATUS_RESULT_DESC] = sResultDesc;
        }

        TLOGTARS("[TARS]TarsCurrent::sendResponse :"
                   << response.iMessageType << "|"
                   << _request.sServantName << "|"
                   << _request.sFuncName << "|"
                   << response.iRequestId << endl);

        response.writeTo(os);
    }

	os.swap(send->buffer());

	assert(send->buffer().size() >= 4);

	iHeaderLen = htonl((int)(send->buffer().size()));

	memcpy(&send->buffer()[0], (const char *)&iHeaderLen, sizeof(iHeaderLen));

	_servantHandle->sendResponse(send);

}


void TarsCurrent::close()
{
    if (_servantHandle)
    {
        _servantHandle->close(_data);
    }
}

ServantHandle* TarsCurrent::getServantHandle()
{
    return _servantHandle;
}

TC_EpollServer::BindAdapter* TarsCurrent::getBindAdapter()
{
    return _data->adapter().get();
}

void TarsCurrent::reportToStat(const string& sObj)
{
    StatReport* stat = Application::getCommunicator()->getStatReport();

    if(stat && stat->getStatPrx())
    {
        // int64_t endtime = TNOWMS;
        // int sptime = endtime - _begintime;

        //被调上报自己的set信息，set信息在setReportInfo设置
        stat->report(sObj, "" , _request.sServantName, _data->ip(), 0, _request.sFuncName, (StatReport::StatResult)_ret, TNOWMS - _data->recvTimeStamp(), 0);
    }
}

////////////////////////////////////////////////////////////////////////////
}

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

#include "servant/Current.h"
#include "servant/ServantHandle.h"
#include "servant/BaseF.h"
#include "servant/Application.h"
#include "tup/tup.h"
#include <cerrno>

namespace tars
{
//////////////////////////////////////////////////////////////////
Current::Current(ServantHandle *pServantHandle)
    : _servantHandle(pServantHandle)
    , _response(true)
    , _ret(0)
    , _reportStat(true)
{
}

Current::~Current()
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

const string &Current::getHostName() const
{
	auto it = _request.context.find("node_name");
	if(it != _request.context.end())
	{
		return it->second;
	}
	return _data->ip();

}

const string &Current::getIp() const
{
    return _data->ip();
}

int Current::getPort() const
{
    return _data->port();
}

uint32_t Current::getUId() const
{
    return _data->uid();
}

string Current::getServantName() const
{
    return _request.sServantName;
}

short Current::getRequestVersion() const
{
    return _request.iVersion;
}

map<string, string>& Current::getContext()
{
    return _request.context;
}

const map<string, string>& Current::getRequestStatus() const
{
    return _request.status;
}

string Current::getFuncName() const
{
    return _request.sFuncName;
}

uint32_t Current::getRequestId() const
{
    return _request.iRequestId;
}

char Current::getPacketType() const
{
    return _request.cPacketType;
}

int Current::getMessageType() const
{
    return _request.iMessageType;
}

struct timeval Current::getRecvTime() const
{
    timeval tm;
    tm.tv_sec  = _data->recvTimeStamp() / 1000;
    tm.tv_usec = (_data->recvTimeStamp() % 1000) * 1000;

    return tm;
}

void Current::setReportStat(bool bReport)
{
    _reportStat = bReport;
}

const vector<char>& Current::getRequestBuffer() const
{
	if (_data->adapter()->isTarsProtocol())
	{
		return _request.sBuffer;
	}
	else
	{
		return _data->buffer();
	}
}

bool Current::isResponse() const
{
    return _response;
}

void Current::setCloseType(int type)
{
    _data->setCloseType(type);
}

int Current::getCloseType() const
{
    return _data->closeType();
}

void Current::initialize(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
	_data = data;

	Application *application = (Application*)this->_servantHandle->getApplication();

	_request.sServantName = application->getServantHelper()->getAdapterServant(_data->adapter()->getName());

//    _request.sServantName = ServantHelperManager::getInstance()->getAdapterServant(_data->adapter()->getName());

    if (_data->adapter()->isTarsProtocol())
    {
        initialize(_data->buffer());
    }
}

void Current::initializeClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
	_data = data;

	Application *application = (Application*)this->_servantHandle->getApplication();

    _request.sServantName = application->getServantHelper()->getAdapterServant(_data->adapter()->getName());
}

void Current::initialize(const vector<char>& sRecvBuffer)
{
    TarsInputStream<BufferReader> is;

    is.setBuffer(sRecvBuffer.data(), sRecvBuffer.size());

    _request.readFrom(is);
}

void Current::sendResponse(const char *buff, uint32_t len)
{
	shared_ptr<TC_EpollServer::SendContext> send = _data->createSendContext();
	send->buffer()->assign(buff, len);
	_servantHandle->sendResponse(send);
}

void Current::sendResponse(int iRet, const vector<char> &buff)
{
	//单向调用不需要返回
	if (_request.cPacketType == TARSONEWAY)
	{
		return;
	}

	ResponsePacket response;
	response.sBuffer = buff;
	sendResponse(iRet, response, TARS_STATUS(), "");
}

void Current::sendResponse(int iRet)
{
	ResponsePacket response;
	sendResponse(iRet, response, TARS_STATUS(), "");
}

void Current::sendResponse(int iRet, tars::TarsOutputStream<tars::BufferWriterVector>& os)
{
	ResponsePacket response;
	os.swap(response.sBuffer);
	sendResponse(iRet, response, TARS_STATUS(), "");
}

void Current::sendResponse(int iRet, tup::UniAttribute<tars::BufferWriterVector, tars::BufferReader>& attr)
{
	ResponsePacket response;
	attr.encode(response.sBuffer);
	sendResponse(iRet, response, TARS_STATUS(), "");
}

void Current::sendResponse(int iRet, ResponsePacket &response,  const map<string, string>& status, const string & sResultDesc)
{
    _ret = iRet;

    //单向调用不需要返回
    if (_request.cPacketType == TARSONEWAY)
    {
        return;
    }

	shared_ptr<TC_EpollServer::SendContext> send = _data->createSendContext();

	Int32 iHeaderLen = 0;

	TarsOutputStream<BufferWriterVector> os;

	//先预留4个字节长度
	os.writeBuf((const char *)&iHeaderLen, sizeof(iHeaderLen));

    if (_request.iVersion != TUPVERSION)
    {
        response.iRequestId     = _request.iRequestId;
		response.cPacketType    = TARSNORMAL;
        response.iMessageType   = _request.iMessageType;
        response.iVersion       = _request.iVersion;
        response.status         = status;
        response.context        = _responseContext;
//        response.sBuffer        = std::move(buffer);
        response.sResultDesc    = sResultDesc;

        response.iRet           = iRet;

        TLOGTARS("Current::sendResponse :"
                   << response.iMessageType << "|"
                   << _request.sServantName << "|"
                   << _request.sFuncName << "|"
                   << response.iRequestId << endl);

        response.writeTo(os);
    }
    else
    {
        //tup回应包用请求包的结构(这里和新版本TAF是有区别的)
        RequestPacket tupResponse;

	    tupResponse.iRequestId     = _request.iRequestId;
	    tupResponse.iMessageType   = _request.iMessageType;
	    tupResponse.cPacketType    = TARSNORMAL;

	    tupResponse.iVersion       = _request.iVersion;
	    tupResponse.status         = status;
	    tupResponse.context        = _responseContext;
	    tupResponse.sBuffer.swap(response.sBuffer);
	    tupResponse.sServantName   = _request.sServantName;
	    tupResponse.sFuncName      = _request.sFuncName;

        //异常的情况下buffer可能为空，要保证有一个空UniAttribute的编码内容
        if(tupResponse.sBuffer.size() == 0)
        {
            tup::UniAttribute<> tarsAttr;
            tarsAttr.setVersion(_request.iVersion);
            tarsAttr.encode(response.sBuffer);
        }
        //iRet为0时,不记录在status里面,节省空间
        if(iRet != 0)
        {
	        tupResponse.status[ServantProxy::STATUS_RESULT_CODE] = TC_Common::tostr(iRet);
        }
        //sResultDesc为空时,不记录在status里面,节省空间
        if(!sResultDesc.empty())
        {
	        tupResponse.status[ServantProxy::STATUS_RESULT_DESC] = sResultDesc;
        }

        TLOGTARS("Current::sendResponse :"
                   << tupResponse.iMessageType << "|"
                   << _request.sServantName << "|"
                   << _request.sFuncName << "|"
                   << tupResponse.iRequestId << endl);

	    tupResponse.writeTo(os);
    }

	assert(os.getLength() >= 4);

	iHeaderLen = htonl((int)(os.getLength()));

	memcpy(os.getByteBuffer().data(), (const char *)&iHeaderLen, sizeof(iHeaderLen));

	send->buffer()->swap(os.getByteBuffer());

	_servantHandle->sendResponse(send);

}

void Current::close()
{
    if (_servantHandle)
    {
        _servantHandle->close(_data);
    }
}

ServantHandle* Current::getServantHandle()
{
    return _servantHandle;
}

TC_EpollServer::BindAdapter* Current::getBindAdapter()
{
    return _data->adapter().get();
}

void Current::reportToStat(const string& sObj)
{
    StatReport* stat = Application::getCommunicator()->getStatReport();

    if(stat && stat->getStatPrx())
    {
        stat->report(sObj, "", _request.sFuncName, _data->ip(), 0, (StatReport::StatResult)_ret, TNOWMS - _data->recvTimeStamp(), 0, false);
    }
}

////////////////////////////////////////////////////////////////////////////
}

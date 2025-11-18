#include "JsonUtil.h"

namespace SipENUMProxy
{
    std::string JsonUtil::jsonTextEncode(const std::string & authKey, int serial, const std::string & channel, const std::string & caller, const std::string & callee,
        const std::string & callId, const std::string & callForwording, const std::string & sipgatewayIp, const std::string & enterpriseIp, std::string & time)
    {
        VoipApp::ReqHead head;
        head.SERIAL = serial;
        head.METHOD = "JEGO_CALL_REQ";
        head.AUTH_KEY = authKey;
        head.CHANNEL = channel;
        
        VoipApp::ReqBody body;
        body.TYPE = "REQUEST";
        body.TIME = time;
        body.CALL_ID = callId;
        body.SRC_GW = sipgatewayIp;
        body.SRC_ENT_ADDR = enterpriseIp;
        body.SRC_CALL_USER = caller;
        body.CALL_FORWORDING_NUMBER = callForwording;
        body.DST_CALL_USER = callee;
        
        VoipApp::ReqMsg msg;
        msg.uip_version = 2;
        msg.uip_head = head;
        msg.uip_body = body;
        return msg.writeToJsonString(); 
    }

    bool JsonUtil::jsonTextEncode(const VoipApp::JegoRouterInfo & info, int serial, const std::string & from, const std::string & to, const std::string & gwIp, const std::string & entIp, int entPort, int numberType, Json::Value & jsonvalue)
    {
        VoipApp::RspHead head;
        head.SERIAL = serial;
        head.METHOD = "JEGO_CALL_REQ";
        head.CHANNEL = "JUPHOON";
        head.RESULT_CODE = info.resultCode;
        head.RESULT_DESC = "ok";

        VoipApp::RspBody body;
        body.ROUTE_ID = info.routeId;
        body.ATOM_ID = info.atomId;
        body.SERVICE_TYPE = info.serviceType;
        body.RECORD = info.record;
        body.BEEP = info.beep;
        body.FORCE_SIP_INBOUND = info.sipInbound;
        body.FORCE_SIP_OUTBOUND = info.sipOutbound;
        body.TYPE = info.type;
        body.TIME = info.time;
        body.CALL_ID = info.callId;
        body.LAST_GW = info.gwAddr;
        body.DST_ENT_ADDR = info.enterpriseIp;
        body.DST_ENT_PORT = info.enterprisePort;
        body.SRC_CALL_USER = info.caller;
        body.DST_CALL_USER = info.callee;
        body.CALLING_E164_NUMBER = info.number;
        body.CHARGE_USER = info.chargeAccount;
        body.MIDDLE_CALL_USER = info.midCallUser;
        body.ORIG_SRC_CALL_USER = info.origSrcUser;
        body.ORIG_DST_CALL_USER = info.origDstUser;
        body.PRICE_NUMBER = info.priceNumber;
        body.FROM = from;
        body.TO = to;
        body.NUMBER_TYPE_FLAG = numberType;

        if (!info.gwAddr.empty() && !info.enterpriseIp.empty())
        {
            VoipApp::RspRouteInfo obj;
            obj.DST_GW = info.gwAddr;
            obj.DST_ENT_ADDR = info.enterpriseIp;
            obj.DST_ENT_PORT = info.enterprisePort;
            body.DST_ARRAY.push_back(obj);
        }

        if (!gwIp.empty() && !entIp.empty())
        {
            VoipApp::RspRouteInfo obj;
            obj.DST_GW = gwIp;
            obj.DST_ENT_ADDR = entIp;
            obj.DST_ENT_PORT = entPort;
            body.DST_ARRAY.push_back(obj);
        }

        VoipApp::RspMsg msg;
        msg.uip_version = 2;
        msg.uip_head = head;
        msg.uip_body = body;
        std::string text = msg.writeToJsonString();

        Json::Reader reader;
        if (!reader.parse(text.c_str(), jsonvalue))
        {
            TLOGWARN("[JsonUtil] " << __FUNCTION__ << "() called, parse failed text:" << text << endl);
            return false;
        }

        TLOGINFO("[JsonUtil] " << __FUNCTION__ << "() called, response text:" << text << endl);
        return true;
    }

    bool JsonUtil::jsonTextDecode(const std::string & content, std::string & callId, int & serial, std::string &time, std::string &caller, std::string & callee, std::string & reason)
    {
        VoipApp::RspMsg msg;
        msg.readFromJsonString(content);

        callId = msg.uip_body.CALL_ID;
        serial = msg.uip_head.SERIAL;
        time = msg.uip_body.TIME;
        caller = msg.uip_body.SRC_CALL_USER;
        callee = msg.uip_body.DST_CALL_USER;
        return true;
    }

    bool JsonUtil::jsonTextDecode(const std::string & content, VoipApp::JegoRouterInfo & info, int & serial, std::string &desc, std::map<std::string, std::string> &params, std::string & reason)
    {
        VoipApp::RspMsg msg;
        msg.readFromJsonString(content);
 
        serial = msg.uip_head.SERIAL;
        desc = msg.uip_head.RESULT_DESC;

        info.version = msg.uip_version;
        info.type = msg.uip_body.TYPE;
        info.time = msg.uip_body.TIME;
        info.caller = msg.uip_body.SRC_CALL_USER;
        info.callee = msg.uip_body.DST_CALL_USER;
        info.callId = msg.uip_body.CALL_ID;
        info.gwAddr = msg.uip_body.LAST_GW;
        info.number = msg.uip_body.CALLING_E164_NUMBER;
        info.chargeAccount = msg.uip_body.CHARGE_USER;
        info.enterpriseIp = msg.uip_body.DST_ENT_ADDR;
        info.enterprisePort = msg.uip_body.DST_ENT_PORT;
        info.midCallUser = msg.uip_body.MIDDLE_CALL_USER;
        info.origSrcUser = msg.uip_body.ORIG_SRC_CALL_USER;
        info.origDstUser = msg.uip_body.ORIG_DST_CALL_USER;
        info.routeId = msg.uip_body.ROUTE_ID;
        info.atomId = msg.uip_body.ATOM_ID;
        info.priceNumber = msg.uip_body.PRICE_NUMBER;
        info.resultCode = msg.uip_head.RESULT_CODE;
        info.serviceType = msg.uip_body.SERVICE_TYPE;
        info.sipInbound = msg.uip_body.FORCE_SIP_INBOUND;
        info.sipOutbound = msg.uip_body.FORCE_SIP_OUTBOUND;

        if (msg.uip_body.BEEP)
            info.beep = msg.uip_body.BEEP;

        if (msg.uip_body.RECORD)
            info.record = msg.uip_body.RECORD;

        if (!msg.uip_body.FROM.empty())
            params[SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_FROM] = msg.uip_body.FROM;

        if (!msg.uip_body.TO.empty())
            params[SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_TO] = msg.uip_body.TO;

        params[SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_NUMBER_TYPE_FLAG] = std::to_string(msg.uip_body.NUMBER_TYPE_FLAG);

        for (int i = 0; i < msg.uip_body.DST_ARRAY.size(); i++)
        {
            VoipApp::RspRouteInfo rspRoute = msg.uip_body.DST_ARRAY.at(i);
            VoipApp::RouteInfo routeInfo;
            routeInfo.gwAddr = rspRoute.DST_GW;
            routeInfo.ipAddr = rspRoute.DST_ENT_ADDR;
            routeInfo.port = rspRoute.DST_ENT_PORT;
            info.routeInfoList.push_back(routeInfo);

            if (i == 0)
            {
                info.gwAddr = rspRoute.DST_GW;
                info.enterpriseIp = rspRoute.DST_ENT_ADDR;
                info.enterprisePort = rspRoute.DST_ENT_PORT;
            }
        }

        return true;
    }

}
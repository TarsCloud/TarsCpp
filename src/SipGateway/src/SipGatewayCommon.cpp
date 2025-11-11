#include "SipGatewayCommon.h"

namespace SipGateway
{
    ErrorCodeManager::ErrorCodeManager()
    {
        sipErrCodeDescInit();
        serverErrCodeDescInit();
        otherErrCodeDescInit();
        mediaErrCodeDescInit();
        mediaErrCodeInit();
    }

    void ErrorCodeManager::sipErrCodeDescInit()
    {
        m_mapErrCodeDesc[400] = "Bad Request";
        m_mapErrCodeDesc[401] = "Unauthorized";
        m_mapErrCodeDesc[402] = "Payment Required";
        m_mapErrCodeDesc[403] = "Forbidden";
        m_mapErrCodeDesc[404] = "Not Found";
        m_mapErrCodeDesc[405] = "Method Not Allowed";
        m_mapErrCodeDesc[406] = "Not Acceptable";
        m_mapErrCodeDesc[408] = "Request Timeout";
        m_mapErrCodeDesc[409] = "Conflicts";
        m_mapErrCodeDesc[410] = "Gone";
        m_mapErrCodeDesc[411] = "Length Required";
        m_mapErrCodeDesc[412] = "Conditional Request Failed";
        m_mapErrCodeDesc[413] = "Request Entity Too Large";
        m_mapErrCodeDesc[414] = "Request URI Too Long";
        m_mapErrCodeDesc[415] = "Unsupported Media Type";
        m_mapErrCodeDesc[416] = "Unsupported URI Scheme";
        m_mapErrCodeDesc[417] = "Unknown Resource-Priority";
        m_mapErrCodeDesc[420] = "Bad Extention";
        m_mapErrCodeDesc[421] = "Extension Required";
        m_mapErrCodeDesc[422] = "Session Interval Too Small";
        m_mapErrCodeDesc[423] = "Interval Too Brief";
        m_mapErrCodeDesc[428] = "Use Identity Header";
        m_mapErrCodeDesc[429] = "Provide Referrer Identity";
        m_mapErrCodeDesc[436] = "Bad Identity-Info";
        m_mapErrCodeDesc[437] = "Unsupported Certificate";
        m_mapErrCodeDesc[438] = "Invalid Identity Header";
        m_mapErrCodeDesc[480] = "Temporarily Unavailable";
        m_mapErrCodeDesc[481] = "Call Leg/Transaction Does Not Exist";
        m_mapErrCodeDesc[482] = "Loop Detected";
        m_mapErrCodeDesc[483] = "Too Many Hops";
        m_mapErrCodeDesc[484] = "Address Incomplete";
        m_mapErrCodeDesc[485] = "Ambiguous";
        m_mapErrCodeDesc[486] = "Busy Here";
        m_mapErrCodeDesc[487] = "Request Terminated";
        m_mapErrCodeDesc[488] = "Not Acceptable Here";
        m_mapErrCodeDesc[489] = "Bad Event";
        m_mapErrCodeDesc[491] = "Request Pending";
        m_mapErrCodeDesc[493] = "Undecipherable";
        m_mapErrCodeDesc[494] = "Security Agreement Required";
        m_mapErrCodeDesc[500] = "Server Internal Error";
        m_mapErrCodeDesc[501] = "Not Implemented";
        m_mapErrCodeDesc[502] = "Bad Gateway";
        m_mapErrCodeDesc[503] = "Service Unavailable";
        m_mapErrCodeDesc[504] = "Server Timeout";
        m_mapErrCodeDesc[505] = "Version Not Supported";
        m_mapErrCodeDesc[513] = "Message Too Large";
        m_mapErrCodeDesc[533] = "Service Lost";
        m_mapErrCodeDesc[580] = "Precondition Failure";
        m_mapErrCodeDesc[600] = "Busy Everywhere";
        m_mapErrCodeDesc[603] = "Decline";
        m_mapErrCodeDesc[604] = "Does Not Exist Anywhere";
        m_mapErrCodeDesc[606] = "Not Acceptable";
    }

    void ErrorCodeManager::serverErrCodeDescInit()
    {
        m_mapErrCodeDesc[kSipCallError100] = "Find SIP resource failed";
        m_mapErrCodeDesc[kSipCallError101] = "Call JMDS create func failed";
        m_mapErrCodeDesc[kSipCallError102] = "Get conference record failed";
        m_mapErrCodeDesc[kSipCallError103] = "No heart beat";
        m_mapErrCodeDesc[kSipCallError104] = "No INVITE send or recv";
        m_mapErrCodeDesc[kSipCallError105] = "Send message to SBC failed";
        m_mapErrCodeDesc[kSipCallError106] = "Callee no response";
        m_mapErrCodeDesc[kSipCallError107] = "Call CallServerEx create func failed";
        m_mapErrCodeDesc[kSipCallError108] = "Call CallServerEx waitingAcception func failed";
        m_mapErrCodeDesc[kSipCallError109] = "Call CallServerEx accept func failed";
        m_mapErrCodeDesc[kSipCallError110] = "Dialog not exist";
        m_mapErrCodeDesc[kSipCallError111] = "Time out for no 180";
        m_mapErrCodeDesc[kSipCallError112] = "Time out for no 200";
        m_mapErrCodeDesc[kSipCallError113] = "OnInvited failed";
        m_mapErrCodeDesc[kSipCallError114] = "Dialog time out";
        m_mapErrCodeDesc[kSipCallError115] = "Transfer onInvite failed";
        m_mapErrCodeDesc[kSipCallError116] = "Generate id failed";
        m_mapErrCodeDesc[kSipCallError117] = "Get callee prefix failed";
        m_mapErrCodeDesc[kSipCallError118] = "Alloc rtp resource failed";
        m_mapErrCodeDesc[kSipCallError119] = "Repeated call";
        m_mapErrCodeDesc[kSipCallError120] = "Caller account format error";
        m_mapErrCodeDesc[kSipCallError121] = "Callee account format error";
        m_mapErrCodeDesc[kSipCallError122] = "Not sufficient funds";
        m_mapErrCodeDesc[kSipCallError123] = "Domain not configured";
        m_mapErrCodeDesc[kSipCallError124] = "Not alive SipProxy";
        m_mapErrCodeDesc[kSipCallError125] = "SipProxy not configured";
        m_mapErrCodeDesc[kSipCallError126] = "Account fee expired";
        m_mapErrCodeDesc[kSipCallError127] = "Interface exception";
        m_mapErrCodeDesc[kSipCallError128] = "Inner error";
        m_mapErrCodeDesc[kSipCallError129] = "No sdp";
        m_mapErrCodeDesc[kSipCallError130] = "num routing param error";
        m_mapErrCodeDesc[kSipCallError131] = "call CallServerEx release func failed";
        m_mapErrCodeDesc[kSipCallError132] = "callee not exist";
        m_mapErrCodeDesc[kSipCallError133] = "callee busy";
    }

    void ErrorCodeManager::mediaErrCodeDescInit()
    {
        m_mapErrCodeDesc[kSipCallError700] = "App no media";
        m_mapErrCodeDesc[kSipCallError701] = "Sip no media";
        m_mapErrCodeDesc[kSipCallError702] = "App and sip no media";

        m_mapErrCodeDesc[kSipCallError710] = "App no rtp pkg";
        m_mapErrCodeDesc[kSipCallError711] = "Sip no rtp pkg";
        m_mapErrCodeDesc[kSipCallError712] = "App and sip no rtp pkg";

        m_mapErrCodeDesc[kSipCallError720] = "App rtp is empty pkg";
        m_mapErrCodeDesc[kSipCallError721] = "Sip rtp is empty pkg";
        m_mapErrCodeDesc[kSipCallError722] = "App and sip rtp are empty pkg";

        m_mapErrCodeDesc[kSipCallError700] = "released by app and app no media";
        m_mapErrCodeDesc[kSipCallError701] = "released by app and cs no media";
        m_mapErrCodeDesc[kSipCallError702] = "released by app without calling connect";
        m_mapErrCodeDesc[kSipCallError703] = "released by app in 2s";
        m_mapErrCodeDesc[kSipCallError704] = "released by app and app/cs no media";
        m_mapErrCodeDesc[kSipCallError705] = "released by app and app empty pkg";
        m_mapErrCodeDesc[kSipCallError706] = "released by app and cs empty pkg";
        m_mapErrCodeDesc[kSipCallError707] = "released by app and app/cs empty pkg";

        m_mapErrCodeDesc[kSipCallError710] = "released by cs and app no media";
        m_mapErrCodeDesc[kSipCallError711] = "released by cs and cs no media";
        m_mapErrCodeDesc[kSipCallError712] = "released by cs without calling connect";
        m_mapErrCodeDesc[kSipCallError713] = "released by cs in 2s";
        m_mapErrCodeDesc[kSipCallError714] = "released by cs and app/cs no media";
        m_mapErrCodeDesc[kSipCallError715] = "released by cs and app empty pkg";
        m_mapErrCodeDesc[kSipCallError716] = "released by cs and cs empty pkg";
        m_mapErrCodeDesc[kSipCallError717] = "released by cs and app/cs empty pkg";

        m_mapErrCodeDesc[kSipCallError720] = "released by server and app no media";
        m_mapErrCodeDesc[kSipCallError721] = "released by server and cs no media";
        m_mapErrCodeDesc[kSipCallError722] = "released by server without calling connect";
        m_mapErrCodeDesc[kSipCallError724] = "released by server and app/cs no media";
        m_mapErrCodeDesc[kSipCallError725] = "released by server and app empty pkg";
        m_mapErrCodeDesc[kSipCallError726] = "released by server and cs empty pkg";
        m_mapErrCodeDesc[kSipCallError727] = "released by server and app/cs empty pkg";

        m_mapErrCodeDesc[kSipCallError800] = "release time > accepted time";
        m_mapErrCodeDesc[kSipCallError801] = "release by media channel create failed";
    }

    void ErrorCodeManager::otherErrCodeDescInit()
    {
        m_mapErrCodeDesc[kSipCallError3006] = "Account in black list";
        m_mapErrCodeDesc[kSipCallError3007] = "SIM card abnormal";
        m_mapErrCodeDesc[kSipCallError4012] = "Time limited";
    }

    void ErrorCodeManager::mediaErrCodeInit()
    {
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_APP_NO_MEDIA)] = kSipCallError700;
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_SIP_NO_MEDIA)] = kSipCallError701;
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_NOT_CAPABLE_OF_DETECTION)] = kSipCallError702;
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_APP_SIP_NO_MEDIA)] = kSipCallError704;
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_APP_EMPTY_PKG)] = kSipCallError705;
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_SIP_EMPTY_PKG)] = kSipCallError706;
        m_mediaErrCodeMap[std::make_pair(0, SIP_ACCOUNT_CHARGE_KEY_APP_SIP_EMPTY_PKG)] = kSipCallError707;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_APP_NO_MEDIA)] = kSipCallError710;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_SIP_NO_MEDIA)] = kSipCallError711;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_NOT_CAPABLE_OF_DETECTION)] = kSipCallError712;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_APP_SIP_NO_MEDIA)] = kSipCallError714;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_APP_EMPTY_PKG)] = kSipCallError715;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_SIP_EMPTY_PKG)] = kSipCallError716;
        m_mediaErrCodeMap[std::make_pair(1, SIP_ACCOUNT_CHARGE_KEY_APP_SIP_EMPTY_PKG)] = kSipCallError717;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_APP_NO_MEDIA)] = kSipCallError720;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_SIP_NO_MEDIA)] = kSipCallError721;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_NOT_CAPABLE_OF_DETECTION)] = kSipCallError722;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_APP_SIP_NO_MEDIA)] = kSipCallError724;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_APP_EMPTY_PKG)] = kSipCallError725;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_SIP_EMPTY_PKG)] = kSipCallError726;
        m_mediaErrCodeMap[std::make_pair(2, SIP_ACCOUNT_CHARGE_KEY_APP_SIP_EMPTY_PKG)] = kSipCallError727;
    }

    int ErrorCodeManager::getMediaErrorCode(int type, const std::string& reason)
    {
        ErrCodeMap::iterator it = m_mediaErrCodeMap.find(std::make_pair(type, reason));
        if (it != m_mediaErrCodeMap.end())
        {
            return it->second;
        }

        return 0;
    }

    std::string ErrorCodeManager::getErrorCodeDesc(int errCode)
    {
        std::map<int, std::string>::iterator itor = m_mapErrCodeDesc.find(errCode);
        if (itor != m_mapErrCodeDesc.end())
        {
            return itor->second;
        }

        return "";
    }

}
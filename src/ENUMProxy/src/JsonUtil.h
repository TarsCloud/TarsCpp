#ifndef __JSON_UTIL__
#define __JSON_UTIL__


#include "SipRouter.h"
#include "json/json.h"
#include "SipCallController/src/SipGW3Common.h"

#include "util/tc_autoptr.h"


namespace SipENUMProxy
{
    class JsonUtil : public tars::TC_HandleBase
    {
    public:
        static std::string jsonTextEncode(const std::string& authKey, int serial, const std::string& channel, const std::string& caller, const std::string& callee,
            const std::string& callId, const std::string& callForwording, const std::string& sipgatewayIp, const std::string& enterpriseIp, std::string& time);

        static bool jsonTextEncode(const VoipApp::JegoRouterInfo & info, int serial, const std::string & from, const std::string & to, const std::string & gwIp, const std::string & entIp, int entPort, int numberType, Json::Value & jsonvalue);

        static bool jsonTextDecode(const std::string & content, std::string & callId, int & serial, std::string &time, std::string &caller, std::string & callee, std::string & reason);

        static bool jsonTextDecode(const std::string & content, VoipApp::JegoRouterInfo & info, int & serial, std::string &desc, std::map<std::string, std::string> &params, std::string & reason);

        template <typename T>
        static inline std::string toJson(const T &t);

        template <typename T>
        static inline bool fromJson(T &t, const std::string &json);

    };
    typedef tars::TC_AutoPtr<JsonUtil> JsonUtilPtr;

    template <typename T>
    inline std::string JsonUtil::toJson(const T &t)
    {
        return t.writeToJsonString();
    }

    template <typename T>
    inline bool JsonUtil::fromJson(T &t, const std::string &json)
    {
        try
        {
            t.readFromJsonString(json);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
}

#endif //__JSON_UTIL__
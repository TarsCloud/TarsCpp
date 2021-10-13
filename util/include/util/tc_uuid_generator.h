
#ifndef __TC_UUID_GENERATOR_H
#define __TC_UUID_GENERATOR_H

#include "util/tc_platform.h"
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <unistd.h>
//#include <arpa/inet.h>
#else
#include <process.h>
//#include <winsock2.h>
#endif

#include <atomic>
#include "util/tc_common.h"
#include "util/tc_timeprovider.h"
#include "util/tc_socket.h"
#include "util/tc_singleton.h"

namespace tars
{

class TC_UUIDGenerator : public TC_Singleton<TC_UUIDGenerator>
{
public:
    TC_UUIDGenerator():initOK(false)
    {
        init("");
    }

    void init(const string& sIP = "")
    {
        if (isIPV4(sIP))
        {
            //ip = (uint32_t)inet_addr(sIP.c_str());
            ip = ipv4Toint(sIP);
        }
        else
        {
            //ip = (uint32_t)inet_addr(getLocalIP().c_str());
            ip = ipv4Toint(getLocalIP().c_str());
        }

        if (ip == 0)
        {
            initOK = false;
            return;
        }

        pid = (uint32_t)getpid();
        seq = 0;
        initOK = true;
    }

    string genID()
    {
        if (!initOK)
        {
            return "";
        }
        char buff[33] = {0};
        sprintf(buff, "%08x%08x%08x%08x", ip, pid, (unsigned int)(TNOW), seq++);
        return string(buff);
    }

protected:
    bool isIPV4(const string& ip)
    {
        vector<int> vs = TC_Common::sepstr<int>(ip, ".");
        if (vs.size() != 4)
        {
            return false;
        }

        for (size_t i = 0; i < vs.size(); i++)
        {
            if (vs[i] < 0 || vs[i] > 255)
            {
                return false;
            }
        }

        return true;
    }

    uint32_t ipv4Toint(const string& ip)
    {
        vector<int> vs = TC_Common::sepstr<int>(ip, ".");
        if (vs.size() != 4)
        {
            return 0;
        }

        uint32_t ipInt = 0;
        for (int i = 3; i >= 0; i--)
        {
            if (vs[i] < 0 || vs[i] > 255)
            {
                return 0;
            }
            else
            {
                ipInt = (ipInt << 8) + vs[i];
            }
        }

        return ipInt;
    }

    string getLocalIP()
    {
        vector<string> vs = TC_Socket::getLocalHosts();

        for (size_t i = 0; i < vs.size(); i++)
        {
            if (vs[i] != "127.0.0.1" && (!TC_Socket::addressIsIPv6(vs[i])))
            {
                return vs[i];
            }
        }
        return "127.0.0.1";
    }

private:
    uint32_t ip;
    uint32_t pid;
    std::atomic<uint32_t> seq;
    bool initOK;
};

}

#endif //__TC_UUID_GENERATOR_H

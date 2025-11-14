#ifndef __DbUtil_h
#define __DbUtil_h

#include <string>
#include <list>
#include <map>
#include <vector>

namespace DbConn
{
    bool parseMonoUri(const std::string& mongoUri, std::string& parseUri);

    class URI
    {
    public:
        URI(const std::string& uri);
        bool valid() const;
        std::string proto() const;
        std::string host() const;
        std::string port() const;
        void addrs(std::list<std::string>& hosts, std::list<std::string>& ports) const;
        std::string path() const;
        std::string param(const std::string& key) const;
        const std::map<std::string, std::string>& params() const;
    private:
        bool _valid;
        std::string _proto;
        std::string _path;
        std::list<std::string> _hosts, _ports;
        std::map<std::string, std::string> _params;
    };

}

#endif

#include "DbUtil.h"
#include "PBEWithMD5AndDES.h"
#include "util/tc_common.h"
#include "servant/Application.h"

namespace DbConn
{
    bool parseMonoUri(const std::string& mongoUri, std::string& parseUri)
    {
        do
        {
            if (mongoUri.find("mongodb://") == -1)
                break;

            std::string user,pwd,host;
            int pos = mongoUri.find('@');
            if (pos != -1)
            {
                host = mongoUri.substr(pos+1);
                std::string subUri = mongoUri.substr(10, pos - 10);
                pos = subUri.find(':');
                if (pos != -1)
                {
                    user = subUri.substr(0,pos);
                    pwd = subUri.substr(pos+1);
                    unsigned char* decrypt;
                    size_t outlen;
                    std::string salt = "sipCloud";
                    if (PBEWithMD5AndDES_decrypt((char*)pwd.c_str(), (unsigned char*)salt.c_str(), salt.size(), &decrypt, &outlen) != NULL)
                    {
                        pwd = std::string((char*)decrypt, (int)outlen);
                        free(decrypt);
                    }
                    parseUri = "mongodb://" + user + ":" + pwd + "@" + host;
                }
                else
                    break;
            }
            else
                parseUri = mongoUri;

            return true;
        } while (0);

        return false;
    }

    URI::URI(const std::string& uri)
    {
        do
        {
            int i = uri.find("://"), k;
            std::string addrs;
            if (i == -1)
                break;
            k = uri.find('/', i + 3);
            _proto = uri.substr(0, i);
            addrs = uri.substr(i + 3, k - i - 3);

            std::vector<std::string> addrs_ = TC_Common::sepstr<std::string>(addrs, ";");
            if (addrs_.empty())
                break;
            std::vector<std::string>::const_iterator it = addrs_.begin();
            for (; it != addrs_.end(); ++it)
            {
                std::string host, port;
                int j = it->find(':');
                if (j == -1)
                {
                    host = *it;
                }
                else
                {
                    host = it->substr(0, j);
                    port = it->substr(j + 1);
                    int port_ = TC_Common::strto<int>(port);
                    if (port_ <= 0 || port_ < 1024
                        || port_ > 65535)
                        break;
                }
                if (host.empty())
                    break;
                _hosts.push_back(host);
                _ports.push_back(port);
            }
            if (it != addrs_.end())
                break;

            if (k != -1)
            {
                int l = uri.find('?', k + 1);
                if (l == -1)
                {
                    _path = uri.substr(k);
                }
                else
                {
                    _path = uri.substr(k, l - k);
                    std::vector<std::string> params = TC_Common::sepstr<std::string>(uri.substr(l + 1), "&");
                    std::vector<std::string>::const_iterator it = params.begin();
                    for (; it != params.end(); ++it)
                    {
                        int o = it->find('=');
                        if (o != -1)
                        {
                            std::string key = it->substr(0, o), value = it->substr(o + 1);
                            if (key == "pwd")
                            {
                                unsigned char* decrypt;
                                size_t outlen;
                                std::string salt = "juphoon419708";
                                if (PBEWithMD5AndDES_decrypt((char*)value.c_str(), (unsigned char*)salt.c_str(), salt.size(), &decrypt, &outlen) != NULL)
                                {
                                    value = std::string((char*)decrypt, (int)outlen);
                                    free(decrypt);
                                }
                                else
                                    TLOGERROR("[DbConn] PBEWithMD5AndDES_decrypt error" << endl);
                            }
                            _params.insert(make_pair(key, value));
                        }
                    }
                }
            }
            _valid = true;
            return;
        } while (0);
        _valid = false;
        _proto.clear();
        _path.clear();
        _hosts.clear();
        _ports.clear();
        _params.clear();
    }

    bool URI::valid() const
    {
        return _valid;
    }

    std::string URI::proto() const
    {
        return _proto;
    }

    std::string URI::host() const
    {
        return *_hosts.begin();
    }

    std::string URI::port() const
    {
        return *_ports.begin();
    }

    void URI::addrs(list<std::string>& hosts, list<std::string>& ports) const
    {
        hosts = _hosts;
        ports = _ports;
    }

    std::string URI::path() const
    {
        return _path;
    }

    std::string URI::param(const std::string& key) const
    {
        std::map<std::string, std::string>::const_iterator it = _params.find(key);
        return it == _params.end() ? "" : it->second;
    }

    const std::map<std::string, std::string>& URI::params() const
    {
        return _params;
    }
}

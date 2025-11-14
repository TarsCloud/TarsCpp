#include "MysqlHelper.h"
#include "SqlConn.h"
#include "json/json.h"

namespace DbConn
{
    bool jsonToMap(const std::string& s, std::map<std::string, std::string>& m)
    {
        m.clear();
        if (s.empty())
            return true;

        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(s.c_str(), root))
            return false;

        Json::ValueIterator it = root.begin();
        for (; it != root.end(); ++it)
        {
            if (!it->isString())
                return false;
            m[it.name().c_str()] = it->asCString();
        }
        return true;
    }

    std::string mapToJson(const std::map<std::string, std::string>& m)
    {
        Json::Value root;
        std::map<std::string, std::string>::const_iterator it = m.begin();
        for (; it != m.end(); ++it)
            root[it->first.c_str()] = Json::Value(it->second.c_str());
        Json::FastWriter fastWriter;
        return fastWriter.write(root).c_str();
    }

    namespace SQL
    {
        std::string escapeStr(const std::string& str)
        {
            int i = 0, j;
            std::string str1, str2;
            while (i < str.size())
            {
                j = str.find('\\', i);
                if (j > -1)
                {
                    str1 += str.substr(i, j - i) + "\\\\";
                    i = j + 1;
                }
                else
                {
                    str1 += str.substr(i, -1);
                    break;
                }
            }
            i = 0;
            while (i < str1.size())
            {
                j = str1.find('\'', i);
                if (j > -1)
                {
                    str2 += str1.substr(i, j - i) + "\\\'";
                    i = j + 1;
                }
                else
                {
                    str2 += str1.substr(i, -1);
                    break;
                }
            }
            return str2;
        }
    }
}

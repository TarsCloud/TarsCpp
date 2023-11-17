
#include "ConfigImp.h"

map<string, string> ConfigImp::files;
std::mutex ConfigImp::_mutex;

void ConfigImp::setConfigFile(const string &name, const string &content)
{
    std::lock_guard<std::mutex> lock(_mutex);
    files[name] = content;
}

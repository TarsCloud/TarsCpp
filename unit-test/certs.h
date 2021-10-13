#ifndef PATH_HEAER
#define PATH_HEAER

#include "util/tc_config.h"

using namespace tars;

static const string CONFIGPATH(CMAKE_SOURCE_DIR);

inline TC_Config CONFIG()
{
	string confFile = TC_Common::replace(TC_File::load2str(CONFIGPATH + "/server/hello.conf"), "PROJECT_PATH", CONFIGPATH);

	TC_Config conf;
	conf.parseString(confFile);

    return conf;    
} 

inline TC_Config CLIENT_CONFIG()
{
	string confFile = TC_Common::replace(TC_File::load2str(CONFIGPATH + "/server/client.conf"), "PROJECT_PATH", CONFIGPATH);

	TC_Config conf;
	conf.parseString(confFile);

    return conf;
}

inline TC_Config FRAMEWORK_CONFIG()
{
	TC_Config conf;
	conf.parseFile(CONFIGPATH + "/server/framework.conf");

	return conf;
}

inline TC_Config RPC1_CONFIG()
{
	TC_Config conf;
	conf.parseFile(CONFIGPATH + "/server/rpc1.conf");

	return conf;
}

inline TC_Config RPC2_CONFIG()
{
	TC_Config conf;
	conf.parseFile(CONFIGPATH + "/server/rpc2.conf");

	return conf;
}

inline TC_Config RPC3_CONFIG()
{
	TC_Config conf;
	conf.parseFile(CONFIGPATH + "/server/rpc3.conf");

	return conf;
}

inline TC_Config WIN_CONFIG()
{
	TC_Config conf;
	conf.parseFile(CONFIGPATH + "/server/windows.conf");

	return conf;
}

#define CA (CONFIGPATH + "/certs/ca.crt")
#define SERVER_CERT (CONFIGPATH + "/certs/server.crt")
#define CLIENT_CERT (CONFIGPATH + "/certs/client.crt")
#define SERVER_KEY (CONFIGPATH + "/certs/server.key")
#define CLIENT_KEY (CONFIGPATH + "/certs/client.key")

#endif




#ifndef _CONFIGIMP_H_
#define _CONFIGIMP_H_
#include "servant/ConfigF.h"
#include "util/tc_file.h"

using namespace tars;

class ConfigImp : public Config
{
public:
    /**
     *
     */
    ConfigImp(){};

    /**
     *
     */
    virtual ~ConfigImp(){};

    /**
     * 初始化
     *
     * @return int
     */
    virtual void initialize()
    {
        files["test.conf"] = "test-content";
    };

    /**
     * 退出
     */
    virtual void destroy() {};

    virtual int ListConfig(const string &app,const string &server,  vector<string> &vf,tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << app << ", " << server << endl;
        return 0;
    }

    /**
     * 加载配置文件
     * param app :应用
     * param server:  server名
     * param filename:  配置文件名
     *
     * return  : 配置文件内容
     */
    virtual int loadConfig(const std::string & app, const std::string & server, const std::string & filename, string &config, tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << app << ", " << server << ", " << filename << endl;

        map<string, string>::iterator it =files.find(filename);
        if(it!=files.end()){
            config = it->second;
        }
        return 0;
    }

    /**
     * 根据ip获取配置
     * @param appServerName
     * @param filename
     * @param host
     * @param config
     *
     * @return int
     */
    virtual int loadConfigByHost(const string &appServerName, const string &filename, const string &host, string &config, tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << appServerName << ", " << filename << ", " << host << endl;
        return 0;
    }

    /**
     *
     * @param appServerName
     * @param filename
     * @param host
     * @param current
     *
     * @return int
     */
    virtual int checkConfig(const string &appServerName, const string &filename, const string &host, string &result, tars::CurrentPtr current)
    {
		LOG_CONSOLE_DEBUG << appServerName << ", " << filename << ", " << host << endl;

        return 0;
    }

    /**
     * 获取配置文件列表
     * param configInfo ConfigInfo
     * param vf: 配置文件名
     *
     * return  : 配置文件内容
     */
    virtual int ListConfigByInfo(const ConfigInfo& configInfo,  vector<string> &vf,tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << configInfo.writeToJsonString() << endl;

        return 0;
    }

    /**
     * 加载配置文件
     * param configInfo ConfigInfo
     * param config:  配置文件内容
     *
     * return  :
     */
    virtual int loadConfigByInfo(const ConfigInfo & configInfo, string &config,tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << configInfo.writeToJsonString() << endl;

        return 0;
    }

    /**
     *
     * @param configInfo ConfigInfo
     *
     * @return int
     */
    virtual int checkConfigByInfo(const ConfigInfo & configInfo, string &result,tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << configInfo.writeToJsonString() << endl;

        return 0;
    }

    /**
      * 获取服务的所有配置文件列表，
      * @param configInfo 支持拉取应用配置列表，服务配置列表，机器配置列表
      * @param[out] vf  获取到的文件名称列表
      * @return int 0: 成功, -1:失败
     **/
    virtual int ListAllConfigByInfo(const tars::GetConfigListInfo & configInfo, vector<std::string> &vf, tars::CurrentPtr current)
    {
    	LOG_CONSOLE_DEBUG << configInfo.writeToJsonString() << endl;

        return 0;
    }
private:
    map<string, string> files;
};

#endif /* TARS_TARS_TEST_TESTCODE_INCLUDE_STUB_CONFIGIMP_H_ */

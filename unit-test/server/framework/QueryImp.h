
#ifndef __QUERY_IMP_H__
#define __QUERY_IMP_H__

#include "servant/QueryF.h"
#include "DbHandle.h"

using namespace tars;

//////////////////////////////////////////////////////

enum FUNID
{
    FUNID_findObjectById              = 0,
    FUNID_findObjectById4Any          = 1,
    FUNID_findObjectById4All          = 2,
    FUNID_findObjectByIdInSameGroup   = 3,
    FUNID_findObjectByIdInSameStation = 4,
    FUNID_findObjectByIdInSameSet     = 5
};

//////////////////////////////////////////////////////
/**
 * 对象查询接口类
 */
class QueryImp: public QueryF
{
public:
    /**
     * 构造函数
     */
    QueryImp(){};

    /**
     * 初始化
     */
    virtual void initialize();

    /**
     ** 退出
     */
    virtual void destroy() {};

    /** 
     * 根据id获取所有该对象的活动endpoint列表
     */
    virtual vector<EndpointF> findObjectById(const string & id, tars::CurrentPtr current);

    /**
     * 根据id获取所有对象,包括活动和非活动对象
     */
    virtual tars::Int32 findObjectById4Any(const std::string & id, vector<tars::EndpointF> &activeEp, vector<tars::EndpointF> &inactiveEp, tars::CurrentPtr current);

    /** 
     * 根据id获取对象所有endpoint列表
     */
    Int32 findObjectById4All(const std::string & id, vector<tars::EndpointF> &activeEp, vector<tars::EndpointF> &inactiveEp, tars::CurrentPtr current);

    /** 
     * 根据id获取对象同组endpoint列表
     */
    Int32 findObjectByIdInSameGroup(const std::string & id, vector<tars::EndpointF> &activeEp, vector<tars::EndpointF> &inactiveEp, tars::CurrentPtr current);

    /** 
     * 根据id获取对象指定归属地的endpoint列表
     */
    Int32 findObjectByIdInSameStation(const std::string & id, const std::string & sStation, vector<tars::EndpointF> &activeEp, vector<tars::EndpointF> &inactiveEp, tars::CurrentPtr current);

    /** 
     * 根据id获取对象同set endpoint列表
     */
    Int32 findObjectByIdInSameSet(const std::string & id,const std::string & setId,vector<tars::EndpointF> &activeEp,vector<tars::EndpointF> &inactiveEp, tars::CurrentPtr current);

    /** 注册数据通知, 同时上报本地缓存数据的最后时间, 如果服务端发现变化则全量推送(企业版功能)
     * 可以定时上报数据
     * @param timestamp <数据类型, 最后数据时间戳>
     * @param name       当前模块名称
     * @return:  0-成功  others-失败
     */
    Int32 registerChange(const map<string, string> &timestamp, const string &name, CurrentPtr current) { return -1; };

    /** 注册id变化的通知, 通知时后需要自己主动find(企业版功能)
     *
     * @param id         对象名称
     * @param name       当前模块名称
     * @return:  0-成功  others-失败
     */
    Int32 registerQuery(const string &id, const string &name, CurrentPtr current) { return -1; };

    /**
     * 获取锁, 实现业务服务一主多备的模式(企业版功能)
     * @return 0: 获取锁成功； 1：获取锁失败； 2： 数据异常， -1：其他异常
     */
    Int32 getLocker(const tars::GetMasterSlaveLock &req, CurrentPtr current) { return -1;};


private:
    /**
     * 打印按天日志
     */
    void doDaylog(const FUNID eFnId,const string& id,const vector<tars::EndpointF> &activeEp, const vector<tars::EndpointF> &inactiveEp, const tars::CurrentPtr& current,const std::ostringstream& os,const string& sSetid="");

    /**
     * 转化成字符串
     */
    string eFunTostr(const FUNID eFnId);
protected:
    //数据库操作
    CDbHandle      _db;
};

#endif

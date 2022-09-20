
#ifndef __QUERY_IMP_H__
#define __QUERY_IMP_H__

#include "servant/QueryF.h"
#include "DbHandle.h"

using namespace tars;

//////////////////////////////////////////////////////

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
	 *
	 * @param current
	 * @return
	 */
	virtual int doClose(tars::CurrentPtr current);
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

	/**
	 *
	 * @param ids
	 * @param current
	 * @return
	 */
	Int32 registerQuery(const std::string & id, const string &name, CurrentPtr current);

	/**
	 *
	 * @param id
	 * @param current
	 * @return
	 */
	Int32 registerChange(const vector<std::string> & id, const string &name, CurrentPtr current);

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

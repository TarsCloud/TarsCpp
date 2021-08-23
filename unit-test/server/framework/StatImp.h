

#ifndef __STAT_IMP_H_
#define __STAT_IMP_H_

#include <functional>
#include "util/tc_common.h"
#include "util/tc_thread.h"
#include "servant/RemoteLogger.h"
#include "servant/StatF.h"

using namespace tars;

class StatImp : public StatF
{
public:
    /**
     *
     */
    StatImp()
    {
    }

    ~StatImp()
    {
    }

    /**
     * 初始化
     *
     * @return int
     */
    virtual void initialize() {};

    /**
     * 退出
     */
    virtual void destroy()
    {
    }

    /**
     * 上报模块间调用信息
     * @param statmsg, 上报信息
     * @return int, 返回0表示成功
     */
    virtual int reportMicMsg( const map<tars::StatMicMsgHead, tars::StatMicMsgBody>& statmsg, bool bFromClient, tars::CurrentPtr current );
    
    /**
     * 上报模块间调用采样信息
     * @param sample, 上报信息
     * @return int, 返回0表示成功
     */
    virtual int reportSampleMsg(const vector<StatSampleMsg> &msg,tars::CurrentPtr current );

};

#endif



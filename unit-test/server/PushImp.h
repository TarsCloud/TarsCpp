#ifndef _PushImp_H_
#define _PushImp_H_

#include "servant/Application.h"
//#include "TestPushServant.h"

/**
 *
 *
 */
class PushImp : public  tars::Servant
{
public:
    /**
     *
     */
    virtual ~PushImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    //重载Servant的doRequest方法
    int doRequest(tars::CurrentPtr current, vector<char>& response);

    //重载Servant的doClose方法
    int doClose(tars::CurrentPtr current);

};
/////////////////////////////////////////////////////
#endif

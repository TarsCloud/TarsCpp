#ifndef _CustomServantImp_H_
#define _CustomServantImp_H_

#include "servant/Application.h"
//#include "CustomServant.h"

/**
 *
 *
 */
class CustomServantImp : public  tars::Servant
{
public:
    /**
     *
     */
    virtual ~CustomServantImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    //overloading method of Servant::doRequest
    int doRequest(tars::TarsCurrentPtr current, vector<char>& response);

    //overloading method of Servant::doClose
    int doClose(tars::TarsCurrentPtr current);

};
/////////////////////////////////////////////////////
#endif

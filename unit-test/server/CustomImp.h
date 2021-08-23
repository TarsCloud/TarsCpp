#ifndef _CustomServantImp_H_
#define _CustomServantImp_H_

#include "servant/Application.h"
//#include "CustomServant.h"

/**
 *
 *
 */
class CustomImp : public  tars::Servant
{
public:
    /**
     *
     */
    virtual ~CustomImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    //overloading method of Servant::doRequest
    int doRequest(tars::CurrentPtr current, vector<char>& response);

    //overloading method of Servant::doClose
    int doClose(tars::CurrentPtr current);

};
/////////////////////////////////////////////////////
#endif

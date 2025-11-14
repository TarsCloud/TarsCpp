#ifndef _SipProxyDb_H_
#define _SipProxyDb_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class SipProxyDb : public Application
{
public:
    /**
     *
     **/
    virtual ~SipProxyDb() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern SipProxyDb g_app;

////////////////////////////////////////////
#endif

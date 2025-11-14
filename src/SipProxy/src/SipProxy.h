#ifndef _SipProxy_H_
#define _SipProxy_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class SipProxy : public Application
{
public:
    /**
     *
     **/
    virtual ~SipProxy() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern SipProxy g_app;

////////////////////////////////////////////
#endif

#ifndef _SipGateway_H_
#define _SipGateway_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class SipGateway : public Application
{
public:
    /**
     *
     **/
    virtual ~SipGateway() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern SipGateway g_app;

////////////////////////////////////////////
#endif

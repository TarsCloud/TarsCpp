#ifndef _SipRegistry_H_
#define _SipRegistry_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class SipRegistry : public Application
{
public:
    /**
     *
     **/
    virtual ~SipRegistry() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern SipRegistry g_app;

////////////////////////////////////////////
#endif

#ifndef _SipCallRelay_H_
#define _SipCallRelay_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class SipCallRelay : public Application
{
public:
    /**
     *
     **/
    virtual ~SipCallRelay() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern SipCallRelay g_app;

////////////////////////////////////////////
#endif

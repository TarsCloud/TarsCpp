#ifndef _SipCallController_H_
#define _SipCallController_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class SipCallController : public Application
{
public:
    /**
     *
     **/
    virtual ~SipCallController() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern SipCallController g_app;

////////////////////////////////////////////
#endif

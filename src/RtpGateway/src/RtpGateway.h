#ifndef _RtpGateway_H_
#define _RtpGateway_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class RtpGateway : public Application
{
public:
    /**
     *
     **/
    virtual ~RtpGateway() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern RtpGateway g_app;

////////////////////////////////////////////
#endif

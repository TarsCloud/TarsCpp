#ifndef _ENUMProxy_H_
#define _ENUMProxy_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class ENUMProxy : public Application
{
public:
    /**
     *
     **/
    virtual ~ENUMProxy() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern ENUMProxy g_app;

////////////////////////////////////////////
#endif

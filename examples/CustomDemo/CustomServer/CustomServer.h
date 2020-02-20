#ifndef _CustomServer_H_
#define _CustomServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class CustomServer : public Application
{
public:
    /**
     *
     **/
    virtual ~CustomServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();

};

extern CustomServer g_app;

////////////////////////////////////////////
#endif

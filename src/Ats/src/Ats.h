#ifndef _Ats_H_
#define _Ats_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class Ats : public Application
{
public:
    /**
     *
     **/
    virtual ~Ats() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern Ats g_app;

////////////////////////////////////////////
#endif

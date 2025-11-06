#ifndef _SipRouterImp_H_
#define _SipRouterImp_H_

#include "servant/Application.h"
#include "SipRouter.h"

/**
 *
 *
 */
class SipRouterImp : public VoipApp::SipRouter
{
public:
    /**
     *
     */
    virtual ~SipRouterImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};
};
/////////////////////////////////////////////////////
#endif

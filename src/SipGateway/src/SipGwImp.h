#ifndef _SipGwImp_H_
#define _SipGwImp_H_

#include "servant/Application.h"
#include "SipGw.h"

/**
 *
 *
 */
class SipGwImp : public VoipApp::SipGw
{
public:
    /**
     *
     */
    virtual ~SipGwImp() {}

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

#ifndef _SipPrxImp_H_
#define _SipPrxImp_H_

#include "servant/Application.h"
#include "SipPrx.h"

/**
 *
 *
 */
class SipPrxImp : public VoipApp::SipPrx
{
public:
    /**
     *
     */
    virtual ~SipPrxImp() {}

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

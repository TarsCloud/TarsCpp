#ifndef _SipDbImp_H_
#define _SipDbImp_H_

#include "servant/Application.h"
#include "SipDb.h"

/**
 *
 *
 */
class SipDbImp : public VoipApp::SipDb
{
public:
    /**
     *
     */
    virtual ~SipDbImp() {}

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

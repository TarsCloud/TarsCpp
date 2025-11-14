#ifndef __SipMsgSenderInterface_h
#define __SipMsgSenderInterface_h

#include "SipGatewayCommon.h"
#include "SipDialog.h"


namespace SipGateway
{
    class SipMsgSenderInterface
    {
    public:

        virtual bool sendEncodedMessage(const std::string & strMsg, int iSize, bool bRequest, SipGateway::SipRequestMethodType reqMethod, const SipDialogPtr & dlg) =0;

    };

    typedef std::shared_ptr<SipMsgSenderInterface> SipMsgSenderInterfacePtr;
};

#endif //__SipMsgSenderInterface_h
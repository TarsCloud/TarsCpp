#ifndef  _SipPrxoy_Msg_Sender_interaface
#define  _SipPrxoy_Msg_Sender_interaface


#include "SipRegisterSubscribe.h"
#include "SipGateway/SipGatewayCommon.h"


namespace SipProxy
{
    class SipProxyMsgSenderInterafece : virtual public Common::Shared
    {
    public:
        virtual bool sendEncodedMessage(const Common::String & strMsg, int iSize,  const Common::String & connectionId, Common::String & reason) =0;


    };

    typedef Common::Handle<SipProxyMsgSenderInterafece> SipProxyMsgSenderInterafecePtr;
};

#endif





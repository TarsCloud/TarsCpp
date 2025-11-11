#ifndef _RtpGatewayMsgSenderInterface_H_
#define _RtpGatewayMsgSenderInterface_H_

#include <string>
#include <memory>

namespace RtpGateway
{
    class RtpGatewayMsgSenderInterface
    {

    public:
        virtual bool receivedSBCRtpMsg(const std::string & strMsg, int size, bool bAudio, void * pConnection) = 0;

    };

    typedef std::shared_ptr<RtpGatewayMsgSenderInterface> RtpGatewayMsgSenderInterfacePtr;

};






#endif











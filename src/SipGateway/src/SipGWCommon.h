#ifndef __SIP_GW_Common_h__
#define __SIP_GW_Common_h__


namespace SipProxy
{
    // --------------------  CONST Var  --------------------
    static const int			SIP_GW_SINGLE_MESSAGE_MAX_LENGTH				= 2 * 1024 * 1024;

    // --------------------  App Config Item Key & Default Value --------------------
    static const std::string	SIP_GW_CONFIG_ITEM_NAME_SIP_TRANSPORT_PROTOCOL	= "SipTransport";
    static const std::string	SIP_GW_CONFIG_ITEM_VALUE_SIP_TRANSPORT_PROTOCOL	= "udp";

    static const std::string	SIP_GW_CONFIG_ITEM_NAME_SIP_HOST				= "SipHost";
    static const std::string	SIP_GW_CONFIG_ITEM_VALUE_SIP_HOST				= "127.0.0.1";

    static const std::string	SIP_GW_CONFIG_ITEM_NAME_SIP_PORT				= "SipPort";
    static const int			SIP_GW_CONFIG_ITEM_VALUE_SIP_PORT				= 5060;

    static const std::string	SIP_GW_CONFIG_ITEM_NAME_SIP_MAX_CONNECT			= "SipMaxConnect";
    static const int			SIP_GW_CONFIG_ITEM_VALUE_SIP_MAX_CONNECT		= 1024;
};

#endif //__SIP_GW_Common_h__
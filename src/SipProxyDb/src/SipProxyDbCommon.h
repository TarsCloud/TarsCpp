#ifndef _SipProxyDB_Common_h_
#define  _SipProxyDB_Common_h_

#include <string>

namespace SipProxyDbCommon
{
    typedef enum SipTableType
    {
        SipTableAccout,
        SipTableRegInfo,
        SipTableSubInfo,
        SipTableDomain,
        SipTableSipProxyConfig,
        SipTableRtpGatewayConfig,
        SipCallStatistic,
        SipTableNationsIp,
        SipTableCallSipPolicy,
        SipTableDomainIM,
        SipTableSipProxyConfigIM,
        SipTableIpWhiteList,
        SipTableNumberRouting,
        SipTableCallRecord
    } SipTableType;

    //service name
    static const std::string SIP_PROXY_DB_SERVICE_NAME = "SipProxyDb";

    //database name
    static const std::string SIP_PROXY_DB_DATABASE_NAME = "db_sipproxy";

    //table name
    static const std::string SIP_TABLE_ACCOUNT = "sip_account";
    static const std::string SIP_TABLE_REGISTER_INFO = "sip_reginfo";
    static const std::string SIP_TABLE_SUBSCRIBE_INFO = "sip_subinfo";
    static const std::string SIP_TABLE_DOMAIN = "domain_config";
    static const std::string SIP_TABLE_PROXY_CONFIG = "sipproxy_config";
    static const std::string SIP_TABLE_RTPGW_CONFIG = "rtpgateway_config";
    static const std::string SIP_TABLE_CALL_STATISTIC = "sip_record";
    static const std::string SIP_TABLE_NATIONS_IP = "nations_code_ip";
    static const std::string SIP_TABLE_ACCESS_IMS_POLICY = "call_sip_policy";
    static const std::string SIP_TABLE_DOMAIN_IM = "domain_im_config";
    static const std::string SIP_TABLE_PROXY_IM_CONFIG = "messagegw_im_config";

    //table name
    static const std::string SIP_TABLE_ACCOUNT_2 = "account";
    static const std::string SIP_TABLE_REGISTER_INFO_2 = "reginfo";
    static const std::string SIP_TABLE_SUBSCRIBE_INFO_2 = "subinfo";
    static const std::string SIP_TABLE_DOMAIN_2 = "domain_setting";
    static const std::string SIP_TABLE_PROXY_CONFIG_2 = "sipproxy_setting";
    static const std::string SIP_TABLE_RTPGW_CONFIG_2 = "rtpgateway_setting";
    static const std::string SIP_TABLE_CALL_STATISTIC_2 = "record";
    static const std::string SIP_TABLE_NATIONS_IP_2 = "nations_ip";
    static const std::string SIP_TABLE_ACCESS_IMS_POLICY_2 = "call_policy";
    static const std::string SIP_TABLE_DOMAIN_IM_2 = "domain_im_setting";
    static const std::string SIP_TABLE_PROXY_IM_CONFIG_2 = "messagegw_im_setting";
    static const std::string SIP_TABLE_PROXY_IP_WHITE_LIST = "ip_whitelist";
    static const std::string SIP_TABLE_NUMBER_ROUTING = "number_routing";
    static const std::string SIP_TABLE_SIP_CALL_RECORD = "t_sip_record";
};



#endif



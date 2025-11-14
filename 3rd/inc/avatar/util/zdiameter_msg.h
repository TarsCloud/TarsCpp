#ifndef _ZDIAMETER_MSG_H__
#define _ZDIAMETER_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct msg ZDIAMETER_MSG;               /**< @brief Type of msg. */
typedef struct avp ZDIAMETER_AVP;               /**< @brief Type of avp. */
//typedef struct dictionary ZDIAMETER_DICT;       /**< @brief Type of dictionary. */
typedef struct dict_object ZDIAMETER_DICT_OBJ;  /**< @brief Type of dict_object. */
typedef union avp_value ZDIAMETER_AVP_VALUE;    /**< @brief Type of avp_value. */
typedef struct dict_avp_request ZDIAMETER_AVP_REQ;  /**< @brief Type of dict_avp_request. */

#define ZDIAMETER_CHECK(_ret, _val, _msg)                               \
{				                                                        \
    if (_ret != _val)                                                   \
    {                                                                   \
        ZOS_LOG_ERROR(ZOS_LOGID, "%s, %s", __FUNCTION__, _msg);         \
        return ZFAILED;                                                 \
    }                                                                   \
}

#define ZDIAMETER_CHECK_NORETURN(_ret, _val, _msg)                               \
{				                                                        \
    if (_ret != _val)                                                   \
    {                                                                   \
        ZOS_LOG_INFO(ZOS_LOGID, "%s, %s", __FUNCTION__, _msg);         \
    }                                                                   \
}

/** @brief diameter msg type */
typedef enum EN_DIAMETER_MSG_TYPE
{
    EN_DIAMETER_MSG_CER,             /**< @brief CER message */
    EN_DIAMETER_MSG_CEA,             /**< @brief CEA message */
    EN_DIAMETER_MSG_DPR,             /**< @brief DPR message */
    EN_DIAMETER_MSG_DPA,             /**< @brief DPA message */
    EN_DIAMETER_MSG_DWR,             /**< @brief DWR message */
    EN_DIAMETER_MSG_DWA,             /**< @brief DWA message */
    EN_DIAMETER_MSG_ASR,             /**< @brief ASR message */
    EN_DIAMETER_MSG_ASA,             /**< @brief ASA message */
    EN_DIAMETER_MSG_CCR,             /**< @brief CCR message */
    EN_DIAMETER_MSG_CCA,             /**< @brief CCA message */
} EN_DIAMETER_MSG_TYPE;

/* Vendor-Specific-Application-Id avp value info */
typedef struct tagVSAI_AVP_VALUE
{
    ZUINT iVendorId;                     /**< @brief Vendor-Id value */
    ZBOOL bAuthFlag;                     /**< @brief Auth-Application-Id flag */
    ZUINT iAuthAppId;                    /**< @brief Auth-Application-Id value */
    ZBOOL bAcctFlag;                     /**< @brief Acct-Application-Id flag */
    ZUINT iAcctAppId;                    /**< @brief Acct-Application-Id value */
}ST_VSAI_AVP_VALUE;

/* Proxy-Info avp value info */
typedef struct tagPI_AVP_VALUE
{
    ZCHAR *pcProxyHost;                  /**< @brief Proxy-Host value */
	ZCHAR *pcProxyState;                 /**< @brief Proxy-State value */
}ST_PI_AVP_VALUE;

/* CER avp value info */
typedef struct tagCER_AVP_VALUE
{
    ZCHAR *pcOriHost;                    /**< @brief Origin-Host value */
    ZCHAR *pcOriRealm;                   /**< @brief Origin-Realm value */
    ZCHAR *pcHostIPAddr;                 /**< @brief Host-IP-Address value */
    ZUINT iVendorId;                     /**< @brief Vendor-Id value */
    ZCHAR *pcProductName;                /**< @brief Product-Name value */
    ZUINT iOriStateId;                   /**< @brief Origin-State-Id value */
    ZUINT iSupVendorId;                  /**< @brief Supported-Vendor-Id value */
    ZUINT iAuthAppId;                    /**< @brief Auth-Application-Id value */
    ZUINT iInbandSecId ;                 /**< @brief Inband-Security-Id value */
    ZUINT iAcctAppId;                    /**< @brief Acct-Application-Id value */
    ST_VSAI_AVP_VALUE stVSAIAvpVal;      /**< @brief Vendor-Specific-Application-Id value */
    ZUINT iFirmRevision;                 /**< @brief Firmware-Revision value */
}ST_CER_AVP_VALUE;

/* DPR avp value info */
typedef struct tagDPR_AVP_VALUE
{
    ZCHAR *pcOriHost;                    /**< @brief Origin-Host value */
    ZCHAR *pcOriRealm;                   /**< @brief Origin-Realm value */
    ZINT iDisconCause;                   /**< @brief Disconnect-Cause value */
}ST_DPR_AVP_VALUE;

/* DWR avp value info */
typedef struct tagDWR_AVP_VALUE
{
    ZCHAR *pcOriHost;                    /**< @brief Origin-Host value */
    ZCHAR *pcOriRealm;                   /**< @brief Origin-Realm value */
    ZUINT iOriStateId;                   /**< @brief Origin-State-Id value */
}ST_DWR_AVP_VALUE;

/* ASR avp value info */
typedef struct tagASR_AVP_VALUE
{
    ZCHAR *pcSessId;                     /**< @brief Session-Id value */
    ZCHAR *pcOriHost;                    /**< @brief Origin-Host value */
    ZCHAR *pcOriRealm;                   /**< @brief Origin-Realm value */
    ZCHAR *pcDestRealm;                  /**< @brief Destination-Realm value */
    ZCHAR *pcDestHost;                   /**< @brief Destination-Host value */
    ZUINT iAuthAppId;                    /**< @brief Auth-Application-Id value */
    ZCHAR *pcUserName;                   /**< @brief User-Name value */
    ZUINT iOriStateId;                   /**< @brief Origin-State-Id value */
    ST_PI_AVP_VALUE stProxyInfo;         /**< @brief Proxy-Info value */
    ZCHAR *pcRouteRecord;                /**< @brief Route-Record value */
}ST_ASR_AVP_VALUE;

/* Subscription-Id avp value info */
typedef struct tagSUBID_AVP_VALUE
{
    ZINT iSubIdType;                     /**< @brief Subscription-Id-Type value */
	ZCHAR *pcSubIdData;                  /**< @brief Subscription-Id-Data value */
}ST_SUBID_AVP_VALUE;

/* IMS-Information avp value info */
typedef struct tagIMSINFO_AVP_VALUE
{
    ZINT iRoleofNode;                    /**< @brief Role-of-Node avp value */
    ZCHAR *pcCallingPAddr;               /**< @brief Calling-Party-Address avp value */
    ZCHAR *pcCalledPAddr;                /**< @brief Called-Party-Address avp value */
    ZCHAR *pcIMSCharId;                  /**< @brief IMS-Charging-Identifier avp value */
    ZCHAR *pcOrigCallingPAddr;           /**< @brief Original-Calling-Party-Address avp value */
    ZCHAR *pcOrigCalledPAddr;            /**< @brief Original-Called-Party-Address avp value */
    ZCHAR *pcMiddlePAddr;                /**< @brief Middle-Party-Address avp value */
    ZCHAR *pcMeetingID;                  /**< @brief meetingID avp value */
    ZCHAR *pcRouteId;                    /**< @brief Route-Id avp value */
    ZCHAR *pcAtomId;                     /**< @brief Atom-Id avp value */
    ZCHAR *pcPriceNum;                   /**< @brief PRICE_NUMBER avp value */
    ZUINT iNumberTypeFlag;               /**< @brief NUMBER_TYPE_FLAG avp value */
    ZUINT iRingTime;                     /**< @brief Ring-Time avp value */
    ZUINT iFinishType;                   /**< @brief Finish-Type avp value */
    ZCHAR *pcReqPAddr;                   /**< @brief Requested-Party-Address avp value */
    ZCHAR *pcCallingVNum;                /**< @brief Calling-Vlr-Number avp value */
    ZCHAR *pcCallingCOSAI;               /**< @brief Calling-CellID-Or-SAI avp value */
    ZCHAR *pcCalledVNum;                 /**< @brief Called-Vlr-Number avp value */
    ZCHAR *pcCalledCOSAI;                /**< @brief Called-CellID-Or-SAI avp value */
	
}ST_IMSINFO_AVP_VALUE;

/* Service-Information avp value info */
typedef struct tagSRVINFO_AVP_VALUE
{
    ST_IMSINFO_AVP_VALUE stImsInfo;       /**< @brief IMS-Information avp */
	
}ST_SRVINFO_AVP_VALUE;

/* Requested-Service-Unit avp value info */
typedef struct tagRSU_AVP_VALUE
{
    ZUINT iCCTime;                       /**< @brief CC-Time value */	
}ST_RSU_AVP_VALUE;

/* Used-Service-Unit avp value info */
typedef struct tagUSU_AVP_VALUE
{
    ZUINT iCCTime;                       /**< @brief CC-Time value */	
}ST_USU_AVP_VALUE;

/* Granted-Service-Unit avp value info */
typedef struct tagGSU_AVP_VALUE
{
    ZUINT iCCTime;                       /**< @brief CC-Time value */
}ST_GSU_AVP_VALUE;

/* Multiple-Services-Credit-Control avp value info */
typedef struct tagMSCC_AVP_VALUE
{
    ST_RSU_AVP_VALUE stReqSerUnit;       /**< @brief Requested-Service-Unit avp */
    ZBOOL bRSUFlag;                      /**< @brief have Requested-Service-Unit avp flag */ 
    ST_USU_AVP_VALUE stUseSerUnit;       /**< @brief Used-Service-Unit avp */
    ZBOOL bUSUFlag;                      /**< @brief have Used-Service-Unit avp flag */
    ST_GSU_AVP_VALUE stGrtSerUnit;       /**< @brief Granted-Service-Unit avp */
    ZBOOL bGSUFlag;                      /**< @brief have Granted-Service-Unit avp flag */
}ST_MSCC_AVP_VALUE;

/* CCR avp value info */
typedef struct tagCCR_AVP_VALUE
{
    ZCHAR *pcSessId;                     /**< @brief Session-Id value */
    ZCHAR *pcOriHost;                    /**< @brief Origin-Host value */
    ZCHAR *pcOriRealm;                   /**< @brief Origin-Realm value */
    ZCHAR *pcDestRealm;                  /**< @brief Destination-Realm value */
    ZCHAR *pcDestHost;                   /**< @brief Destination-Host value */
    ZUINT iAuthAppId;                    /**< @brief Auth-Application-Id value */
    ZCHAR *pcSerConId;                   /**< @brief Service-Context-Id value */
    ZINT iCCReqType;                     /**< @brief CC-Request-Type value */
    ZINT iCCReqNum;                      /**< @brief CC-Request-Number value */
    ZLONG lEvtTimestamp;                 /**< @brief Event-Timestamp value */
    ST_SUBID_AVP_VALUE stSubId;          /**< @brief Subscription-Id value */
    ST_SRVINFO_AVP_VALUE stSrvInfo;      /**< @brief Service-Information value */
    ZUINT iServiceId;                    /**< @brief Service-Identifier value */
    ZINT iMulSerId;                      /**< @brief Multiple-Services-Indicator value */
    ST_MSCC_AVP_VALUE stMulSrvCreCtl;    /**< @brief Multiple-Services-Credit-Control value */
}ST_CCR_AVP_VALUE;

/* diameter init dictionary */
ZFUNC ZVOID ZDiameter_MsgInitDict(ZDIAMETER_DICT **pstDict);

/* diameter free dictionary */
ZFUNC ZVOID ZDiameter_MsgFreeDict(ZDIAMETER_DICT **pstDict);

/* diameter message create */
ZFUNC ZINT ZDiameter_MsgCreate(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG **ppstMsg, ZINT iMsgType);

/* diameter delete message */
ZFUNC ZINT ZDiameter_MsgDelete(ZDIAMETER_MSG *pstMsg);

/* diameter add Origin-Host avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_OH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Origin-Realm avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_OR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Host-IP-Address avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_HIA(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue, ZBOOL bIpv4);

/* diameter add Vendor-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_VI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Product-Name avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_PN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Origin-State-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_OSI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Supported-Vendor-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_SVI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Auth-Application-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_AuAI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Inband-Security-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_ISI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Acct-Application-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_AcAI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Firmware-Revision avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_FR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Vendor-Specific-Application-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_VSAI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, 
                        ZUINT iVendorId, ZBOOL bAuthFlag, ZUINT iAuthId,
                        ZBOOL bAcctFlag, ZUINT iAcctId);
                        
/* diameter add Result-Code avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_RC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Error-Message avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_EM(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Disconnect-Cause avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_DC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT iValue);

/* diameter add Session-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_SI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Destination-Realm avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_DR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Destination-Host avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_DH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add User-Name avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_UN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Route-Record avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_RR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Proxy-Info avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_PI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, 
                    ZCHAR *pcProxyHost, ZCHAR *pcProxyState);

/* diameter add Error-Reporting-Host avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_ERH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Redirect-Host avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_RH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add Redirect-Host-Usage avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_RHU(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT iValue);

/* diameter add Redirect-Max-Cache-Time avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_RMCT(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Service-Context-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_SCI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZCHAR *pcValue);

/* diameter add CC-Request-Type avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_CCRT(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add CC-Request-Number avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_CCRN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add CC-Sub-Session-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_CCSSI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZULONG dwValue);

/* diameter add Event-Timestamp avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_ET(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZLONG iValue);

/* diameter add Subscription-Id avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_SubId(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, 
                    ZINT iType, ZCHAR *pcData);

/* diameter add Service-Identifier avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_ServiceId(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT iValue);

/* diameter add Requested-Service-Unit avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_RSU(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUINT iTime);

/* diameter add Used-Service-Unit avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_USU(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUINT iTime);

/* diameter add Granted-Service-Unit avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_GSU(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUINT iTime);

/* diameter add Role-of-Node avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_RON(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZINT iValue);

/* diameter add Calling-Party-Address avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_CINGPA(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Called-Party-Address avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_CEDPA(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add IMS-Charging-Identifier avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_ICI(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Original-Calling-Party-Address avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_OCPA(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Original-Called-Party-Address avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_OCDPA(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Middle-Party-Address avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_MPA(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add meetingID avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_MID(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Ring-Time avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_RT(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUINT iValue);

/* diameter add Finish-Type avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_FT(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUINT iValue);

/* diameter add Atom-Id avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_RID(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Atom-Id avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_AID(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add PRICE_NUMBER avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_PNUM(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add NUMBER_TYPE_FLAG avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_NTF(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUINT iValue);

/* diameter add Requested-Party-Address avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_RPA(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Calling-Vlr-Number avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_CINGVN(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Calling-CellID-Or-SAI avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_CINGCOS(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Called-Vlr-Number avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_CEDVN(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add Called-CellID-Or-SAI avp in the msg or group avp */
ZFUNC ZINT ZDiameter_MsgAddAvp_CEDCOS(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ZUCHAR *pcValue);

/* diameter add IMS-Information avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_IMSINFO(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ST_IMSINFO_AVP_VALUE *pstIMSInfo);

/* diameter add Service-Information avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_SRVINFO(ZDIAMETER_DICT *pstDict, ZVOID *pstData, ZBOOL bAddMsg, ST_SRVINFO_AVP_VALUE *pstSRVInfo);

/* diameter add Multiple-Services-Indicator avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_MSI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT iValue);

/* diameter add Termination-Cause avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_TC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT iValue);

/* diameter add Multiple-Services-Credit-Control avp in the msg */
ZFUNC ZINT ZDiameter_MsgAddAvp_MSCC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ST_MSCC_AVP_VALUE *pstMulSrvCreCtl);

/* diameter get msg type */
ZFUNC ZINT ZDiameter_MsgGetMsgType(ZDIAMETER_MSG *pstMsg);

/* diameter create a CER message with avp */
ZFUNC ZINT ZDiameter_MsgCreateCER(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG **ppstMsg, 
                    ST_CER_AVP_VALUE *pstCERAvpVal, ZBOOL bIpv4);

/* diameter create a DPR message with avp */
ZFUNC ZINT ZDiameter_MsgCreateDPR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG **ppstMsg, 
                    ST_DPR_AVP_VALUE *pstDPRAvpVal);

/* diameter create a DWR message with avp */
ZFUNC ZINT ZDiameter_MsgCreateDWR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG **ppstMsg, 
                    ST_DWR_AVP_VALUE *pstDWRAvpVal);

/* diameter create a ASR message with avp */
ZFUNC ZINT ZDiameter_MsgCreateASR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG **ppstMsg, 
                    ST_ASR_AVP_VALUE *pstASRAvpVal);

/* diameter create a CCR message with avp */
ZFUNC ZINT ZDiameter_MsgCreateCCR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG **ppstMsg, 
                    ST_CCR_AVP_VALUE *pstCCRAvpVal);

/* diameter encode msg to buffer */
ZFUNC ZINT ZDiameter_MsgEncodeMsgToBuf(ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcBuf, ZSIZE_T *piLen);

/* diameter decode buffer to msg */
ZFUNC ZINT ZDiameter_MsgDecodeBufToMsg(ZUCHAR **ppcBuf, ZSIZE_T iLen, ZDIAMETER_MSG **ppstMsg);

/* diameter get Origin-Host avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_OH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Origin-Realm avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_OR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Host-IP-Address avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_HIA(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Vendor-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_VI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Product-Name avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_PN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Origin-State-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_OSI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Supported-Vendor-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_SVI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Auth-Application-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_AuAI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Inband-Security-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_ISI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Acct-Application-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_AcAI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Firmware-Revision avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_FR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Vendor-Specific-Application-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_VSAI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, 
                        ZUINT *piVendorId, ZUINT *piAuthId, ZUINT *piAcctId);

/* diameter get Result-Code avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_RC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Error-Message avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_EM(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Disconnect-Cause avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_DC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT *piVal);

/* diameter get Session-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_SI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Destination-Realm avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_DR(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Destination-Host avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_DH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get User-Name avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_UN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get User-Name avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_UN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Proxy-Info avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_PI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg,
                        ZUCHAR **ppcHost, ZUCHAR **ppcState);

/* diameter get Error-Reporting-Host avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_ERH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Redirect-Host avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_RH(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get Redirect-Host-Usage avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_RHU(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT *piVal);

/* diameter get Redirect-Max-Cache-Time avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_RMCT(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Service-Context-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_SCI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUCHAR **ppcVal);

/* diameter get CC-Request-Type avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_CCRT(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT *piVal);

/* diameter get CC-Request-Number avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_CCRN(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get CC-Sub-Session-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_CCSSI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZULONG *pdwVal);

/* diameter get Event-Timestamp avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_ET(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZLONG *piVal);

/* diameter get Subscription-Id avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_SubId(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg,
                        ZINT *piType, ZUCHAR **ppcData);

/* diameter get Service-Identifier avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_ServiceId(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piVal);

/* diameter get Requested-Service-Unit avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_RSU(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZUINT *piTime);

/* diameter get Multiple-Services-Indicator avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_MSI(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT *piVal);

/* diameter get Termination-Cause avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_TC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ZINT *piVal);

/* diameter get Multiple-Services-Credit-Control avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_MSCC(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ST_MSCC_AVP_VALUE *pstMulSrvCreCtl);

/* diameter get Service-Information avp_value from a msg */
ZFUNC ZINT ZDiameter_MsgGetAvpValue_SRVINFO(ZDIAMETER_DICT *pstDict, ZDIAMETER_MSG *pstMsg, ST_SRVINFO_AVP_VALUE *pstSRVInfo);

/* diameter set msg end-to-end id */
ZFUNC ZINT ZDiameter_MsgSetEteId(ZDIAMETER_MSG **ppstMsg);

/* diameter set msg hop-by-hop id */
ZFUNC ZINT ZDiameter_MsgSetHbhId(ZDIAMETER_MSG **ppstMsg);

/* diameter check msg valid */
ZFUNC ZINT ZDiameter_MsgValid(ZDIAMETER_MSG *pstMsg);

#ifdef __cplusplus
}
#endif

#endif


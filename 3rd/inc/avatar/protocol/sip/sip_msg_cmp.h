/************************************************************************

        Copyright (c) 2005-2007 by Juphoon System Software, Inc.
                       All rights reserved.

     This software is confidential and proprietary to Juphoon System,
     Inc. No part of this software may be reproduced, stored, transmitted,
     disclosed or used in any form or by any means other than as expressly
     provided by the written license agreement between Juphoon and its
     licensee.

     THIS SOFTWARE IS PROVIDED BY JUPHOON "AS IS" AND ANY EXPRESS OR 
     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL JUPHOON BE LIABLE FOR ANY DIRECT, 
     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
     OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
     POSSIBILITY OF SUCH DAMAGE. 

                    Juphoon System Software, Inc.
                    Email: support@juphoon.com
                    Web: http://www.juphoon.com

************************************************************************/
/*************************************************
  File name     : sip_msg_cmp.h
  Module        : sip protocol service message compare
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_MSG_CMP_H__
#define _SIP_MSG_CMP_H__

/** 
 * @file
 * @brief Interfaces for SIP message compare.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SIP message compare check */
#define SIP_CMP_FUNC_CHK(_parm1, _parm2) \
    if (!_parm1 || !_parm2) return ZFAILED

/** @brief SIP message compare check present */
#define SIP_CMP_FUNC_PRE_CHK(_parm1, _parm2) do { \
    if (!_parm1 || !_parm2 || (_parm1)->ucPres != (_parm2)->ucPres) \
        return ZFAILED; \
    if (!(_parm1)->ucPres) return ZOK; \
} while (0)

/** @brief SIP message compare structure string */
#define SIP_CMP_SSTR(_sstr1, _sstr2) do { \
    if (ZOS_SSTR_CMPX(_sstr1, _sstr2) != ZOK) \
        return ZFAILED; \
} while (0)

/**
 * @brief Compare From or To header.
 *
 * @param [in] pstParm1 First header.
 * @param [in] pstParm2 Second header.
 *
 * @retval ZOK The 2 headers are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpAddrSpecX
 */
ZFUNC ZINT Sip_CmpHdrFromTo(ST_SIP_HDR_FROM_TO *pstParm1,
                ST_SIP_HDR_FROM_TO *pstParm2);

/**
 * @brief Compare "CSeq" header.
 *
 * @param [in] pstParm1 First "CSeq" header.
 * @param [in] pstParm2 Second "CSeq" header.
 *
 * @retval ZOK The 2 header CSeq's stMethod and iCseqVal are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpMethod 
 */
ZFUNC ZINT Sip_CmpHdrCseq(ST_SIP_HDR_CSEQ *pstParm1,
                ST_SIP_HDR_CSEQ *pstParm2);

/**
 * @brief Compare "Call-ID" header.
 *
 * @param [in] pstParm1 First "Call-ID" header.
 * @param [in] pstParm2 Second "Call-ID" header.
 *
 * @retval ZOK The 2 "Call-ID" header are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpCallId
 */
ZFUNC ZINT Sip_CmpHdrCallId(ST_SIP_HDR_CALL_ID *pstParm1,
                ST_SIP_HDR_CALL_ID *pstParm2);

/**
 * @brief Compare "Contact" header.
 *
 * @param [in] pstParm1 First "Contact" header.
 * @param [in] pstParm2 Second "Contact" header.
 *
 * @retval ZOK The 2 "Contact" header are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpContactParm 
 */
ZFUNC ZINT Sip_CmpHdrContact(ST_SIP_HDR_CONTACT *pstParm1,
                ST_SIP_HDR_CONTACT *pstParm2);

/**
 * @brief Compare "Route" header.
 *
 * @param [in] pstParm1 First "Route" header.
 * @param [in] pstParm2 Second "Route" header.
 *
 * @retval ZOK The 2 "Route" header are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpRouteParm 
 */
ZFUNC ZINT Sip_CmpHdrRoute(ST_SIP_HDR_ROUTE *pstParm1,
                ST_SIP_HDR_ROUTE *pstParm2);
/**  
 * @brief Compare "Via" header.
 *
 * @param [in] pstParm1 First "Via" header.
 * @param [in] pstParm2 Second "Via" header.
 *
 * @retval ZOK The 2 "Via" header are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpViaParm 
 */
ZFUNC ZINT Sip_CmpHdrVia(ST_SIP_HDR_VIA *pstParm1,
                ST_SIP_HDR_VIA *pstParm2);

/**
 * @brief Compare userinfo.
 *
 * @param [in] pstParm1 First userinfo.
 * @param [in] pstParm2 Second userinfo.
 *
 * @retval ZOK The ucPres,ucPasswdPres,stPasswd of 2 userinfo are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpUserInfo(ST_SIP_USER_INFO *pstParm1,
                ST_SIP_USER_INFO *pstParm2);

/**
 * @brief Compare host.
 *
 * @param [in] pstParm1 First host.
 * @param [in] pstParm2 Second host.
 *
 * @retval ZOK The uctype,union of 2 Structure of SIP hosts are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpHost(ST_SIP_HOST *pstParm1,
                ST_SIP_HOST *pstParm2);

/**
 * @brief Compare hostport.
 *
 * @param [in] pstParm1 First hostport.
 * @param [in] pstParm2 Second hostport.
 *
 * @retval ZOK 2 port must be defined and the stHost,PortPres and iPort of
 *             2 Structure of SIP hostports are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpHostPort(ST_SIP_HOST_PORT *pstParm1,
                ST_SIP_HOST_PORT *pstParm2);

/**
 * @brief Compare header.
 *
 * @param [in] pstParm1 First header.
 * @param [in] pstParm2 Second header.
 *
 * @retval ZOK 2 port's value not null and 2 headers are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpHdr(ST_SIP_HDR *pstParm1,
                ST_SIP_HDR *pstParm2);

/**
 * @brief Compare headers.
 *
 * @param [in] pstParm1 First headers.
 * @param [in] pstParm2 Second headers.
 *
 * @retval ZOK 2 port's value not null and stHdrLst of 2 headerses are equal 
 *             and 2 headers are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpHdr
 */
ZFUNC ZINT Sip_CmpHdrs(ST_SIP_HDRS *pstParm1,
                ST_SIP_HDRS *pstParm2);

/**
 * @brief Compare SIP-URI.
 *
 * @param [in] pstParm1 First SIP-URI.
 * @param [in] pstParm2 Second SIP-URI.
 *
 * @retval ZOK The 2 SIP-URIs are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpHostPort, Sip_CmpHostPortX, Sip_CmpHost, Sip_CmpUserInfo
 */
ZFUNC ZINT Sip_CmpSipUri(ST_SIP_SIP_URI *pstParm1,
                ST_SIP_SIP_URI *pstParm2);

/**
 * @brief Compare global-number.
 *
 * @param [in] pstParm1 First global-number.
 * @param [in] pstParm2 Second global-number.
 *
 * @retval ZOK The stNumberDigits and stParLst of structure of global-numbers
 *            are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpPar
 */
ZFUNC ZINT Sip_CmpGlobalNumber(ST_SIP_GLOBAL_NUMBER *pstParm1,
                ST_SIP_GLOBAL_NUMBER *pstParm2);

/**
 * @brief Compare local-number.
 *
 * @param [in] pstParm1 First local-number.
 * @param [in] pstParm2 Second local-number.
 *
 * @retval ZOK The stNumberDigits and stParLst of structure of local-numbers
 *             are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpPar
 */
ZFUNC ZINT Sip_CmpLocalNumber(ST_SIP_LOCAL_NUMBER *pstParm1,
                ST_SIP_LOCAL_NUMBER *pstParm2);

/**
 * @brief Compare TEL-URI.
 *
 * @param [in] pstParm1 First TEL-URI.
 * @param [in] pstParm2 Second TEL-URI.
 *
 * @retval ZOK it depends on the type of TEL-URI                  
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpLocalNumber, Sip_CmpGlobalNumber
 */
ZFUNC ZINT Sip_CmpTelUri(ST_SIP_TEL_URI *pstParm1,
                ST_SIP_TEL_URI *pstParm2);

/**
 * @brief Compare absoluteURI.
 *
 * @param [in] pstParm1 First absoluteURI.
 * @param [in] pstParm2 Second absoluteURI.
 *
 * @retval ZOK The stScheme and stDesc of absoluteURI are equal.                  
 * @retval ZFAILED Otherwise.
 *
 * @see
 */
ZFUNC ZINT Sip_CmpAbsoUri(ST_SIP_ABSO_URI *pstParm1,
                ST_SIP_ABSO_URI *pstParm2);

/**
 * @brief Compare addr-spec.
 *
 * @param [in] pstParm1 First addr-spec.
 * @param [in] pstParm2 Second addr-spec.
 *
 * @retval ZOK it depends on the uctype of structure of addr-spec                  
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpAbsoUri, Sip_CmpTelUri, Sip_CmpImUri, Sip_CmpMcUri
 */
ZFUNC ZINT Sip_CmpAddrSpec(ST_SIP_ADDR_SPEC *pstParm1,
                ST_SIP_ADDR_SPEC *pstParm2);

/**
 * @brief Compare addr-spec.
 *
 * @param [in] pstParm1 First addr-spec.
 * @param [in] pstParm2 Second addr-spec.
 *
 * @retval ZOK The 2 addr-specs are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpAddrSpecX(ST_SIP_NA_SPEC *pstParm1,
                ST_SIP_NA_SPEC *pstParm2);

/**
 * @brief Compare addr-spec.
 *
 * @param [in] pstParm1 First display-name.
 * @param [in] pstParm2 Second display-name.
 *
 * @retval ZOK The ucNamePres, u.stName, u.stQStr of display-name are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpDispName(ST_SIP_DISP_NAME *pstParm1,
                ST_SIP_DISP_NAME *pstParm2);

/**
 * @brief Compare name address.
 *
 * @param [in] pstParm1 First name address.
 * @param [in] pstParm2 Second name address.
 *
 * @retval ZOK The ucDispNamePres, stDispName, stAddrSpec of name-addr are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpNameAddr(ST_SIP_NAME_ADDR *pstParm1,
                ST_SIP_NAME_ADDR *pstParm2);

/**
 * @brief Compare name-addr or addr-spec.
 *
 * @param [in] pstParm1 First ame-addr or addr-spec.
 * @param [in] pstParm2 Second ame-addr or addr-spec.
 *
 * @retval ZOK The 2 name-addr are equal.                 
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpNaSpec(ST_SIP_NA_SPEC *pstParm1,
                ST_SIP_NA_SPEC *pstParm2);

/**
 * @brief Compare request-uri.
 *
 * @param [in] pstParm1 First request-uri.
 * @param [in] pstParm2 Second request-uri.
 *
 * @retval ZOK it depends on the uctype of structure of request-uri.
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpSipUri, Sip_CmpTelUri, Sip_CmpImUri, Sip_CmpMcUri
 */
ZFUNC ZINT Sip_CmpReqUri(ST_SIP_REQ_URI *pstParm1,
                ST_SIP_REQ_URI *pstParm2);

/**
 * @brief Compare IM-URI.
 *
 * @param [in] pstParm1 First IM-URI.
 * @param [in] pstParm2 Second IM-URI.
 *
 * @retval ZOK The ucDescPres,stDesc of IM-URI are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpImUri(ST_SIP_IM_URI *pstParm1,
                ST_SIP_IM_URI *pstParm2);

/**
 * @brief Compare Message-ID & Content-ID uri.
 *
 * @param [in] pstParm1 First Message-ID & Content-ID uri.
 * @param [in] pstParm2 Second Message-ID & Content-ID uri.
 *
 * @retval ZOK The stLocal,stHost of Message-ID & Content-ID uri are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpMcUri(ST_SIP_MC_URI *pstParm1,
                ST_SIP_MC_URI *pstParm2);

/**
 * @brief Compare Method.
 *
 * @param [in] pstParm1 First Method.
 * @param [in] pstParm2 Second Method.
 *
 * @retval ZOK 2 method type are equal. When the type of Method is 
 *             EN_SIP_METHOD_EXT, the stExt of Method are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpMethod(ST_SIP_METHOD *pstParm1,
                ST_SIP_METHOD *pstParm2);

/**
 * @brief Compare call-id.
 *
 * @param [in] pstParm1 First call-id.
 * @param [in] pstParm2 Second call-id.
 *
 * @retval ZOK The ucHostPres, stNumber, stHost of call-id are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpCallId(ST_SIP_CALLID *pstParm1,
                ST_SIP_CALLID *pstParm2);

/**
 * @brief Compare route parameter.
 *
 * @param [in] pstParm1 First route parameter.
 * @param [in] pstParm2 Second route parameter.
 *
 * @retval ZOK The stNameaddr, stRrParmLst of route parameter are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpRouteParm(ST_SIP_ROUTE_PARM *pstParm1,
                ST_SIP_ROUTE_PARM *pstParm2);

/**
 * @brief Compare via-parms.
 *
 * @param [in] pstParm1 First via-parms.
 * @param [in] pstParm2 Second via-parms.
 *
 * @retval ZOK The parameter type and detail structure are equal.                  
 * @retval ZFAILED Otherwise.
 *
 * @see Sip_CmpHost
 */
ZFUNC ZINT Sip_CmpViaParms(ST_SIP_VIA_PARMS *pstParm1,
                ST_SIP_VIA_PARMS *pstParm2);

/**
 * @brief Compare via-parm.
 *
 * @param [in] pstParm1 First via-parm.
 * @param [in] pstParm2 Second via-parm.
 *
 * @retval ZOK The stProtocol, stBy, stParmsLst of via-parm are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpViaParm(ST_SIP_VIA_PARM *pstParm1,
                ST_SIP_VIA_PARM *pstParm2);
                
/**
 * @brief Compare sent-protocol parameter.
 *
 * @param [in] pstParm1 First sent-protocol parameter.
 * @param [in] pstParm2 Second sent-protocol parameter.
 *
 * @retval ZOK The stName,stVer,ucTptType of sent-protocol parameter are equal.
 *         and when ucTptType is equal with EN_SIP_TPT_OTHER,
 *         the stOtherTpt of sent-protocol parameter are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpViaSentProtocol(ST_SIP_SENT_PROTOCOL *pstParm1,
                ST_SIP_SENT_PROTOCOL *pstParm2);

/**
 * @brief Compare sent-by parameter.
 *
 * @param [in] pstParm1 First sent-by parameter.
 * @param [in] pstParm2 Second sent-by parameter.
 *
 * @retval ZOK The stHost, ucPortPres, iport of sent-by parameter are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpViaSentBy(ST_SIP_SENT_BY *pstParm1,
                ST_SIP_SENT_BY *pstParm2); 

/**
 * @brief Compare from-param & to-param.
 *
 * @param [in] pstParm1 First from-param & to-param.
 * @param [in] pstParm2 Second from-param & to-param.
 *
 * @retval ZOK The ucTagPres, u.stTag, u.stParm of 2 from-param or 
 *             to-params are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpFromToParm(ST_SIP_FROM_TO_PARM *pstParm1,
                ST_SIP_FROM_TO_PARM *pstParm2);

/**
 * @brief Compare generic-param.
 *
 * @param [in] pstParm1 First generic-param.
 * @param [in] pstParm2 Second generic-param.
 *
 * @retval ZOK The type and detail structure of generic-param are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpGenParm(ST_SIP_GEN_PARM *pstParm1, 
                ST_SIP_GEN_PARM *pstParm2);

/**
 * @brief Compare  contact-param.
 *
 * @param [in] pstParm1 First contact-param.
 * @param [in] pstParm2 Second contact-param.
 *
 * @retval ZOK The stParmsLst, stNaSpec of 2 contact-params are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpContactParm(ST_SIP_CONTACT_PARM *pstParm1,
                ST_SIP_CONTACT_PARM *pstParm2);

/**
 * @brief Compare contact-params.
 *
 * @param [in] pstParm1 First contact-params.
 * @param [in] pstParm2 Second contact-params.
 *
 * @retval ZOK The type and detail structure of contact-params are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpContactParms(ST_SIP_CONTACT_PARMS *pstParm1,
                ST_SIP_CONTACT_PARMS *pstParm2);

/**
 * @brief Compare  c-p-q.
 *
 * @param [in] pstParm1 First c-p-q.
 * @param [in] pstParm2 Second c-p-q.
 *
 * @retval ZOK The wFrac, wInt, ucFracPres of 2 c-p-qs are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpQval(ST_SIP_QVAL *pstParm1,
                ST_SIP_QVAL *pstParm2);

/**
 * @brief Compare uri-parameter.
 *
 * @param [in] pstParm1 First uri-parameter.
 * @param [in] pstParm2 Second uri-parameter.
 *
 * @retval ZOK The type and detail structure of uri-parameter are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpUriParm(ST_SIP_URI_PARM *pstParm1,
                ST_SIP_URI_PARM *pstParm2);

/**
 * @brief Compare sip par (global-phone-number's parameters).
 *
 * @param [in] pstParm1 First sip par (global-phone-number's parameters).
 * @param [in] pstParm2 Second sip par (global-phone-number's parameters).
 *
 * @retval ZOK The type and detail structure of sip par 
 *             (global-phone-number's parameters) are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpPar(ST_SIP_PAR *pstParm1,
                ST_SIP_PAR *pstParm2);
/**
 * @brief Compare  sip parameter.
 *
 * @param [in] pstParm1 First sip parameter.
 * @param [in] pstParm2 Second sip parameter.
 *
 * @retval ZOK The stPname, ucValPres, stPval of 2 sturcture 
 *             of sip parameters are equal.                   
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpParm(ST_SIP_PARM *pstParm1,
                ST_SIP_PARM *pstParm2);

/**
 * @brief Compare sip event package.
 *
 * @param [in] pstParm1 First sip event package.
 * @param [in] pstParm2 Second sip event package.
 *
 * @retval ZOK The ucType of sip event package are equal.
 *         and when ucType is equal with EN_SIP_EVNT_PKG_OTHER,
 *         the stOther of sip event packager are equal.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Sip_CmpEvntPkg(ST_SIP_EVNT_PKG *pstParm1,
                ST_SIP_EVNT_PKG *pstParm2);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_MSG_DUP_H__ */


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
  File name     : sdp_abnf_encode.h
  Module        : sdp abnf encode
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2005-04-14    
  Description   :
      Data structure and function definitions required by the sdp abnf encode
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SDP_ABNF_ENCODE_H__
#define _SDP_ABNF_ENCODE_H__

#ifdef __cplusplus
extern "C"{
#endif

/* sdp encode result fail */
#define SDP_ENCODE_RETFAIL(_msg, _info)  do { \
    ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
    return ZFAILED; \
} while (0)

/* sdp encode check result */
#define SDP_ENCODE_CHKRET(_msg, _ret, _info)  do { \
    if (_ret != ZOK) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp check present flag */
#define SDP_ENCODE_CHKPRES(_msg, _pres, _info) do { \
    if (_pres != ZTRUE) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp check optional field */
#define SDP_ENCODE_CHKOPT(_pres) if (_pres != ZTRUE) return ZOK; 

/* sdp check mandatory field */
#define SDP_ENCODE_CHKMAND(_msg, _lst, _info) do { \
    if (ABNF_LIST_SIZE(_lst) == 0) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp check optional field of list */
#define SDP_ENCODE_CHKOPT2(_lst) if (ABNF_LIST_ISEMPTY(_lst)) return ZOK

/* sdp check version */
#define SDP_ENCODE_CHKVER(_msg, _ver, _info) do { \
    if (_ver != 1) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/*lint -save -e* */

/* encode a character */
#define SDP_ADD_CHR(_msg, _chr, _ret) \
    _ret = Abnf_AddPstChr(_msg, _chr)

/* encode a token */
#define SDP_ADD_TKN(_msg, _tkntype, _tknid, _ret) \
    _ret = Sdp_TknEncode(_msg, _tkntype, _tknid)

/* encode a string */
#define SDP_ADD_SSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPstSStr(_msg, _ststr)

/* encode a string with length */
#define SDP_ADD_STRN(_msg, _str, _len, _ret) \
    _ret = Abnf_AddPstStrN(_msg, _str, _len)

/* encode unsigned long decimal data */
#define SDP_ADD_UIDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUiDigit(_msg, _data)

/* encode unsigned short decimal data */
#define SDP_ADD_USDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUsDigit(_msg, _data)

/* encode unsigned chr decimal data */
#define SDP_ADD_UCDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUcDigit(_msg, _data)

/* encode unsigned int hexadecimal data */
#define SDP_ADD_XUIDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddXUiDigit(_msg, _data)

/* encode CRLF */
#define SDP_ADD_CRLF(_msg, _ret) SDP_ADD_STRN(_msg, "\r\n", 2, _ret)

/* encode space */
#define SDP_ADD_SPACE(_msg, _ret) SDP_ADD_CHR(_msg, EN_CHR_WSP, _ret)

/* encode an ip in the buffer */
#define SDP_ADD_IPV4(_msg, _ip, _ret) _ret = Abnf_AddIpV4(_msg, _ip)

/* encode an ipv6 in the buffer */
#define SDP_ADD_IPV6(_msg, _ipv6, _ret) _ret = Abnf_AddIpV6(_msg, _ipv6)

/* encode a quoted string */
#define SDP_ADD_QSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPairSStr(_msg, '"', _ststr)

/*lint -restore */

/* sdp encode list of session-description */
ZFUNC ZINT Sdp_EncodeSessDescLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SESS_DESC_LST *pstSessDescLst);

/* sdp encode one session-description */
ZFUNC ZINT Sdp_EncodeSessDesc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SESS_DESC *pstSessDesc);
    
/* sdp encode field */
ZFUNC ZINT Sdp_EncodeVF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_VF *pstVer);
ZFUNC ZINT Sdp_EncodeOF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_OF *pstOrig);
ZFUNC ZINT Sdp_EncodeSF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_SF *pstSessName);
ZFUNC ZINT Sdp_EncodeIF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_IF *pstInfo);
ZFUNC ZINT Sdp_EncodeUF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_UF *pstUri);
ZFUNC ZINT Sdp_EncodeEFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_EF_LST *pstEmailLst);
ZFUNC ZINT Sdp_EncodeEF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_EF *pstEmail);
ZFUNC ZINT Sdp_EncodePFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PF_LST *pstPhoneLst);
ZFUNC ZINT Sdp_EncodePF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_PF *pstPhone);
ZFUNC ZINT Sdp_EncodeCFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CF_LST *pstConnLst);
ZFUNC ZINT Sdp_EncodeCF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_CF *pstConn);
ZFUNC ZINT Sdp_EncodeBFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_BF_LST *pstBwLst);
ZFUNC ZINT Sdp_EncodeBF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_BF *pstBw);
ZFUNC ZINT Sdp_EncodeSessTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SESS_TIME *pstSessTime);
ZFUNC ZINT Sdp_EncodeTFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TF_LST *pstTimeLst);
ZFUNC ZINT Sdp_EncodeTF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_TF *pstTime);
ZFUNC ZINT Sdp_EncodeKF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_KF *pstKey);
ZFUNC ZINT Sdp_EncodeAFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_AF_LST *pstAttrLst);
ZFUNC ZINT Sdp_EncodeAF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_AF *pstAttr);
ZFUNC ZINT Sdp_EncodeMdescLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MDESC_LST *pstMdescLst);
ZFUNC ZINT Sdp_EncodeMdesc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MDESC *pstMdesc);
ZFUNC ZINT Sdp_EncodeMF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_MF *pstMedia);
ZFUNC ZINT Sdp_EncodeRFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RF_LST *pstRepLst);
ZFUNC ZINT Sdp_EncodeRF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_RF *pstRep);
ZFUNC ZINT Sdp_EncodeZFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ZF_LST *pstZoneAdjLst);
ZFUNC ZINT Sdp_EncodeZF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_ZF *pstZoneAdj);

ZFUNC ZINT Sdp_EncodeUcastAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_UCAST_ADDR *pstAddr);
ZFUNC ZINT Sdp_EncodeConnAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CONN_ADDR *pstConnAddr);
ZFUNC ZINT Sdp_EncodeFmtLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMT_LST *pstFmtLst);
ZFUNC ZINT Sdp_EncodeFmt(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_FMT *pstFmt);
ZFUNC ZINT Sdp_EncodeTypedTimeLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPED_TIME_LST *pstTypedTimeLst);
ZFUNC ZINT Sdp_EncodeTypedTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPED_TIME *pstTypedTime);
ZFUNC ZINT Sdp_EncodeAttrStr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_STR *pstAttrStr);
ZFUNC ZINT Sdp_EncodeIp4Mcast(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCAST_IP4 *pstIp4Mcast);
ZFUNC ZINT Sdp_EncodeIp6Mcast(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCAST_IP6 *pstIp6Mcast);
ZFUNC ZINT Sdp_EncodeAttrConf(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_CONF *pstConf);
ZFUNC ZINT Sdp_EncodeFmtp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP *pstFmtp);
ZFUNC ZINT Sdp_EncodeFmt2733(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_2733 *pst2733);
ZFUNC ZINT Sdp_EncodeFmt2833(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_2833 *pst2833);
ZFUNC ZINT Sdp_EncodeFmt3640(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3640 *pst3640);
ZFUNC ZINT Sdp_EncodeFmt3952(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3952 *pst3952);
ZFUNC ZINT Sdp_EncodeFmtH26X(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_H26X *pstH26x);
ZFUNC ZINT Sdp_EncodeFmt4587(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_4587 *pst4587);
ZFUNC ZINT Sdp_EncodeFmt4629(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_4629 *pst4629);
ZFUNC ZINT Sdp_EncodeFmt3984(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3984 *pst3984);
ZFUNC ZINT Sdp_EncodeFmt3267(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3267 *pst3267);
ZFUNC ZINT Sdp_EncodeFmtTbcp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_TBCP *pstTbcp);
ZFUNC ZINT Sdp_EncodeFmt4588(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_4588 *pst4588);
ZFUNC ZINT Sdp_Encode2833ValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_2833_VAL_LST *pstValLst);
ZFUNC ZINT Sdp_Encode2833Val(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_2833_VAL *pstVal);
ZFUNC ZINT Sdp_Encode3640ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3640_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Encode3640Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3640_PARM *pstParm);
ZFUNC ZINT Sdp_EncodeH261OptLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H261_OPT_LST *pstH261OptLst);
ZFUNC ZINT Sdp_EncodeH261Opt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H261_OPT *pstH261Opt);
ZFUNC ZINT Sdp_EncodeH263OptLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_OPT_LST *pstH263OptLst);
ZFUNC ZINT Sdp_EncodeH263Opt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_OPT *pstH261Opt);
ZFUNC ZINT Sdp_EncodeH263Size(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_SIZE *pstH261Size);
ZFUNC ZINT Sdp_EncodeH263Anx(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_ANX *pstH261Anx);
ZFUNC ZINT Sdp_EncodeH263Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_PARM *pstParm);
ZFUNC ZINT Sdp_Encode4587ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4587_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Encode4587Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4587_PARM *pstParm);
ZFUNC ZINT Sdp_Encode4629ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4629_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Encode4629Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4629_PARM *pstParm);
ZFUNC ZINT Sdp_Encode4629Custom(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4629_CUSTOM *pstCustom);
ZFUNC ZINT Sdp_Encode3984ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3984_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Encode3984Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3984_PARM *pstParm);
ZFUNC ZINT Sdp_Encode3267ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3267_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Encode3267Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3267_PARM *pstParm);
ZFUNC ZINT Sdp_EncodeTbcpParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TBCP_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_EncodeTbcpParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TBCP_PARM *pstParm);
ZFUNC ZINT Sdp_EncodeRtpmap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTPMAP *pstRtpMap);
ZFUNC ZINT Sdp_EncodeCurSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CUR_STA *pstCurSta);
ZFUNC ZINT Sdp_EncodeDesSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DES_STA *pstDesSta);
ZFUNC ZINT Sdp_EncodeCnfSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CNF_STA *pstCnfSta);
ZFUNC ZINT Sdp_EncodeRtcp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTCP *pstRtcp);
ZFUNC ZINT Sdp_EncodeMid(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MID *pstMid);
ZFUNC ZINT Sdp_EncodeGrp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_GRP *pstGrp);
ZFUNC ZINT Sdp_EncodeClir(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bClir);
ZFUNC ZINT Sdp_EncodeAttrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_LST *pstAttrLst);
ZFUNC ZINT Sdp_EncodeAttr(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_ATTR *pstAttr);
ZFUNC ZINT Sdp_EncodeIdTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ID_TAG_LST *pstIdTagLst);
ZFUNC ZINT Sdp_EncodeIdTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ID_TAG *pstIdTag);
ZFUNC ZINT Sdp_EncodeSsrc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SSRC *pstSsrc);
ZFUNC ZINT Sdp_EncodeSsrcGrp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SSRC_GRP *pstSsrcGrp);
ZFUNC ZINT Sdp_EncodeSsrcIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SSRC_ID_LST *pstSsrcIdLst);
ZFUNC ZINT Sdp_EncodeContent(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CNT *pstContent);
ZFUNC ZINT Sdp_EncodeMcntLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCNT_LST *pstMcntLst);
ZFUNC ZINT Sdp_EncodeMcnt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCNT *pstContent);
ZFUNC ZINT Sdp_EncodeAttValue(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_COPT *pstCOpt);
ZFUNC ZINT Sdp_EncodeCOptLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_C_OPT_LST *pstCOptLst);
ZFUNC ZINT Sdp_EncodeAcap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ACAP *pstAcap);
ZFUNC ZINT Sdp_EncodeTcap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TCAP *pstTcap);
ZFUNC ZINT Sdp_EncodeProtoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PROTO_LST *pstProtoLst);
ZFUNC ZINT Sdp_EncodePcfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PCFG *pstPcfg);
ZFUNC ZINT Sdp_EncodePotCfgLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_POT_CFG_LST *pstPotCfgLst);
ZFUNC ZINT Sdp_EncodePotCfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_POT_CFG *pstPotCfg);
ZFUNC ZINT Sdp_EncodeTransProCfgLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TRANS_PRO_CFG_LST *pstTransProCfgLst);
ZFUNC ZINT Sdp_EncodeAttrCfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_CFG_LST *pstAttrCfg);
ZFUNC ZINT Sdp_EncodeMaClLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MA_CL_LST *pstMaClLst);
ZFUNC ZINT Sdp_EncodeMaCl(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MA_CL *pstMaCl);
ZFUNC ZINT Sdp_EncodeAttCapLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATT_CAP_LST *pstAttCapLst);
ZFUNC ZINT Sdp_EncodeAcfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ACFG *pstAcfg);
ZFUNC ZINT Sdp_EncodeSelCfgLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SEL_CFG_LST *pstSelCfgLst);
ZFUNC ZINT Sdp_EncodeSelCfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SEL_CFG *pstSelCfg);
ZFUNC ZINT Sdp_EncodeAcptTypes(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ACPT_TYPES *pstAcptTypes);
ZFUNC ZINT Sdp_EncodeFormatLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FORMAT_LST *pstFormatLst);
ZFUNC ZINT Sdp_EncodeFormat(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FORMAT *pstFormat);
ZFUNC ZINT Sdp_EncodeFormatType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FORMAT_TYPE *pstFormatType);
ZFUNC ZINT Sdp_EncodeTypeParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPE_PARM_LST *pstTypeParmLst);
ZFUNC ZINT Sdp_EncodeTypeParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPE_PARM *pstTypeParm);
ZFUNC ZINT Sdp_EncodePval(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PVAL *pstPval);
ZFUNC ZINT Sdp_EncodeQStr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQStr);
ZFUNC ZINT Sdp_EncodePath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PATH *pstPath);
ZFUNC ZINT Sdp_EncodeMsrpUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MSRP_URI_LST *pstMsrpUriLst);
ZFUNC ZINT Sdp_EncodeMsrpUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MSRP_URI *pstMsrpUri);
ZFUNC ZINT Sdp_EncodeAuthority(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_AUTHORITY *pstAuthority);
ZFUNC ZINT Sdp_EncodeUserInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstUserInfo);
ZFUNC ZINT Sdp_EncodeHost(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_HOST *pstHost);
ZFUNC ZINT Sdp_EncodeIpv6(ST_ABNF_MSG *pstAbnfMsg, ZUCHAR *pucIpv6Addr);
ZFUNC ZINT Sdp_EncodeIpvFuture(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IPV_FUTURE *pstIpvFuture);
ZFUNC ZINT Sdp_EncodeTrans(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TRANS *pstTrans);
ZFUNC ZINT Sdp_EncodeUriParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_URI_PARM_LST *pstUriParmLst);
ZFUNC ZINT Sdp_EncodeUriParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_URI_PARM *pstUriParm);
ZFUNC ZINT Sdp_EncodeFileSlt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_SLT *pstFileSlt);
ZFUNC ZINT Sdp_EncodeSelectorLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_LST *pstSelectorLst);
ZFUNC ZINT Sdp_EncodeSelector(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR *pstSelector);
ZFUNC ZINT Sdp_EncodeFileName(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_FNAME *pstFileName);
ZFUNC ZINT Sdp_EncodeFileType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_FTYPE *pstFileType);
ZFUNC ZINT Sdp_EncodeFileHash(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_HASH *pstFileHash);
ZFUNC ZINT Sdp_EncodeFtParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FTPARM_LST *pstFtParmLst);
ZFUNC ZINT Sdp_EncodeFtParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FTPARM *pstFtParm);
ZFUNC ZINT Sdp_EncodeHashValueLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_HASH_VALUE_LST *pstHashValueLst);
ZFUNC ZINT Sdp_EncodeFileDate(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_DATE *pstFileDate);
ZFUNC ZINT Sdp_EncodeDateParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE_PARM_LST *pstDateParmLst);
ZFUNC ZINT Sdp_EncodeDateParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE_PARM *pstDateParm);
ZFUNC ZINT Sdp_EncodeDateTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE_TIME *pstDate);
ZFUNC ZINT Sdp_EncodeDayOfWeek(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DAY_OF_WEEK *pstDayOfWeek);
ZFUNC ZINT Sdp_EncodeDate(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE *pstDate);
ZFUNC ZINT Sdp_EncodeDayYear(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DAY_YEAR *pstDay);
ZFUNC ZINT Sdp_EncodeTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TIME *pstTime);
ZFUNC ZINT Sdp_EncodeTimeOfDay(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TIME_OF_DAY *pstTimeOfDay);
ZFUNC ZINT Sdp_EncodeZone(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ZONE *pstZone);
ZFUNC ZINT Sdp_EncodeCfws(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CFWS *pstCfws);
ZFUNC ZINT Sdp_EncodeCommLst(ST_ABNF_MSG *pstAbnfMsg,
                    ST_SDP_COMM_LST *pstCommLst);
ZFUNC ZINT Sdp_EncodeComm(ST_ABNF_MSG *pstAbnfMsg,
                    ST_SDP_COMM *pstComm);
ZFUNC ZINT Sdp_EncodeCConLst(ST_ABNF_MSG *pstAbnfMsg,
                    ST_SDP_CCON_LST *pstCConLst);
ZFUNC ZINT Sdp_EncodeFws(ST_ABNF_MSG *pstAbnfMsg);
ZFUNC ZINT Sdp_EncodeFileIcon(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_ICON *pstFileIcon);
ZFUNC ZINT Sdp_EncodeCidUrl(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CID_URL *pstCidUrl);
ZFUNC ZINT Sdp_EncodeAddrSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ADDR_SPEC *pstAddrSpec);
ZFUNC ZINT Sdp_EncodeWordLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_WORD_LST *pstWordLst);
ZFUNC ZINT Sdp_EncodeWord(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_WORD *pstWord);
ZFUNC ZINT Sdp_EncodeSDomainLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SDOMAIN_LST *pstSDomainLst);
ZFUNC ZINT Sdp_EncodeSDomain(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SDOMAIN *pstSDomain);
ZFUNC ZINT Sdp_EncodeDLiteral(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstDLiteral);
ZFUNC ZINT Sdp_EncodeQDTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQDTime);
ZFUNC ZINT Sdp_EncodeFileRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_RANGE *pstFileRange);
ZFUNC ZINT Sdp_EncodeConnection(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CONNECTION *pstConn);
ZFUNC ZINT Sdp_EncodeSetup(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SETUP *pstSetup);
ZFUNC ZINT Sdp_EncodeFramesize(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FRAMESIZE *pstFramesize);
ZFUNC ZINT Sdp_EncodeImgAttr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR *pstImgAttr);
ZFUNC ZINT Sdp_EncodeImgAttrSetLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_SET_LST *pstSetLst);
ZFUNC ZINT Sdp_EncodeImgAttrSet(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_SET *pstSet);
ZFUNC ZINT Sdp_EncodeImgAttrRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_RANGE *pstRange);
ZFUNC ZINT Sdp_EncodeImgAttrSar(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_SRANGE *pstSar);
ZFUNC ZINT Sdp_EncodeImgAttrPar(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_PRANGE *pstPar);
ZFUNC ZINT Sdp_EncodeImgAttrXrange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_XRANGE *pstXrange);
ZFUNC ZINT Sdp_EncodeImgAttrFloat(ST_ABNF_MSG *pstAbnfMsg, ZFLOAT fValue,
                ZUINT iDecCnt);
ZFUNC ZINT Sdp_EncodeRtcpFb(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTCP_FB *pstRtcpFb);
ZFUNC ZINT Sdp_EncodeRtcpFbParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTCP_FB_PARM *pstParm);
ZFUNC ZINT Sdp_EncodeCand(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CAND *pstCand);
ZFUNC ZINT Sdp_EncodeRmtCands(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RMT_CAND_LST *pstRmtCandLst);
ZFUNC ZINT Sdp_EncodeIceOpts(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR_LST *pstIceOptLst);
ZFUNC ZINT Sdp_EncodeFgrprnt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FGRPRNT *pstFgrprnt);
ZFUNC ZINT Sdp_EncodeCrypto (ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO *pstCrypto);
ZFUNC ZINT Sdp_EncodeCryptoKeyParmLst(ST_ABNF_MSG *pstAbnfMsg,  
                ST_SDP_CRYPTO_KEY_PARM_LST *pstKeyParmLst);
ZFUNC ZINT Sdp_EncodeCryptoKeyParm (ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_KEY_PARM *pstKeyParam);
ZFUNC ZINT Sdp_EncodeCryptoSrtpInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_STRP_KEY_INFO *pstKeyInfo);
ZFUNC ZINT Sdp_EncodeCryptoSessParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_SESS_PARM_LST *pstSessParmLst);
ZFUNC ZINT Sdp_EncodeCryptoSessParm (ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_SESS_PARM *pstParm);
ZFUNC ZINT Sdp_EncodeExtMap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_EXTMAP *pstExtMap);

#ifdef __cplusplus
}
#endif

#endif /* _SDP_ABNF_ENCODE_H__ */


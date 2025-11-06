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
  File name     : sdp_abnf_decode.h
  Module        : sdp abnf decode
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2005-04-14    
  Description   :
      Data structure and function definitions required by the sdp abnf decode
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SDP_ABNF_DECODE_H__
#define _SDP_ABNF_DECODE_H__

#ifdef __cplusplus
extern "C"{
#endif

/* decode result fail */
#define SDP_DECODE_RETFAIL(_msg, _info)   do { \
    ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
    return ZFAILED; \
} while (0)    

/* decode check result */
#define SDP_DECODE_CHKRET(_msg, _ret, _info) do { \
    if (_ret != ZOK) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* decode check memory */
#define SDP_DECODE_CHKMEM(_msg, _mem, _info) do { \
    if (_mem == ZNULL) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp check mandatory field */
#define SDP_DECODE_CHKMAND(_msg, _lst, _info) do { \
    if (ABNF_LIST_SIZE(_lst) == 0) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp check version */
#define SDP_DECODE_CHKVER(_msg, _ver, _info) do { \
    if (_ver != 0) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp save buffer */
#define SDP_SAVE_BUF_STATE ABNF_SAVE_BUF_STATE

/* sdp restore buffer */
#define SDP_RESTORE_BUF_STATE ABNF_RESTORE_BUF_STATE

/* sdp is match field */
/*lint -save -e(773) */
#define SDP_DECODE_MATCHFIELD(_msg, _chr, _ret) \
    _ret = Sdp_DecodeMatchField(_msg, _chr)
/*lint -restore */

/* sdp check field */
#define SDP_DECODE_CHKFIELD(_msg, _chr, _ret) do { \
    SDP_DECODE_MATCHFIELD(_msg, _chr, _ret); \
    if (_ret != ZOK) return ZOK; \
} while (0)

/* decode check result */
#define SDP_DECODE_CHKTKNID(_msg, _tknid, _info) do { \
    if (_tknid == ABNF_TKNID_UNKNOWN) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* sdp get the last separator */
#define SDP_GET_LASTSEPA ABNF_GET_LASTSEPA

/*lint -save -e* */

/* sdp get the scanned string */
#define SDP_GET_SCANNED_STR(_msg, _pstr) ABNF_GET_SCANNED_STR(_msg, _pstr) 

/* sdp expect to match one character */
#define SDP_EXPECT_CHR(_msg, _chr, _case, _ret) \
    _ret = Abnf_ExpectChr(_msg, _chr, _case)

/* sdp try to expect to match one character */
#define SDP_TRY_EXPECT_CHR(_msg, _chr, _case, _ret) \
    _ret = Abnf_TryExpectChr(_msg, _chr, _case)

/** @brief The sdp gets one character. */
#define SDP_GET_CHR(_msg, _pchr, _ret) \
    _ret = Abnf_GetChr(_msg, _pchr)

/* sdp decode try to expect eol(end of line) */
#define SDP_TRY_EXPECT_EOL(_msg, _ret)  _ret = Abnf_TryExpectEol(_msg)

/* sdp decode ignore WSP = SP / HTAB*/
#define SDP_IGNORE_WSP(_msg, _ret) _ret = Abnf_IgnWS(_msg)

/* get a line */
#define SDP_GET_LINE(_msg, _pststr, _ret) \
    ABNF_SET_CHRLEN(_msg, 1, ABNF_MAX_LEN_NA); \
    _ret = Abnf_GetLine(_msg, _pststr); \
    ABNF_CLR_CHRLEN(_msg)

/* get a string(length can't be zero) */
#define SDP_GET_NSSTR(_msg, _class, _pststr, _ret) \
    _ret = Abnf_GetNSStrChrset(_msg, Sdp_ChrsetGetId(), _class, 1, \
                               ABNF_MAX_LEN_NA, _pststr)

/* get a string(length can't be zero) */
#define SDP_GET_NSSTR_ESCAPE(_msg, _class, _chr, _pststr, _ret) \
    _ret = Abnf_GetNSStrEscape(_msg, Sdp_ChrsetGetId(), _class, _chr, \
                               _class, 1, ABNF_MAX_LEN_NA, _pststr)

/* get a string(length can be zero) */
#define SDP_GET_SSTR_ESCAPE(_msg, _class, _echr, _eclass, _pststr, _ret) \
    _ret = Abnf_GetSStrEscape(_msg, Sdp_ChrsetGetId(), _class, _echr, _eclass, \
                              _pststr)

/* get a string(length can be zero) */
#define SDP_GET_SSTR_SEPAS(_msg, _chr, _pststr, _ret) \
    _ret = Abnf_GetSStrSepas(_msg, _chr, '\r', '\n', 0, _pststr)

/* get unsigned int decimal data */
#define SDP_GET_UIDIGIT(_msg, _pdata, _ret)  \
    _ret = Abnf_GetUiDigit(_msg, _pdata)

/* get unsigned short decimal data */
#define SDP_GET_USDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUsDigit(_msg, _pdata)

/* get unsigned chr decimal data */
#define SDP_GET_UCDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUcDigit(_msg, _pdata)

/* get unsigned long hexadecimal data */
#define SDP_GET_XUIDIGIT(_msg, _pdata, _ret)  \
    _ret = Abnf_GetXUiDigit(_msg, _pdata)

/* expect the eol(end of line */
#define SDP_EXPECT_EOL(_msg, _ret) _ret = Abnf_ExpectEol(_msg)

/* sdp decode an ip in the buffer */
#define SDP_GET_IPV4(_msg, _ip, _ret) _ret = Abnf_GetIpV4(_msg, _ip)

/* sdp decode an ipv6 in the buffer */
#define SDP_GET_IPV6(_msg, _ipv6, _ret) _ret = Abnf_GetIpV6(_msg, _ipv6)

/*lint -restore */

/* sdp decode list of session-description */
ZFUNC ZINT Sdp_DecodeSessDescLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SESS_DESC_LST *pstSessDescLst);

/* sdp decode one session-description */
ZFUNC ZINT Sdp_DecodeSessDesc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SESS_DESC *pstSessDesc);

/* sdp decode field */
ZFUNC ZINT Sdp_DecodeVF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_VF *pstVer);
ZFUNC ZINT Sdp_DecodeOF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_OF *pstOrig);
ZFUNC ZINT Sdp_DecodeSF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_SF *pstSessName);
ZFUNC ZINT Sdp_DecodeIF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_IF *pstInfo);
ZFUNC ZINT Sdp_DecodeUF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_UF *pstUri);
ZFUNC ZINT Sdp_DecodeEFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_EF_LST *pstEmailLst);
ZFUNC ZINT Sdp_DecodeEF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_EF *pstEmail);
ZFUNC ZINT Sdp_DecodePFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PF_LST *pstPhoneLst);
ZFUNC ZINT Sdp_DecodePF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_PF *pstPhone);
ZFUNC ZINT Sdp_DecodeCFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CF_LST *pstConnLst);
ZFUNC ZINT Sdp_DecodeCF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_CF *pstConn);
ZFUNC ZINT Sdp_DecodeBFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_BF_LST *pstBwLst);
ZFUNC ZINT Sdp_DecodeBF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_BF *pstBw);
ZFUNC ZINT Sdp_DecodeSessTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SESS_TIME *pstSessTime);
ZFUNC ZINT Sdp_DecodeTFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TF_LST *pstTimeLst);
ZFUNC ZINT Sdp_DecodeTF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_TF *pstTime);
ZFUNC ZINT Sdp_DecodeKF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_KF *pstKey);
ZFUNC ZINT Sdp_DecodeAFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_AF_LST *pstAttrLst);
ZFUNC ZINT Sdp_DecodeAF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_AF *pstAttr);
ZFUNC ZINT Sdp_DecodeMdescLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MDESC_LST *pstMdescLst);
ZFUNC ZINT Sdp_DecodeMdesc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MDESC *pstMdesc);
ZFUNC ZINT Sdp_DecodeMF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_MF *pstMedia);
ZFUNC ZINT Sdp_DecodeRFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RF_LST *pstRepLst);
ZFUNC ZINT Sdp_DecodeRF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_RF *pstRep);
ZFUNC ZINT Sdp_DecodeZFLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ZF_LST *pstZoneAdjLst);
ZFUNC ZINT Sdp_DecodeZF(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_ZF *pstZoneAdj);

ZFUNC ZINT Sdp_DecodeUcastAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_UCAST_ADDR *pstAddr);
ZFUNC ZINT Sdp_DecodeConnAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CONN_ADDR *pstConnAddr);
ZFUNC ZINT Sdp_DecodeFmtLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMT_LST *pstFmtLst);
ZFUNC ZINT Sdp_DecodeFmt(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_FMT *pstFmt);
ZFUNC ZINT Sdp_DecodeTypedTimeLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPED_TIME_LST *pstTypedTimeLst);
ZFUNC ZINT Sdp_DecodeTypedTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPED_TIME *pstTypedTime);
ZFUNC ZINT Sdp_DecodeAttrStr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_STR *pstAttrStr);
ZFUNC ZINT Sdp_DecodeIp4Mcast(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCAST_IP4 *pstIp4Mcast);
ZFUNC ZINT Sdp_DecodeIp6Mcast(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCAST_IP6 *pstIp6Mcast);
ZFUNC ZINT Sdp_DecodeAttrConf(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_CONF *pstConf);
ZFUNC ZINT Sdp_DecodeFmtp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP *pstFmtp);
ZFUNC ZINT Sdp_DecodeFmt2733(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_2733 *pst2733);
ZFUNC ZINT Sdp_DecodeFmt2833(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_2833 *pst2833);
ZFUNC ZINT Sdp_DecodeFmt3640(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3640 *pst3640);
ZFUNC ZINT Sdp_DecodeFmt3952(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3952 *pst3952);
ZFUNC ZINT Sdp_DecodeFmtH26X(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_H26X *pstH26x);
ZFUNC ZINT Sdp_DecodeFmt4587(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_4587 *pst4587);
ZFUNC ZINT Sdp_DecodeFmt4629(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_4629 *pst4629);
ZFUNC ZINT Sdp_DecodeFmt3984(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3984 *pst3984);
ZFUNC ZINT Sdp_DecodeFmt3267(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_3267 *pst3267);
ZFUNC ZINT Sdp_DecodeFmtTbcp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_TBCP *pstTbcp);
ZFUNC ZINT Sdp_DecodeFmt2733Red(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_2733_RED *pst2733Red);
ZFUNC ZINT Sdp_DecodeFmt2733Fec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_2733_FEC *pst2733Fec);
ZFUNC ZINT Sdp_DecodeFmt4588(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FMTP_4588 *pst4588);
ZFUNC ZINT Sdp_Decode2833ValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_2833_VAL_LST *pstValLst);
ZFUNC ZINT Sdp_Decode2833Val(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_2833_VAL *pstVal);
ZFUNC ZINT Sdp_Decode3640ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3640_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Decode3640Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3640_PARM *pstParm);
ZFUNC ZINT Sdp_DecodeH261OptLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H261_OPT_LST *pstH261OptLst);
ZFUNC ZINT Sdp_DecodeH261Opt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H261_OPT *pstH261Opt);
ZFUNC ZINT Sdp_DecodeH263OptLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_OPT_LST *pstH263OptLst);
ZFUNC ZINT Sdp_DecodeH263Opt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_OPT *pstH261Opt);
ZFUNC ZINT Sdp_DecodeH263Size(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_SIZE *pstH261Size);
ZFUNC ZINT Sdp_DecodeH263Anx(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_ANX *pstH261Anx);
ZFUNC ZINT Sdp_DecodeH263Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_H263_PARM *pstParm);
ZFUNC ZINT Sdp_Decode4587ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4587_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Decode4587Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4587_PARM *pstParm);
ZFUNC ZINT Sdp_Decode4629ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4629_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Decode4629Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4629_PARM *pstParm);
ZFUNC ZINT Sdp_Decode4629Custom(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_4629_CUSTOM *pstCustom);
ZFUNC ZINT Sdp_Decode3984ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3984_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Decode3984Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3984_PARM *pstParm);
ZFUNC ZINT Sdp_Decode3267ParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3267_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_Decode3267Parm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_3267_PARM *pstParm);
ZFUNC ZINT Sdp_DecodeTbcpParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TBCP_PARM_LST *pstParmLst);
ZFUNC ZINT Sdp_DecodeTbcpParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TBCP_PARM *pstParm);
ZFUNC ZINT Sdp_DecodeRtpmap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTPMAP *pstRtpMap);
ZFUNC ZINT Sdp_DecodeCurSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CUR_STA *pstCurSta);
ZFUNC ZINT Sdp_DecodeDesSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DES_STA *pstDesSta);
ZFUNC ZINT Sdp_DecodeCnfSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CNF_STA *pstCnfSta);
ZFUNC ZINT Sdp_DecodeRtcp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTCP *pstRtcp);
ZFUNC ZINT Sdp_DecodeMid(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MID *pstMid);
ZFUNC ZINT Sdp_DecodeGrp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_GRP *pstGrp);
ZFUNC ZINT Sdp_DecodeClir(ST_ABNF_MSG *pstAbnfMsg, ZBOOL *pbClir);
ZFUNC ZINT Sdp_DecodeAttrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_LST *pstAttrLst);
ZFUNC ZINT Sdp_DecodeAttr(ST_ABNF_MSG *pstAbnfMsg, ST_SDP_ATTR *pstAttr);
ZFUNC ZINT Sdp_DecodeIdTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ID_TAG_LST *pstIdTagLst);
ZFUNC ZINT Sdp_DecodeIdTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ID_TAG *pstIdTag);
ZFUNC ZINT Sdp_DecodeSsrc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SSRC *pstSsrc);
ZFUNC ZINT Sdp_DecodeSsrcGrp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SSRC_GRP *pstSsrcGrp);
ZFUNC ZINT Sdp_DecodeSsrcIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SSRC_ID_LST *pstSsrcIdLst);
ZFUNC ZINT Sdp_DecodeContent(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CNT *pstContent);
ZFUNC ZINT Sdp_DecodeMcntLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCNT_LST *pstMcntLst);
ZFUNC ZINT Sdp_DecodeMcnt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MCNT *pstContent);
ZFUNC ZINT Sdp_DecodeAttValue(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_COPT *pstCOpt);
ZFUNC ZINT Sdp_DecodeCOptLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_C_OPT_LST *pstCOptLst);
ZFUNC ZINT Sdp_DecodeAcap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ACAP *pstAcap);
ZFUNC ZINT Sdp_DecodeTcap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TCAP *pstTcap);
ZFUNC ZINT Sdp_DecodeProtoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PROTO_LST *pstProtoLst);
ZFUNC ZINT Sdp_DecodePcfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PCFG *pstPcfg);
ZFUNC ZINT Sdp_DecodePotCfgLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_POT_CFG_LST *pstPotCfgLst);
ZFUNC ZINT Sdp_DecodePotCfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_POT_CFG *pstPotCfg);
ZFUNC ZINT Sdp_DecodeTransProCfgLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TRANS_PRO_CFG_LST *pstTransProCfgLst);
ZFUNC ZINT Sdp_DecodeAttrCfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATTR_CFG_LST *pstAttrCfg);
ZFUNC ZINT Sdp_DecodeMaClLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MA_CL_LST *pstMaClLst);
ZFUNC ZINT Sdp_DecodeMaCl(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MA_CL *pstMaCl);
ZFUNC ZINT Sdp_DecodeAttCapLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ATT_CAP_LST *pstAttCapLst);
ZFUNC ZINT Sdp_DecodeAcfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ACFG *pstAcfg);
ZFUNC ZINT Sdp_DecodeSelCfgLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SEL_CFG_LST *pstSelCfgLst);
ZFUNC ZINT Sdp_DecodeSelCfg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SEL_CFG *pstSelCfg);
ZFUNC ZINT Sdp_DecodeAcptTypes(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ACPT_TYPES *pstAcptTypes);
ZFUNC ZINT Sdp_DecodeFormatLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FORMAT_LST *pstFormatLst);
ZFUNC ZINT Sdp_DecodeFormat(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FORMAT *pstFormat);
ZFUNC ZINT Sdp_DecodeFormatType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FORMAT_TYPE *pstFormatType);
ZFUNC ZINT Sdp_DecodeTypeParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPE_PARM_LST *pstTypeParmLst);
ZFUNC ZINT Sdp_DecodeTypeParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TYPE_PARM *pstTypeParm);
ZFUNC ZINT Sdp_DecodePval(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PVAL *pstPval);
ZFUNC ZINT Sdp_DecodeQStr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQStr);
ZFUNC ZINT Sdp_DecodePath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_PATH *pstPath);
ZFUNC ZINT Sdp_DecodeMsrpUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MSRP_URI_LST *pstMsrpUriLst);
ZFUNC ZINT Sdp_DecodeMsrpUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_MSRP_URI *pstMsrpUri);
ZFUNC ZINT Sdp_DecodeAuthority(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_AUTHORITY *pstAuthority);
ZFUNC ZINT Sdp_DecodeUserInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstUserInfo);
ZFUNC ZINT Sdp_DecodeHost(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_HOST *pstHost);
ZFUNC ZINT Sdp_DecodeIpv6(ST_ABNF_MSG *pstAbnfMsg, ZUCHAR *pucIpv6Addr);
ZFUNC ZINT Sdp_DecodeIpvFuture(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IPV_FUTURE *pstIpvFuture);
ZFUNC ZINT Sdp_DecodeTrans(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TRANS *pstTrans);
ZFUNC ZINT Sdp_DecodeUriParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_URI_PARM_LST *pstUriParmLst);
ZFUNC ZINT Sdp_DecodeUriParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_URI_PARM *pstUriParm);
ZFUNC ZINT Sdp_DecodeFileSlt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_SLT *pstFileSlt);
ZFUNC ZINT Sdp_DecodeSelectorLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_LST *pstSelectorLst);
ZFUNC ZINT Sdp_DecodeSelector(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR *pstSelector);
ZFUNC ZINT Sdp_DecodeFileName(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_FNAME *pstFileName);
ZFUNC ZINT Sdp_DecodeFileType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_FTYPE *pstFileType);
ZFUNC ZINT Sdp_DecodeFileHash(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SELECTOR_HASH *pstFileHash);
ZFUNC ZINT Sdp_DecodeFtParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FTPARM_LST *pstFtParmLst);
ZFUNC ZINT Sdp_DecodeFtParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FTPARM *pstFtParm);
ZFUNC ZINT Sdp_DecodeHashValueLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_HASH_VALUE_LST *pstHashValueLst);
ZFUNC ZINT Sdp_DecodeFileDate(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_DATE *pstFileDate);
ZFUNC ZINT Sdp_DecodeDateParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE_PARM_LST *pstDateParmLst);
ZFUNC ZINT Sdp_DecodeDateParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE_PARM *pstDateParm);
ZFUNC ZINT Sdp_DecodeDateTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE_TIME *pstDate);
ZFUNC ZINT Sdp_DecodeDayOfWeek(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DAY_OF_WEEK *pstDayOfWeek);
ZFUNC ZINT Sdp_DecodeDate(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DATE *pstDate);
ZFUNC ZINT Sdp_DecodeDayYear(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_DAY_YEAR *pstDay);
ZFUNC ZINT Sdp_DecodeTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TIME *pstTime);
ZFUNC ZINT Sdp_DecodeTimeOfDay(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_TIME_OF_DAY *pstTimeOfDay);
ZFUNC ZINT Sdp_DecodeZone(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ZONE *pstZone);
ZFUNC ZINT Sdp_DecodeCfws(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CFWS *pstCfws);
ZFUNC ZINT Sdp_DecodeCommLst(ST_ABNF_MSG *pstAbnfMsg,
                    ST_SDP_COMM_LST *pstCommLst);
ZFUNC ZINT Sdp_DecodeComm(ST_ABNF_MSG *pstAbnfMsg,
                    ST_SDP_COMM *pstComm);
ZFUNC ZINT Sdp_DecodeCConLst(ST_ABNF_MSG *pstAbnfMsg,
                    ST_SDP_CCON_LST *pstCConLst);
ZFUNC ZINT Sdp_DecodeFws(ST_ABNF_MSG *pstAbnfMsg);
ZFUNC ZINT Sdp_DecodeFileIcon(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_ICON *pstFileIcon);
ZFUNC ZINT Sdp_DecodeCidUrl(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CID_URL *pstCidUrl);
ZFUNC ZINT Sdp_DecodeAddrSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_ADDR_SPEC *pstAddrSpec);
ZFUNC ZINT Sdp_DecodeWordLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_WORD_LST *pstWordLst);
ZFUNC ZINT Sdp_DecodeWord(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_WORD *pstWord);
ZFUNC ZINT Sdp_DecodeSDomainLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SDOMAIN_LST *pstSDomainLst);
ZFUNC ZINT Sdp_DecodeSDomain(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SDOMAIN *pstSDomain);
ZFUNC ZINT Sdp_DecodeDLiteral(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstDLiteral);
ZFUNC ZINT Sdp_DecodeQDTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQDTime);
ZFUNC ZINT Sdp_DecodeFileRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FILE_RANGE *pstFileRange);
ZFUNC ZINT Sdp_DecodeConnection(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CONNECTION *pstConn);
ZFUNC ZINT Sdp_DecodeSetup(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_SETUP *pstSetup);
ZFUNC ZINT Sdp_DecodeFramesize(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FRAMESIZE *pstFramesize);
ZFUNC ZINT Sdp_DecodeMatchField(ST_ABNF_MSG *pstAbnfMsg, ZCHAR chr);
ZFUNC ZINT Sdp_DecodeImgAttr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR *pstImgAttr);
ZFUNC ZINT Sdp_DecodeImgAttrSetLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_SET_LST *pstSetLst);
ZFUNC ZINT Sdp_DecodeImgAttrSet(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_SET *pstSet);
ZFUNC ZINT Sdp_DecodeImgAttrRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_RANGE *pstRange);
ZFUNC ZINT Sdp_DecodeImgAttrSar(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_SRANGE *pstSar);
ZFUNC ZINT Sdp_DecodeImgAttrPar(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_PRANGE *pstPar);
ZFUNC ZINT Sdp_DecodeImgAttrXrange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_IMG_ATTR_XRANGE *pstXrange);
ZFUNC ZINT Sdp_DecodeImgAttrFloat(ST_ABNF_MSG *pstAbnfMsg, ZFLOAT *pfValue);
ZFUNC ZINT Sdp_DecodeRtcpFb(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTCP_FB *pstRtcpFb);
ZFUNC ZINT Sdp_DecodeRtcpFbParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RTCP_FB_PARM *pstParm);
ZFUNC ZINT Sdp_DecodeCand(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CAND *pstCand);
ZFUNC ZINT Sdp_DecodeRmtCands(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_RMT_CAND_LST *pstRmtCandLst);
ZFUNC ZINT Sdp_DecodeIceUfrag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstIceUfrag);
ZFUNC ZINT Sdp_DecodeIcePwd(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstIcePwd);
ZFUNC ZINT Sdp_DecodeIceOpts(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR_LST *pstIceOptLst);
ZFUNC ZINT Sdp_DecodeFgrprnt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_FGRPRNT *pstFgrprnt);
ZFUNC ZINT Sdp_DecodeCrypto (ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO*pstCrypto);
ZFUNC ZINT Sdp_DecodeCryptoKeyParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_KEY_PARM_LST *pstKeyParmLst);
ZFUNC ZINT Sdp_DecodeCryptoKeyParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_KEY_PARM *pstKeyParam);
ZFUNC ZINT Sdp_DecodeCryptoSrtpInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_STRP_KEY_INFO *pstKeyInfo);
ZFUNC ZINT Sdp_DecodeCryptoSessParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_SESS_PARM_LST *pstSessionParmLst);
ZFUNC ZINT Sdp_DecodeCryptoSessParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_CRYPTO_SESS_PARM *pstParm);
ZFUNC ZINT Sdp_DecodeExtMap(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SDP_EXTMAP *pstExtMap);

#ifdef __cplusplus
}
#endif

#endif /* _SDP_ABNF_DECODE_H__ */


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
  File name     : abnf_util.h
  Module        : abnf utility library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-12-29
  Description   :
      Data structure and function definitions required by the 
      abnf utility interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ABNF_UTIL_H__
#define _ABNF_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* abnf alloc memory of specific size */
#define ABNF_ALLOC(_membuf, _size) \
    Zos_UbufAllocClrd(_membuf, _size)
    
/* abnf free memory of specific address */
#define ABNF_FREE(_membuf, _mem) \
    Zos_UbufFree(_membuf, _mem)

/* abnf alloc memory of specific type */
#define ABNF_ALLOC_ELEM(_membuf, _type) \
    ABNF_ALLOC(_membuf, sizeof(_type))

/* typedef abnf list */
typedef ST_ZOS_DLIST ST_ABNF_LIST;

/* typedef abnf list node */
typedef ST_ZOS_DLIST_NODE ST_ABNF_LIST_NODE;

/* abnf message any element decode function */
typedef ZINT (*PFN_ABNFDECODE)(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pAnyElem); 

/* abnf message any element encode function */
typedef ZINT (*PFN_ABNFENCODE)(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pAnyElem); 

/** abnf copy function type defination. */
typedef ZINT (*PFN_ABNFANYCPY)(ZUBUF zBufId, ZVOID *pstDst, ZVOID *pstSrc);

/* abnf list use the slist */
#define ABNF_TYPEDEF_LIST(_name) \
    ZOS_TYPEDEF_DLIST(_name)

/* abnf list macros */
#define ABNF_LIST_SIZE(_list)  \
    ZOS_TYPEDEF_DLIST_SIZE(_list)
    
#define ABNF_LIST_ISFULL(_list)  \
    ZOS_TYPEDEF_DLIST_ISFULL(_list)
    
#define ABNF_LIST_ISEMPTY(_list) \
    ZOS_TYPEDEF_DLIST_ISEMPTY(_list)

#define ABNF_LIST_ISNOEMPTY(_list) \
    ZOS_TYPEDEF_DLIST_ISNOEMPTY(_list)

#define ABNF_LIST_HEAD_NODE(_list) \
    ZOS_TYPEDEF_DLIST_HEAD_NODE(_list)
    
#define ABNF_LIST_TAIL_NODE(_list) \
    ZOS_TYPEDEF_DLIST_TAIL_NODE(_list)

#define ABNF_LIST_PREV_NODE(_node) \
    ZOS_TYPEDEF_DLIST_PREV_NODE(_node)

#define ABNF_LIST_NEXT_NODE(_node) \
    ZOS_TYPEDEF_DLIST_NEXT_NODE(_node)
    
#define ABNF_LIST_GET_DATA(_node) \
    ZOS_TYPEDEF_DLIST_GET_DATA(_node)

#define ABNF_LIST_HEAD_DATA(_list) \
    ZOS_TYPEDEF_DLIST_HEAD_DATA(_list)

#define ABNF_LIST_TAIL_DATA(_list) \
    ZOS_TYPEDEF_DLIST_TAIL_DATA(_list)

#define ABNF_LIST_PREV_DATA(_node) \
    ZOS_TYPEDEF_DLIST_PREV_DATA(_node)

#define ABNF_LIST_NEXT_DATA(_node) \
    ZOS_TYPEDEF_DLIST_NEXT_DATA(_node)

#define FOR_ALL_NODE_IN_ABNF_LIST(_list, _node) \
    FOR_ALL_NODE_IN_TYPEDEF_DLIST(_list, _node)

#define FOR_ALL_NODE_IN_ABNF_LISTX(_list, _node) \
    FOR_ALL_NODE_IN_TYPEDEF_DLISTX(_list, _node)

#define FOR_ALL_DATA_IN_ABNF_LIST(_list, _node, _data) \
    FOR_ALL_DATA_IN_TYPEDEF_DLIST(_list, _node, _data)

#define FOR_ALL_DATA_IN_ABNF_LISTX(_list, _node, _data) \
    FOR_ALL_DATA_IN_TYPEDEF_DLISTX(_list, _node, _data)

/* abnf list create */
#define ABNF_LIST_CREATE(_list) \
    ZOS_TYPEDEF_DLIST_CREATEX(_list)

/* abnf list delete */
#define ABNF_LIST_DELETE(_list) \
    ZOS_TYPEDEF_DLIST_DELETE(_list)

/* abnf list node init */
#define ABNF_LIST_NODE_INIT(_node, _data) \
    ZOS_TYPEDEF_DLIST_NODE_INIT(_node, _data)

/* abnf insert node before head in abnf list */
#define ABNF_LIST_ADD2HEAD(_list, _node) \
    ZOS_TYPEDEF_DLIST_ADD2HEAD(_list, _node)

/* abnf insert node after tail in abnf list */
#define ABNF_LIST_ADD2TAIL(_list, _node) \
    ZOS_TYPEDEF_DLIST_ADD2TAIL(_list, _node)

/* abnf insert node after previou node in abnf list */
#define ABNF_LIST_INSERT(_list, _prevnode, _node) \
    ZOS_TYPEDEF_DLIST_INSERT(_list, _prevnode, _node)

/* abnf dequeue node from the first node in abnf list */
#define ABNF_LIST_DEQUEUE(_list, _node) \
   ZOS_TYPEDEF_DLIST_DEQUEUE(_list, _node)

/* abnf remove one node */
#define ABNF_LIST_REMOVE(_list, _node) \
    ZOS_TYPEDEF_DLIST_REMOVE(_list, _node)

/* abnf get list node from data address */
#define ABNF_LIST_DATA_NODE(_data) \
    ((ST_ZOS_DLIST_NODE *)(_data) - 1)

/* abnf alloc memory of one abnf list node */
#define ABNF_LIST_ALLOC_DATA(_membuf, _type, _data) \
    Abnf_ListAllocData(_membuf, sizeof(_type), (ZVOID **)&_data)

/* abnf alloc memory of one abnf list node */
#define ABNF_LIST_ALLOC_DATAX(_membuf, _size, _data) \
    Abnf_ListAllocData(_membuf, _size, (ZVOID **)&_data)

/* abnf list append one node(not alloced) with data */
#define ABNF_LIST_APPEND_DATA(_membuf, _list, _data) \
    Abnf_ListAppendData(_membuf, (ST_ABNF_LIST *)_list, _data)

/* abnf list append one node(alloced) with data */
#define ABNF_LIST_APPEND_NODE(_list, _data) \
    ABNF_LIST_ADD2TAIL(_list, (ST_ZOS_DLIST_NODE *)(_data) - 1)

/* abnf list insert one node(alloced) with data */
#define ABNF_LIST_INSERT_NODE(_list, _data) \
    ABNF_LIST_ADD2HEAD(_list, (ST_ZOS_DLIST_NODE *)(_data) - 1)

/* abnf remove one node(known by data) */
#define ABNF_LIST_REMOVE_DATA(_list, _data) \
    ABNF_LIST_REMOVE(_list, (ST_ZOS_DLIST_NODE *)(_data) - 1)
    
/* abnf remove one node(free data)(known by data) */
#define ABNF_LIST_REMOVE_DATAX(_membuf, _list, _data) \
    Abnf_ListRemoveData(_list, (ST_ABNF_LIST *)_list, _data)

/* abnf list find node data on index location */
#define ABNF_LIST_FIND_BY_INDEX(_list, _index, _node, _data) do { \
    ZOS_TYPEDEF_DLIST_FIND_BY_INDEX(_list, _index, _node); \
    _data = (_node) ? (_node)->pData : ZNULL; \
} while (0)

/*lint -save -e* */

/* abnf set use LWSP as valid character in the string */
#define ABNF_SET_USELWSP(_msg) (_msg)->stChr.ucIsUseLwsp = ZTRUE
    
/* abnf set use WSP as valid character in the string */
#define ABNF_SET_USEWSP(_msg) (_msg)->stChr.ucIsUseWsp = ZTRUE

/*lint -restore */

/* abnf set the escaped chrset info */
#define ABNF_SET_ESCAPECHR(_msg, _escapechr, _escapeclass) do { \
     (_msg)->stChr.ucEscapeChr = _escapechr; \
     (_msg)->stChr.iEscapeClass = _escapeclass; \
} while (0)

/* abnf set the chrset info */
#define ABNF_SET_CHRSET(_msg, _chrsetid, _chrclass) do { \
     (_msg)->stChr.iClass = _chrclass; \
     (_msg)->stChr.zChrset = _chrsetid; \
} while (0)

/* abnf set the string length */
#define ABNF_SET_CHRLEN(_msg, _minlen, _maxlen) do { \
     (_msg)->stChr.iMinLen = _minlen; \
     (_msg)->stChr.iMaxLen = _maxlen; \
} while (0)

/* abnf set the string length */
#define ABNF_SET_CHRVAL(_msg, _minval, _maxval) do { \
     (_msg)->stChr.dwMinVal = _minval; \
     (_msg)->stChr.dwMaxVal = _maxval; \
} while (0)

/* abnf set the token info */
#define ABNF_SET_CHRTKN(_msg, _tknmgrid, _tkntype) do { \
     (_msg)->stChr.iTknType = _tkntype; \
     (_msg)->stChr.zTknMgr = _tknmgrid; \
} while (0)

/* abnf set the separators */
#define ABNF_SET_SEPAS(_msg, _sepa1, _sepa2, _sepa3, _sepa4) do { \
     (_msg)->stSepa.ucPres = ZTRUE; \
     (_msg)->stSepa.aucSepa[0] = _sepa1; \
     (_msg)->stSepa.aucSepa[1] = _sepa2; \
     (_msg)->stSepa.aucSepa[2] = _sepa3; \
     (_msg)->stSepa.aucSepa[3] = _sepa4; \
} while (0)

/* abnf set the extend parameters */
#define ABNF_SET_EXTN_PARMS(_msg, _parm1, _parm2, _parm3) do { \
     (_msg)->stExtn.aiParm[0] = _parm1; \
     (_msg)->stExtn.aiParm[1] = _parm2; \
     (_msg)->stExtn.aiParm[2] = _parm3; \
} while (0)

/* abnf clear property of character on LWSP */
#define ABNF_CLR_USELWSP(_msg) \
    (_msg)->stChr.ucIsUseLwsp = ZFALSE

/* abnf clear property of character on LWSP */
#define ABNF_CLR_USEWSP(_msg) \
    (_msg)->stChr.ucIsUseWsp = ZFALSE

/* abnf clear property of character on ESCAPE */
#define ABNF_CLR_ESCAPECHR(_msg) do { \
    (_msg)->stChr.ucEscapeChr = EN_CHR_EOS; \
    (_msg)->stChr.iEscapeClass = ABNF_CHRCLASS_NA; \
} while (0)

/* abnf clear property of character on character set */
#define ABNF_CLR_CHRSET(_msg) do { \
    (_msg)->stChr.iClass = ABNF_CHRCLASS_NA; \
    (_msg)->stChr.zChrset = ABNF_CHRSETID_NA; \
} while (0)

/* abnf clear property of character on length */
#define ABNF_CLR_CHRLEN(_msg) do { \
    (_msg)->stChr.iMinLen = ABNF_MIN_LEN_NA; \
    (_msg)->stChr.iMaxLen = ABNF_MAX_LEN_NA; \
} while (0)
    
/* abnf clear property of character on value */
#define ABNF_CLR_CHRVAL(_msg) do { \
    (_msg)->stChr.iMinVal = ABNF_MIN_VAL_NA; \
    (_msg)->stChr.iMaxVal = ABNF_MAX_VAL_NA; \
} while (0)
    
/* abnf clear property of character on token */
#define ABNF_CLR_CHRTKN(_msg) do { \
     (_msg)->stChr.iTknType = 0; \
     (_msg)->stChr.zTknMgr = ZNULL; \
} while (0)

/* abnf clear property of separator */
#define ABNF_CLR_SEPAS(_msg) do { \
    (_msg)->stSepa.ucPres = ZFALSE; \
    (_msg)->stSepa.aucSepa[0] = 0; \
    (_msg)->stSepa.aucSepa[1] = 0; \
    (_msg)->stSepa.aucSepa[2] = 0; \
    (_msg)->stSepa.aucSepa[3] = 0; \
} while (0)

/* abnf clear property of extend parameters */
#define ABNF_CLR_EXTN_PARMS(_msg) do { \
     (_msg)->stExtn.aiParm[0] = 0; \
     (_msg)->stExtn.aiParm[1] = 0; \
     (_msg)->stExtn.aiParm[2] = 0; \
} while (0)

/* abnf list item decode */
#define ABNF_ANY_LST_ITEM_DECODE(_msg, _decode, _type, _parmlst) \
    Abnf_AnyLstItemDecode(_msg, _parmlst, sizeof(_type), \
                          (PFN_ABNFDECODE)_decode)

/* abnf any list encode 
   Syntax: [_sepab] parm <_sepa> parm [_sepae] 
   Example: slides, speaker, main <-- _sepab = _sepae = ZFALSE, _sepa = ',' */
#define ABNF_ANY_LST_ENCODE(_msg, _parmlst, _sepab, _sepae, _sepa, _encode) \
    Abnf_AnyLstEncode(_msg, _parmlst, _sepab, _sepae, _sepa, \
                      (PFN_ABNFENCODE)_encode)

/* abnf any list encode */
#define ABNF_ANY_LST_ENCODEX(_msg, _parmlst, _sepab, _sepae, _sepa, _encode) \
    Abnf_AnyLstEncodeX(_msg, _parmlst, _sepab, _sepae, _sepa, \
                      (PFN_ABNFENCODE)_encode)

/* abnf get the last data */
#define ABNF_GET_LASTDATA(_msg) \
    (!(_msg)->stBuf.iCurLen)

/* abnf get the last separator */
#define ABNF_GET_LASTSEPA(_msg) \
    (_msg)->stSepa.ucLastSepa

/* abnf get the scanned string */
#define ABNF_GET_SCANNED_STR \
    Abnf_GetScannedStr

/* abnf get the scanning(the left message to be scanned) string */
#define ABNF_GET_SCANNING_STR \
    Abnf_GetScanningStr

/* abnf save buffer state */
#define ABNF_SAVE_BUF_STATE \
    Abnf_SaveBufState

/* abnf restore buffer state */
#define ABNF_RESTORE_BUF_STATE \
    Abnf_RestoreBufState

/* abnf convert string to ipv4 address */
#define ABNF_STR2IPV4(_str, _ipv4) \
    Abnf_NStr2Ipv4(ZOS_NSTR(_str), _ipv4)

/* abnf convert structure string to ipv4 address */
#define ABNF_SSTR2IPV4(_sstr, _ipv4) \
    Abnf_NStr2Ipv4(ZOS_NSSTR_SAFE(_sstr), _ipv4)

/* abnf check string is ipv4 address */
#define ABNF_STRISIPV4(_str) \
    Abnf_NStrIsIpv4(ZOS_NSTR(_str))

/* abnf check structure string is ipv4 address */
#define ABNF_SSTRISIPV4(_sstr) \
    Abnf_NStrIsIpv4(ZOS_NSSTR_SAFE(_sstr))

/* abnf convert string to ipv6 address */
#define ABNF_STR2IPV6(_str, _ipv6) \
    Abnf_NStr2Ipv6(ZOS_NSTR(_str), _ipv6)

/* abnf convert structure string to ipv6 address */
#define ABNF_SSTR2IPV6(_sstr, _ipv6) \
    Abnf_NStr2Ipv6(ZOS_NSSTR_SAFE(_sstr), _ipv6)

/* abnf check string is ipv6 address */
#define ABNF_STRISIPV6(_str) \
    Abnf_NStrIsIpv6(ZOS_NSTR(_str))

/* abnf check structure string is ipv6 address */
#define ABNF_SSTRISIPV6(_sstr) \
    Abnf_NStrIsIpv6(ZOS_NSSTR_SAFE(_sstr))

/* abnf convert structure percent-encoded string to normal string */
#define ABNF_SSTRP2N(_psstr, _nsstr) \
    Abnf_StrP2N(ZOS_NSSTR_SAFE(_psstr), (_nsstr)->pcStr, &(_nsstr)->iLen)

/* abnf convert structure normal string to percent-encoded  string */
#define ABNF_SSTRN2P(_nsstr, _chrset, _class, _psstr) \
    Abnf_StrN2P(ZOS_NSSTR_SAFE(_nsstr), _chrset, _class, \
                (_psstr)->pcStr, &(_psstr)->iLen)

/* abnf init the message, porotocol see ZPROTOCOL_SDP... */
ZFUNC ZINT Abnf_MsgInit(ST_ABNF_MSG *pstMsg, ZUINT iProto, ST_ZOS_SSTR *pstStr, 
                ZDBUF zMemBuf, ST_ABNF_ERR_INFO *pstErrInfo, ZBOOL bIsDecode);

/* abnf init the message, porotocol see ZPROTOCOL_SDP... */
ZFUNC ZINT Abnf_MsgInitL(ST_ABNF_MSG *pstMsg, ZUINT iProto, 
                ST_ZOS_SSTR *pstStr, ZDBUF zMemBuf, 
                ST_ABNF_ERR_INFO *pstErrInfo, ZBOOL bIsDecode);

/* abnf init the message, porotocol see ZPROTOCOL_SDP... */
ZFUNC ZINT Abnf_MsgInitX(ST_ABNF_MSG *pstMsg, ZUINT iProto, ZCHAR *pcStr, 
                ZDBUF zMemBuf, ZBOOL bIsDecode);

/* abnf save the buffer state */
ZFUNC ZINT Abnf_SaveBufState(ST_ABNF_MSG *pstMsg, 
                ST_ABNF_BUF_INFO *pstSavedBuf);

/* abnf restore the buffer state */
ZFUNC ZINT Abnf_RestoreBufState(ST_ABNF_MSG *pstMsg, 
                ST_ABNF_BUF_INFO *pstSavedBuf);

/* abnf move the buffer state */
ZFUNC ZINT Abnf_MoveBufState(ST_ABNF_MSG *pstMsg, ZINT iMoveLen);

/* abnf reset the buffer state */
ZFUNC ZINT Abnf_ResetBufState(ST_ABNF_MSG *pstMsg);

/* abnf list alloc data */
ZFUNC ZINT Abnf_ListAllocData(ZUBUF zMemBuf, ZINT iSize, ZVOID **ppData);

/* abnf list append data */
ZFUNC ZINT Abnf_ListAppendData(ZUBUF zMemBuf, ST_ABNF_LIST *pstList, 
                ZVOID *pData);

/* abnf list remove data */
ZFUNC ZINT Abnf_ListRemoveData(ZUBUF zMemBuf, ST_ABNF_LIST *pstList, 
                ZVOID *pData);

/* abnf convert string with length to unsigned long value */
ZFUNC ZINT Abnf_NStr2UiDigit(ZCHAR *pcStr, ZUINT iLen, ZUINT *piData);

/* abnf convert string with length to unsigned short value */
ZFUNC ZINT Abnf_NStr2UsDigit(ZCHAR *pcStr, ZUINT iLen, ZUSHORT *pwData);

/* abnf convert string with length to unsigned char value */
ZFUNC ZINT Abnf_NStr2UcDigit(ZCHAR *pcStr, ZUINT iLen, ZUCHAR *pucData);

/* abnf convert string with length to hexadecimal value */
ZFUNC ZINT Abnf_NStr2XUiDigit(ZCHAR *pcStr, ZUINT iLen, ZUINT *piData);

/* abnf convert string with length to ipv4 address */
ZFUNC ZINT Abnf_NStr2Ipv4(ZCHAR *pcStr, ZUSHORT wLen, ZUINT *piAddr);

/* abnf convert string with length to ipv6 address */
ZFUNC ZINT Abnf_NStr2Ipv6(ZCHAR *pcStr, ZUSHORT wLen, ZUCHAR *pucAddr);

/* abnf check string with length is ipv4 address */
ZFUNC ZBOOL Abnf_NStrIsIpv4(ZCONST ZCHAR *pcStr, ZUSHORT wLen);

/* abnf check string with length is ipv6 address */
ZFUNC ZBOOL Abnf_NStrIsIpv6(ZCONST ZCHAR *pcStr, ZUSHORT wLen);

/* abnf convert percent-encoded string to normal string */ 
ZFUNC ZINT Abnf_StrP2N(ZCHAR *pcPStr, ZUINT iPLen, 
                ZCHAR *pcNStr, ZUINT *piNStrSize);

/* abnf convert normal string to percent-encoded string */ 
ZFUNC ZINT Abnf_StrN2P(ZCHAR *pcNStr, ZUINT iNLen, ZCHRSET zChrset,
                ZUINT iClass, ZCHAR *pcPStr, ZUINT *piPStrSize);

/* abnf get size of convert string percent-encoded to normal string */ 
ZFUNC ZINT Abnf_SizeP2N(ZCHAR *pcPStr, ZUINT iPLen, ZUINT *piStrSize);

/* abnf get size of convert normal string to percent-encoded string */ 
ZFUNC ZINT Abnf_SizeN2P(ZCHAR *pcNStr, ZUINT iNLen, ZCHRSET zChrset,
                ZUINT iClass, ZUINT *piStrSize);

/* abnf convert hexadecimal character to digit */
ZFUNC ZINT Abnf_XChr2Digit(ZCHAR cXChr, ZINT *piDigit);

/* abnf message any element decode, 
   user should ensure pAnyElem memory is valid and clear */
ZFUNC ZINT Abnf_AnyDecode(ZUINT iProtocol, PFN_ABNFDECODE pfnDecode,
                ST_ZOS_SSTR *pstStr, ZDBUF zMemBuf, ZVOID *pAnyElem);

/* abnf message any element decode, 
   user should ensure pAnyElem memory is valid and clear */
ZFUNC ZINT Abnf_AnyDecodeL(ZUINT iProtocol, PFN_ABNFDECODE pfnDecode,
                ST_ZOS_SSTR * pstStr, ZDBUF zMemBuf, ZVOID * pAnyElem);

/* abnf message any element decode, 
   user should ensure pAnyElem memory is valid and clear */
ZFUNC ZINT Abnf_AnyDecodeX(ZUINT iProtocol, PFN_ABNFDECODE pfnDecode,
                ST_ZOS_SSTR *pstStr, ZDBUF *pzMemBuf, ZVOID *pAnyElem);

/* abnf message any element encode, 
   user should ensure pAnyElem memory is valid */
ZFUNC ZINT Abnf_AnyEncode(ZUINT iProtocol, PFN_ABNFENCODE pfnEncode, 
                ZVOID *pAnyElem, ZDBUF zMsgBuf);

/* abnf message any element encode, 
   user should ensure pAnyElem memory is valid */
ZFUNC ZINT Abnf_AnyEncodeX(ZUINT iProtocol, PFN_ABNFENCODE pfnEncode, 
                ZVOID *pAnyElem, ZBOOL bFlatBuf, ZDBUF *pzMsgBuf);

/* abnf message any element list item decode */
ZFUNC ZINT Abnf_AnyLstItemDecode(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pParmLst, 
                ZINT iSize, PFN_ABNFDECODE pfnDecode);

/* abnf message any element list encode */
ZFUNC ZINT Abnf_AnyLstEncode(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pParmLst, 
                ZBOOL bSepaBegin, ZBOOL bSepaEnd, ZUCHAR ucSepa, 
                PFN_ABNFENCODE pfnEncode);

/* abnf message any element list encode */
ZFUNC ZINT Abnf_AnyLstEncodeX(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pParmLst, 
                ZBOOL bSepaBegin, ZBOOL bSepaEnd, ZCHAR *pcSepa, 
                PFN_ABNFENCODE pfnEncode);

/* abnf any element list add parameter */
ZFUNC ZINT Abnf_AnyLstAddParm(ZUBUF zBufId, ZVOID *pParmLst, ZINT iSize, 
                ZVOID **ppParm);

/* abnf any element list remove parameter */
ZFUNC ZINT Abnf_AnyLstRmvParm(ZVOID *pParmLst, ZVOID *pParm);

/* abnf any element list copy parameter */
ZFUNC ZINT Abnf_AnyLstCpyParm(ZUBUF zBufId, ZVOID *pParmLst,
                ZVOID *pSrcParm, PFN_ABNFANYCPY pfnCpy, ZINT iSize, 
                ZVOID **ppDstParm);


#ifdef __cplusplus
}
#endif

#endif /* _ABNF_UTIL_H__ */


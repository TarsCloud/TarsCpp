/************************************************************************

        Copyright (c) 2005-2014 by Juphoon System Software, Inc.
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
  File name     : ea_cab_pcc.h
  Module        : eax cab pcc
  Author        : zocol
  Version       : 0.1
  Created on    : 2014-09-04
  Description   :
    Marcos and structure definitions required by the eax cab pcc.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_CAB_PCC_H__
#define _EA_CAB_PCC_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:oma:xml:cab:pcc **************/
typedef enum EN_EA_CAB_PCC_TYPE
{
    EN_EA_CAB_PCC_PCC,               /* pcc */
    EN_EA_CAB_PCC_PRSN_DTL,          /* person-details */
    EN_EA_CAB_PCC_PCC_TYPE,          /* pcc-type */
    EN_EA_CAB_PCC_NAME,              /* name */
    EN_EA_CAB_PCC_ADDR,              /* address */
    EN_EA_CAB_PCC_COMM_ADDR,         /* comm-addr */
    EN_EA_CAB_PCC_BIRTH,             /* birth */
    EN_EA_CAB_PCC_CAREER,            /* career */
    EN_EA_CAB_PCC_NAME_ENTRY,        /* name-entry */
    EN_EA_CAB_PCC_GIVEN,             /* given */
    EN_EA_CAB_PCC_MIDDLE,            /* middle */
    EN_EA_CAB_PCC_FAMILY,            /* family */
    EN_EA_CAB_PCC_DISPLAY_NAME,      /* display-name */
    EN_EA_CAB_PCC_FIRST,             /* first */
    EN_EA_CAB_PCC_NAME_TYPE,         /* name-type */
    EN_EA_CAB_PCC_PREF,              /* pref */
    EN_EA_CAB_PCC_ADDR_ENTRY,        /* address-entry */
    EN_EA_CAB_PCC_LABEL,             /* label */
    EN_EA_CAB_PCC_ADDR_DATA,         /* address-data */
    EN_EA_CAB_PCC_ADDR_STR,          /* addr-string */
    EN_EA_CAB_PCC_ADDR_TYPE,         /* address-type */
    EN_EA_CAB_PCC_URI_ENTRY,         /* uri-entry */
    EN_EA_CAB_PCC_TEL,               /* tel */
    EN_EA_CAB_PCC_ADDR_URI,          /* addr-uri */
    EN_EA_CAB_PCC_ADDR_URI_TYPE,     /* addr-uri-type */
    EN_EA_CAB_PCC_TEL_NB,            /* tel-nb */
    EN_EA_CAB_PCC_TEL_TYPE,          /* tel-type */
    EN_EA_CAB_PCC_XUI_TYPE,          /* xui-type */
    EN_EA_CAB_PCC_TEL_STR,           /* tel-string */
    EN_EA_CAB_PCC_BIRTH_DATE,        /* birth-date */
    EN_EA_CAB_PCC_PLACE,             /* place */
    EN_EA_CAB_PCC_EMPLOYER,          /* employer */
    EN_EA_CAB_PCC_DUTY,              /* duty */
    EN_EA_CAB_PCC_DATE,              /* date */
    EN_EA_CAB_PCC_NON_GREG_DATE,     /* non-greg-date */
    EN_EA_CAB_PCC_CAL_TYPE,          /* cal-type */
    EN_EA_CAB_PCC_PHONETIC,          /* phonetic */
    EN_EA_CAB_PCC_DISPLAY_ORDER,     /* display-order */
    EN_EA_CAB_PCC_MEDIA_LST,         /* media-list */
    EN_EA_CAB_PCC_MEDIA_ENTRY,       /* media-entry */
    EN_EA_CAB_PCC_MEDIA_TYPE,        /* media-type */
    EN_EA_CAB_PCC_MEDIA_URL,         /* media-url */
    EN_EA_CAB_PCC_MEDIA,             /* media */
    EN_EA_CAB_PCC_ALIAS,             /* Alias */
    EN_EA_CAB_PCC_LEGALNAME,         /* LegalName */
    EN_EA_CAB_PCC_KNOWNAS,           /* KnownAs */
    EN_EA_CAB_PCC_MAIDENNAME,        /* MaidenName */
    EN_EA_CAB_PCC_FORMERNAME,        /* FormerName */
    EN_EA_CAB_PCC_NAMEATBIRTH,       /* NameAtBirth */
    EN_EA_CAB_PCC_OFFICIALNAME,      /* OfficialName */
    EN_EA_CAB_PCC_OTHER,             /* Other */
    EN_EA_CAB_PCC_HOME,              /* Home */
    EN_EA_CAB_PCC_WORK,              /* Work */
    EN_EA_CAB_PCC_BUSINESS,          /* Business */
    EN_EA_CAB_PCC_TRAVEL,            /* Travel */
    EN_EA_CAB_PCC_FAX,               /* Fax */
    EN_EA_CAB_PCC_MOBILE,            /* Mobile */
    EN_EA_CAB_PCC_FIXED,             /* Fixed */
    EN_EA_CAB_PCC_EMAIL,             /* Email */
    EN_EA_CAB_PCC_PAGER,             /* Pager */
    EN_EA_CAB_PCC_SIP_URI,           /* SIP-URI */
    EN_EA_CAB_PCC_IM,                /* IM */
    EN_EA_CAB_PCC_VIDEO,             /* Video */
    EN_EA_CAB_PCC_CAB,               /* CAB */
    EN_EA_CAB_PCC_AKAN,              /* Akan */
    EN_EA_CAB_PCC_ASSYRIAN,          /* Assyrian */
    EN_EA_CAB_PCC_ARMENIAN,          /* Armenian */
    EN_EA_CAB_PCC_AZTEC,             /* Aztec */
    EN_EA_CAB_PCC_BABYLONIAN,        /* Babylonian */
    EN_EA_CAB_PCC_BAHAI,             /* Bahai */
    EN_EA_CAB_PCC_BENGALI,           /* Bengali */
    EN_EA_CAB_PCC_BERBER,            /* Berber */
    EN_EA_CAB_PCC_BIKRAM_SAVAT,      /* Bikram Savat */
    EN_EA_CAB_PCC_BUDDHIST,          /* Buddhist */
    EN_EA_CAB_PCC_BURMESE,           /* Burmese */
    EN_EA_CAB_PCC_BYZANTINE,         /* Byzantine */
    EN_EA_CAB_PCC_CELTIC,            /* Celtic */
    EN_EA_CAB_PCC_CHINESE,           /* Chinese */
    EN_EA_CAB_PCC_COPTIC,            /* Coptic */
    EN_EA_CAB_PCC_EGYPTIAN,          /* Egyptian */
    EN_EA_CAB_PCC_ETHIOPIAN,         /* Ethiopian */
    EN_EA_CAB_PCC_FRENCH_REPUBLICAN, /* French Republican */
    EN_EA_CAB_PCC_GERMANIC,          /* Germanic */
    EN_EA_CAB_PCC_HEBREW,            /* Hebrew */
    EN_EA_CAB_PCC_HELLENIC,          /* Hellenic */
    EN_EA_CAB_PCC_HINDU,             /* Hindu */
    EN_EA_CAB_PCC_IGBO,              /* Igbo */
    EN_EA_CAB_PCC_INDIAN,            /* Indian */
    EN_EA_CAB_PCC_IRANIAN,           /* Iranian */
    EN_EA_CAB_PCC_IRISH,             /* Irish */
    EN_EA_CAB_PCC_ISLAMIC,           /* Islamic */
    EN_EA_CAB_PCC_JAPANSES,          /* Japanese */
    EN_EA_CAB_PCC_JAVANSES,          /* Javanese */
    EN_EA_CAB_PCC_JUCHE,             /* Juche */
    EN_EA_CAB_PCC_JULIAN,            /* Julian */
    EN_EA_CAB_PCC_KOREAN,            /* Korean */
    EN_EA_CAB_PCC_KURDISH,           /* Kurdish */
    EN_EA_CAB_PCC_LITHUANIAN,        /* Lithuanian */
    EN_EA_CAB_PCC_MALAYALAM,         /* Malayalam */
    EN_EA_CAB_PCC_MAYA,              /* Maya */
    EN_EA_CAB_PCC_NANAKSHAHI,        /* Nanakshahi */
    EN_EA_CAB_PCC_NEPAL_SAMBAT,      /* Nepal Sambat */
    EN_EA_CAB_PCC_SOVIET,            /* Soviet */
    EN_EA_CAB_PCC_TAMIL,             /* Tamil */
    EN_EA_CAB_PCC_TIBETAN,           /* Tibetan */
    EN_EA_CAB_PCC_THAI,              /* Thai */
    EN_EA_CAB_PCC_VIETNAMESE,        /* Vietnamese */
    EN_EA_CAB_PCC_XHOSA,             /* Xhosa */
    EN_EA_CAB_PCC_YORUBA,            /* Yoruba */
    EN_EA_CAB_PCC_INDVDL,            /* individual */
    EN_EA_CAB_PCC_ORG,               /* organization */
    EN_EA_CAB_PCC_GRP                /* group */
} EN_EA_CAB_PCC_TYPE;

/* eax cab pcc pcc-type type */
typedef enum EN_EA_CAB_PCC_PTYPE_TYPE
{
    EN_EA_CAB_PCC_PTYPE_INDVDL,      /* individual */
    EN_EA_CAB_PCC_PTYPE_ORG,         /* organization */
    EN_EA_CAB_PCC_PTYPE_GRP          /* group */
} EN_EA_CAB_PCC_PTYPE_TYPE;

/* eax cab pcc name-type type */
typedef enum EN_EA_CAB_PCC_NTYPE_TYPE
{
    EN_EA_CAB_PCC_NTYPE_ALIAS,       /* Alias */
    EN_EA_CAB_PCC_NTYPE_LEGALNAME,   /* LegalName */
    EN_EA_CAB_PCC_NTYPE_KNOWNAS,     /* KnownAs */
    EN_EA_CAB_PCC_NTYPE_MAIDENNAME,  /* MaidenName */
    EN_EA_CAB_PCC_NTYPE_FORMERNAME,  /* FormerName */
    EN_EA_CAB_PCC_NTYPE_NAMEATBIRTH, /* NameAtBirth */
    EN_EA_CAB_PCC_NTYPE_OFFICIALNAME, /* OfficialName */
    EN_EA_CAB_PCC_NTYPE_OTHER,       /* Other */
} EN_EA_CAB_PCC_NTYPE_TYPE;

/* eax cab pcc address-type type */
typedef enum EN_EA_CAB_PCC_ATYPE_TYPE
{
    EN_EA_CAB_PCC_ATYPE_HOME,        /* Home */
    EN_EA_CAB_PCC_ATYPE_WORK,        /* Work */
    EN_EA_CAB_PCC_ATYPE_BUSINESS,    /* Business */
    EN_EA_CAB_PCC_ATYPE_TRAVEL,      /* Travel */
    EN_EA_CAB_PCC_ATYPE_NAMEATBIRTH, /* NameAtBirth */
    EN_EA_CAB_PCC_ATYPE_OFFICIALNAME, /* OfficialName */
    EN_EA_CAB_PCC_ATYPE_OTHER,       /* Other */
} EN_EA_CAB_PCC_ATYPE_TYPE;

/* eax cab pcc addr-uri-type type */
typedef enum EN_EA_CAB_PCC_AUTYPE_TYPE
{
    EN_EA_CAB_PCC_AUTYPE_WORK,       /* Work */
    EN_EA_CAB_PCC_AUTYPE_HOME,       /* Home */
    EN_EA_CAB_PCC_AUTYPE_FAX,        /* Fax */
    EN_EA_CAB_PCC_AUTYPE_MOBILE,     /* Mobile */
    EN_EA_CAB_PCC_AUTYPE_FIXED,      /* Fixed */
    EN_EA_CAB_PCC_AUTYPE_EMAIL,      /* Email */
    EN_EA_CAB_PCC_AUTYPE_PAGER,      /* Pager */
    EN_EA_CAB_PCC_AUTYPE_SIP_URI,    /* SIP-URI */
    EN_EA_CAB_PCC_AUTYPE_IM,         /* IM */
    EN_EA_CAB_PCC_AUTYPE_VIDEO,      /* Video */
    EN_EA_CAB_PCC_AUTYPE_OTHER,      /* Other */
} EN_EA_CAB_PCC_AUTYPE_TYPE;

/* eax cab pcc tel-type type */
typedef enum EN_EA_CAB_PCC_TTYPE_TYPE
{
    EN_EA_CAB_PCC_TTYPE_WORK,        /* Work */
    EN_EA_CAB_PCC_TTYPE_HOME,        /* Home */
    EN_EA_CAB_PCC_TTYPE_FAX,         /* Fax */
    EN_EA_CAB_PCC_TTYPE_MOBILE,      /* Mobile */
    EN_EA_CAB_PCC_TTYPE_FIXED,       /* Fixed */
    EN_EA_CAB_PCC_TTYPE_EMAIL,       /* Email */
    EN_EA_CAB_PCC_TTYPE_OTHER,       /* Other */
} EN_EA_CAB_PCC_TTYPE_TYPE;

/* eax cab pcc xui-type type */
typedef enum EN_EA_CAB_PCC_XTYPE_TYPE
{
    EN_EA_CAB_PCC_XTYPE_CAB,         /* CAB */
    EN_EA_CAB_PCC_XTYPE_OTHER,       /* Other */
} EN_EA_CAB_PCC_XTYPE_TYPE;

/* eax cab pcc cal-type type */
typedef enum EN_EA_CAB_PCC_CTYPE_TYPE
{
    EN_EA_CAB_PCC_CTYPE_AKAN,              /* Akan */
    EN_EA_CAB_PCC_CTYPE_ASSYRIAN,          /* Assyrian */
    EN_EA_CAB_PCC_CTYPE_ARMENIAN,          /* Armenian */
    EN_EA_CAB_PCC_CTYPE_AZTEC,             /* Aztec */
    EN_EA_CAB_PCC_CTYPE_BABYLONIAN,        /* Babylonian */
    EN_EA_CAB_PCC_CTYPE_BAHAI,             /* Bahai */
    EN_EA_CAB_PCC_CTYPE_BENGALI,           /* Bengali */
    EN_EA_CAB_PCC_CTYPE_BERBER,            /* Berber */
    EN_EA_CAB_PCC_CTYPE_BIKRAM_SAVAT,      /* Bikram Savat */
    EN_EA_CAB_PCC_CTYPE_BUDDHIST,          /* Buddhist */
    EN_EA_CAB_PCC_CTYPE_BURMESE,           /* Burmese */
    EN_EA_CAB_PCC_CTYPE_BYZANTINE,         /* Byzantine */
    EN_EA_CAB_PCC_CTYPE_CELTIC,            /* Celtic */
    EN_EA_CAB_PCC_CTYPE_CHINESE,           /* Chinese */
    EN_EA_CAB_PCC_CTYPE_COPTIC,            /* Coptic */
    EN_EA_CAB_PCC_CTYPE_EGYPTIAN,          /* Egyptian */
    EN_EA_CAB_PCC_CTYPE_ETHIOPIAN,         /* Ethiopian */
    EN_EA_CAB_PCC_CTYPE_FRENCH_REPUBLICAN, /* French Republican */
    EN_EA_CAB_PCC_CTYPE_GERMANIC,          /* Germanic */
    EN_EA_CAB_PCC_CTYPE_HEBREW,            /* Hebrew */
    EN_EA_CAB_PCC_CTYPE_HELLENIC,          /* Hellenic */
    EN_EA_CAB_PCC_CTYPE_HINDU,             /* Hindu */
    EN_EA_CAB_PCC_CTYPE_IGBO,              /* Igbo */
    EN_EA_CAB_PCC_CTYPE_INDIAN,            /* Indian */
    EN_EA_CAB_PCC_CTYPE_IRANIAN,           /* Iranian */
    EN_EA_CAB_PCC_CTYPE_IRISH,             /* Irish */
    EN_EA_CAB_PCC_CTYPE_ISLAMIC,           /* Islamic */
    EN_EA_CAB_PCC_CTYPE_JAPANSES,          /* Japanese */
    EN_EA_CAB_PCC_CTYPE_JAVANSES,          /* Javanese */
    EN_EA_CAB_PCC_CTYPE_JUCHE,             /* Juche */
    EN_EA_CAB_PCC_CTYPE_JULIAN,            /* Julian */
    EN_EA_CAB_PCC_CTYPE_KOREAN,            /* Korean */
    EN_EA_CAB_PCC_CTYPE_KURDISH,           /* Kurdish */
    EN_EA_CAB_PCC_CTYPE_LITHUANIAN,        /* Lithuanian */
    EN_EA_CAB_PCC_CTYPE_MALAYALAM,         /* Malayalam */
    EN_EA_CAB_PCC_CTYPE_MAYA,              /* Maya */
    EN_EA_CAB_PCC_CTYPE_NANAKSHAHI,        /* Nanakshahi */
    EN_EA_CAB_PCC_CTYPE_NEPAL_SAMBAT,      /* Nepal Sambat */
    EN_EA_CAB_PCC_CTYPE_SOVIET,            /* Soviet */
    EN_EA_CAB_PCC_CTYPE_TAMIL,             /* Tamil */
    EN_EA_CAB_PCC_CTYPE_TIBETAN,           /* Tibetan */
    EN_EA_CAB_PCC_CTYPE_THAI,              /* Thai */
    EN_EA_CAB_PCC_CTYPE_VIETNAMESE,        /* Vietnamese */
    EN_EA_CAB_PCC_CTYPE_XHOSA,             /* Xhosa */
    EN_EA_CAB_PCC_CTYPE_YORUBA,            /* Yoruba */
} EN_EA_CAB_PCC_CTYPE_TYPE;

/* eax cab pcc get the pcc */
ZFUNC ZINT EaCab_PccGetPcc(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstPcc);

/* eax cab pcc get the first person-details */
ZFUNC ZINT EaCab_PccGetFstPrsnDtls(ST_XML_ELEM *pstPcc, 
                ST_XML_ELEM **ppstPrsnDtls);

/* eax cab pcc get the next person-details */
ZFUNC ZINT EaCab_PccGetNxtPrsnDtls(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstPrsnDtls);

/* eax cab pcc get the pcc-type, type see EN_EA_CAB_PCC_PTYPE_TYPE */
ZFUNC ZINT EaCab_PccGetPccType(ST_XML_ELEM *pstPcc, 
                ZUCHAR *pucType);

/* eax cab pcc get the person-details name */
ZFUNC ZINT EaCab_PccPrsnDtlsGetName(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstName);

/* eax cab pcc get the person-details address */
ZFUNC ZINT EaCab_PccPrsnDtlsGetAddr(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstAddr);

/* eax cab pcc get the person-details comm-addr */
ZFUNC ZINT EaCab_PccPrsnDtlsGetCommAddr(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstCommAddr);

/* eax cab pcc get the person-details birth */
ZFUNC ZINT EaCab_PccPrsnDtlsGetBirth(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstBirth);

/* eax cab pcc get the person-details career */
ZFUNC ZINT EaCab_PccPrsnDtlsGetCareer(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstCareer);

/* eax cab pcc get the person-details media-list */
ZFUNC ZINT EaCab_PccPrsnDtlsGetMediaLst(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstMediaLst);

/* eax cab pcc get the name name-entry */
ZFUNC ZINT EaCab_PccNameGetFstNameEntry(ST_XML_ELEM *pstName, 
                ST_XML_ELEM **ppstNameEntry);

/* eax cab pcc get the name name-entry */
ZFUNC ZINT EaCab_PccNameGetNxtNameEntry(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstNameEntry);

/* eax cab pcc get the address address-entry */
ZFUNC ZINT EaCab_PccAddrGetFstAddrEntry(ST_XML_ELEM *pstAddr, 
                ST_XML_ELEM **ppstAddrEntry);

/* eax cab pcc get the address address-entry */
ZFUNC ZINT EaCab_PccAddrGetNxtAddrEntry(ST_XML_ELEM *pstAddrEntry, 
                ST_XML_ELEM **ppstAddrEntry);

/* eax cab pcc get the comm-addr uri-entry */
ZFUNC ZINT EaCab_PccCommAddrGetFstUriEntry(ST_XML_ELEM *pstCommAddr, 
                ST_XML_ELEM **ppstUriEntry);

/* eax cab pcc get the comm-addr uri-entry */
ZFUNC ZINT EaCab_PccCommAddrGetNxtUriEntry(ST_XML_ELEM *pstUriEntry, 
                ST_XML_ELEM **ppstUriEntry);

/* eax cab pcc get the comm-addr tel */
ZFUNC ZINT EaCab_PccCommAddrGetFstTel(ST_XML_ELEM *pstCommAddr, 
                ST_XML_ELEM **ppstTel);

/* eax cab pcc get the comm-addr tel */
ZFUNC ZINT EaCab_PccCommAddrGetNxtTel(ST_XML_ELEM *pstTel, 
                ST_XML_ELEM **ppstTel);

/* eax cab pcc get the birth birth-date */
ZFUNC ZINT EaCab_PccBirthGetBirthDate(ST_XML_ELEM *pstBirth, 
                ST_XML_ELEM **ppstBirthDate);

/* eax cab pcc get the birth place */
ZFUNC ZINT EaCab_PccBirthGetPlace(ST_XML_ELEM *pstBirth, 
                ST_ZOS_USTR **ppstPlace);

/* eax cab pcc get the career employer */
ZFUNC ZINT EaCab_PccCareerGetEmployer(ST_XML_ELEM *pstCareer, 
                ST_ZOS_USTR **ppstEmployer);

/* eax cab pcc get the career duty */
ZFUNC ZINT EaCab_PccCareerGetDuty(ST_XML_ELEM *pstCareer, 
                ST_ZOS_USTR **ppstDuty);

/* eax cab pcc get the media-list media-entry */
ZFUNC ZINT EaCab_PccMediaLstGetFstMediaEntry(ST_XML_ELEM *pstMediaLst, 
                ST_XML_ELEM **ppstMediaEntry);

/* eax cab pcc get the media-list media-entry */
ZFUNC ZINT EaCab_PccMediaLstGetNxtMediaEntry(ST_XML_ELEM *pstMediaEntry, 
                ST_XML_ELEM **ppstMediaEntry);

/* eax cab pcc get the name-entry given */
ZFUNC ZINT EaCab_PccNameEntryGetGiven(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstGiven);

/* eax cab pcc get the name-entry given */
ZFUNC ZINT EaCab_PccNameEntryGetGivenX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR **ppstGiven);

/* eax cab pcc get the name-entry middle */
ZFUNC ZINT EaCab_PccNameEntryGetMiddle(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstMiddle);

/* eax cab pcc get the name-entry middle */
ZFUNC ZINT EaCab_PccNameEntryGetMiddleX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR **ppstMiddle);

/* eax cab pcc get the name-entry family */
ZFUNC ZINT EaCab_PccNameEntryGetFamily(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstFamily);

/* eax cab pcc get the name-entry family */
ZFUNC ZINT EaCab_PccNameEntryGetFamilyX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR **ppstFamily);

/* eax cab pcc get the name-entry display-name */
ZFUNC ZINT EaCab_PccNameEntryGetDispName(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstDispName);

/* eax cab pcc get the name-entry display-name */
ZFUNC ZINT EaCab_PccNameEntryGetDispNameX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR **ppstDispName);

/* eax cab pcc get the name-entry first */
ZFUNC ZINT EaCab_PccNameEntryGetFirst(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstFirst);

/* eax cab pcc get the name-entry first */
ZFUNC ZINT EaCab_PccNameEntryGetFirstX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR **ppstFirst);

/* eax cab pcc get the name-entry name-type, type see EN_EA_CAB_PCC_NTYPE_TYPE */
ZFUNC ZINT EaCab_PccNameEntryGetNameType(ST_XML_ELEM *pstNameEntry, 
                ZUCHAR *pucType);

/* eax cab pcc get the name-entry pref */
ZFUNC ZINT EaCab_PccNameEntryGetPref(ST_XML_ELEM *pstNameEntry, 
                ZUINT *piPref);

/* eax cab pcc get the address-entry label */
ZFUNC ZINT EaCab_PccAddrEntryGetLabel(ST_XML_ELEM *pstAddrEntry, 
                ST_ZOS_USTR **ppstLabel);

/* eax cab pcc get the address-entry address-data */
ZFUNC ZINT EaCab_PccAddrEntryGetAddrData(ST_XML_ELEM *pstAddrEntry, 
                ST_XML_ELEM **ppstAddrData);

/* eax cab pcc get the address-entry address-type, type see EN_EA_CAB_PCC_ATYPE_TYPE */
ZFUNC ZINT EaCab_PccAddrEntryGetAddrType(ST_XML_ELEM *pstAddrEntry, 
                ZUCHAR *pucType);

/* eax cab pcc get the address-entry pref */
ZFUNC ZINT EaCab_PccAddrEntryGetPref(ST_XML_ELEM *pstAddrEntry, 
                ZUINT *piPref);

/* eax cab pcc get the uri-entry addr-uri */
ZFUNC ZINT EaCab_PccUriEntryGetAddrUri(ST_XML_ELEM *pstUriEntry, 
                ST_ZOS_USTR **ppstAddrUri);

/* eax cab pcc get the uri-entry label */
ZFUNC ZINT EaCab_PccUriEntryGetLabel(ST_XML_ELEM *pstUriEntry, 
                ST_ZOS_USTR **ppstLabel);

/* eax cab pcc get the uri-entry pref */
ZFUNC ZINT EaCab_PccUriEntryGetPref(ST_XML_ELEM *pstUriEntry, 
                ZUINT *piPref);

/* eax cab pcc get the uri-entry addr-uri-type, type see EN_EA_CAB_PCC_AUTYPE_TYPE */
ZFUNC ZINT EaCab_PccUriEntryGetAddrUriType(ST_XML_ELEM *pstUriEntry, 
                ZUCHAR *pucType);

/* eax cab pcc get the tel tel-nb */
ZFUNC ZINT EaCab_PccTelGetTelNb(ST_XML_ELEM *pstTel, 
                ST_XML_ELEM **ppstTelNb);

/* eax cab pcc get the tel label */
ZFUNC ZINT EaCab_PccTelGetLabel(ST_XML_ELEM *pstTel, 
                ST_ZOS_USTR **ppstLabel);

/* eax cab pcc get the tel tel-type, type see EN_EA_CAB_PCC_TTYPE_TYPE */
ZFUNC ZINT EaCab_PccTelGetTelType(ST_XML_ELEM *pstTel, 
                ZUCHAR *pucType);

/* eax cab pcc get the tel xui-type, type see EN_EA_CAB_PCC_XTYPE_TYPE */
ZFUNC ZINT EaCab_PccTelGetXuiType(ST_XML_ELEM *pstTel, 
                ZUCHAR *pucType);

/* eax cab pcc get the tel pref */
ZFUNC ZINT EaCab_PccTelGetPref(ST_XML_ELEM *pstTel, 
                ZUINT *piPref);

/* eax cab pcc get the birth-date date */
ZFUNC ZINT EaCab_PccBirthDateGetDate(ST_XML_ELEM *pstBirthDate, 
                ST_ZOS_USTR **ppstDate);

/* eax cab pcc get the birth-date non-greg-date */
ZFUNC ZINT EaCab_PccBirthDateGetNgd(ST_XML_ELEM *pstBirthDate, 
                ST_XML_ELEM **ppstNgd);

/* eax cab pcc get the birth-date non-greg-date */
ZFUNC ZINT EaCab_PccBirthDateGetNgdX(ST_XML_ELEM *pstBirthDate, 
                ST_ZOS_USTR **ppstNgd);

/* eax cab pcc get the media-entry media */
ZFUNC ZINT EaCab_PccMediaEntryGetMedia(ST_XML_ELEM *pstMediaEntry, 
                ST_XML_ELEM **ppstMedia);

/* eax cab pcc get the media-entry media-type */
ZFUNC ZINT EaCab_PccMediaEntryGetMediaType(ST_XML_ELEM *pstMediaEntry, 
                ST_ZOS_USTR **ppstMediaType);

/* eax cab pcc get the media-entry label */
ZFUNC ZINT EaCab_PccMediaEntryGetLabel(ST_XML_ELEM *pstMediaEntry, 
                ST_ZOS_USTR **ppstLabel);

/* eax cab pcc get the media-entry pref */
ZFUNC ZINT EaCab_PccMediaEntryGetPref(ST_XML_ELEM *pstNameEntry, 
                ZUINT *piPref);

/* eax cab pcc get the element(given, middle, family, display-name, first) phonetic */
ZFUNC ZINT EaCab_PccElemGetPhonetic(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstPhonetic);

/* eax cab pcc get the element(given, middle, family, display-name, first) display-order */
ZFUNC ZINT EaCab_PccElemGetDispOrder(ST_XML_ELEM *pstElem, 
                ZUINT *piOrder);

/* eax cab pcc get the address-data addr-string */
ZFUNC ZINT EaCab_PccAddrDataGetAddrStr(ST_XML_ELEM *pstAddrData, 
                ST_ZOS_USTR **ppstAddrStr);

/* eax cab pcc get the tel-nb tel-string */
ZFUNC ZINT EaCab_PccTelNbGetTelStr(ST_XML_ELEM *pstTelNb, 
                ST_ZOS_USTR **ppstTelStr);

/* eax cab pcc get the non-greg-date cal-type, type see EN_EA_CAB_PCC_CTYPE_TYPE */
ZFUNC ZINT EaCab_PccNgdGetCalType(ST_XML_ELEM *pstNgd, 
                ZUCHAR *pucType);

/* eax cab pcc get the media media-url */
ZFUNC ZINT EaCab_PccMediaGetMediaUrl(ST_XML_ELEM *pstMediaEntry, 
                ST_ZOS_USTR **ppstMediaUrl);

/* eax cab pcc set the pcc */
ZFUNC ZINT EaCab_PccSetPcc(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstPcc);

/* eax cab pcc set the person-details */
ZFUNC ZINT EaCab_PccSetPrsnDtls(ST_XML_ELEM *pstPcc, 
                ST_XML_ELEM **ppstPrsnDtls);

/* eax cab pcc set the pcc-type, type see EN_EA_CAB_PCC_PTYPE_TYPE */
ZFUNC ZINT EaCab_PccSetPccType(ST_XML_ELEM *pstPcc, 
                ZUCHAR ucType);

/* eax cab pcc set the person-details name */
ZFUNC ZINT EaCab_PccPrsnDtlsSetName(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstName);

/* eax cab pcc set the person-details address */
ZFUNC ZINT EaCab_PccPrsnDtlsSetAddr(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstAddr);

/* eax cab pcc set the person-details comm-addr */
ZFUNC ZINT EaCab_PccPrsnDtlsSetCommAddr(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstCommAddr);

/* eax cab pcc set the person-details birth */
ZFUNC ZINT EaCab_PccPrsnDtlsSetBirth(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstBirth);

/* eax cab pcc set the person-details career */
ZFUNC ZINT EaCab_PccPrsnDtlsSetCareer(ST_XML_ELEM *pstPrsnDtls, 
                ST_XML_ELEM **ppstCareer);

/* eax cab pcc set the name name-entry */
ZFUNC ZINT EaCab_PccNameSetNameEntry(ST_XML_ELEM *pstName, 
                ST_XML_ELEM **ppstNameEntry);

/* eax cab pcc set the address address-entry */
ZFUNC ZINT EaCab_PccAddrSetAddrEntry(ST_XML_ELEM *pstAddr, 
                ST_XML_ELEM **ppstAddrEntry);

/* eax cab pcc set the comm-addr uri-entry */
ZFUNC ZINT EaCab_PccCommAddrSetUriEntry(ST_XML_ELEM *pstCommAddr, 
                ST_XML_ELEM **ppstUriEntry);

/* eax cab pcc set the comm-addr tel */
ZFUNC ZINT EaCab_PccCommAddrSetTel(ST_XML_ELEM *pstCommAddr, 
                ST_XML_ELEM **ppstTel);

/* eax cab pcc set the birth birth-date */
ZFUNC ZINT EaCab_PccBirthSetBirthDate(ST_XML_ELEM *pstBirth, 
                ST_XML_ELEM **ppstBirthDate);

/* eax cab pcc set the birth place */
ZFUNC ZINT EaCab_PccBirthSetPlace(ST_XML_ELEM *pstBirth, 
                ST_ZOS_USTR *pstPlace);

/* eax cab pcc set the career employer */
ZFUNC ZINT EaCab_PccCareerSetEmployer(ST_XML_ELEM *pstCareer, 
                ST_ZOS_USTR *pstEmployer);

/* eax cab pcc set the career duty */
ZFUNC ZINT EaCab_PccCareerSetDuty(ST_XML_ELEM *pstCareer, 
                ST_ZOS_USTR *pstDuty);

/* eax cab pcc set the name-entry given */
ZFUNC ZINT EaCab_PccNameEntrySetGiven(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstGiven);

/* eax cab pcc set the name-entry given */
ZFUNC ZINT EaCab_PccNameEntrySetGivenX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR *pstGiven);

/* eax cab pcc set the name-entry middle */
ZFUNC ZINT EaCab_PccNameEntrySetMiddle(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstMiddle);

/* eax cab pcc set the name-entry middle */
ZFUNC ZINT EaCab_PccNameEntrySetMiddleX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR *pstMiddle);

/* eax cab pcc set the name-entry family */
ZFUNC ZINT EaCab_PccNameEntrySetFamily(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstFamily);

/* eax cab pcc set the name-entry family */
ZFUNC ZINT EaCab_PccNameEntrySetFamilyX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR *pstFamily);

/* eax cab pcc set the name-entry display-name */
ZFUNC ZINT EaCab_PccNameEntrySetDispName(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstDispName);

/* eax cab pcc set the name-entry display-name */
ZFUNC ZINT EaCab_PccNameEntrySetDispNameX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR *pstDispName);

/* eax cab pcc set the name-entry first */
ZFUNC ZINT EaCab_PccNameEntrySetFirst(ST_XML_ELEM *pstNameEntry, 
                ST_XML_ELEM **ppstFirst);

/* eax cab pcc set the name-entry first */
ZFUNC ZINT EaCab_PccNameEntrySetFirstX(ST_XML_ELEM *pstNameEntry, 
                ST_ZOS_USTR *pstFirst);

/* eax cab pcc set the name-entry name-type, type see EN_EA_CAB_PCC_NTYPE_TYPE */
ZFUNC ZINT EaCab_PccNameEntrySetNameType(ST_XML_ELEM *pstNameEntry, 
                ZUCHAR ucType);

/* eax cab pcc set the name-entry pref */
ZFUNC ZINT EaCab_PccNameEntrySetPref(ST_XML_ELEM *pstNameEntry, 
                ZUINT iPref);

/* eax cab pcc set the address-entry label */
ZFUNC ZINT EaCab_PccAddrEntrySetLabel(ST_XML_ELEM *pstAddrEntry, 
                ST_ZOS_USTR *pstLabel);

/* eax cab pcc set the address-entry address-data */
ZFUNC ZINT EaCab_PccAddrEntrySetAddrData(ST_XML_ELEM *pstAddrEntry, 
                ST_XML_ELEM **ppstAddrData);

/* eax cab pcc set the address-entry address-type, type see EN_EA_CAB_PCC_ATYPE_TYPE */
ZFUNC ZINT EaCab_PccAddrEntrySetAddrType(ST_XML_ELEM *pstAddrEntry, 
                ZUCHAR ucType);

/* eax cab pcc set the address-entry pref */
ZFUNC ZINT EaCab_PccAddrEntrySetPref(ST_XML_ELEM *pstAddrEntry, 
                ZUINT iPref);

/* eax cab pcc set the uri-entry addr-uri */
ZFUNC ZINT EaCab_PccUriEntrySetAddrUri(ST_XML_ELEM *pstUriEntry, 
                ST_ZOS_USTR *pstAddrUri);

/* eax cab pcc set the uri-entry label */
ZFUNC ZINT EaCab_PccUriEntrySetLabel(ST_XML_ELEM *pstUriEntry, 
                ST_ZOS_USTR *pstLabel);

/* eax cab pcc set the uri-entry pref */
ZFUNC ZINT EaCab_PccUriEntrySetPref(ST_XML_ELEM *pstUriEntry, 
                ZUINT iPref);

/* eax cab pcc set the uri-entry addr-uri-type, type see EN_EA_CAB_PCC_AUTYPE_TYPE */
ZFUNC ZINT EaCab_PccUriEntrySetAddrUriType(ST_XML_ELEM *pstUriEntry, 
                ZUCHAR ucType);

/* eax cab pcc set the tel tel-nb */
ZFUNC ZINT EaCab_PccTelSetTelNb(ST_XML_ELEM *pstTel, 
                ST_XML_ELEM **ppstTelNb);

/* eax cab pcc set the tel label */
ZFUNC ZINT EaCab_PccTelSetLabel(ST_XML_ELEM *pstTel, 
                ST_ZOS_USTR *pstLabel);

/* eax cab pcc set the tel tel-type, type see EN_EA_CAB_PCC_TTYPE_TYPE */
ZFUNC ZINT EaCab_PccTelSetTelType(ST_XML_ELEM *pstTel, 
                ZUCHAR ucType);

/* eax cab pcc set the tel xui-type, type see EN_EA_CAB_PCC_XTYPE_TYPE */
ZFUNC ZINT EaCab_PccTelSetXuiType(ST_XML_ELEM *pstTel, 
                ZUCHAR ucType);

/* eax cab pcc set the tel pref */
ZFUNC ZINT EaCab_PccTelSetPref(ST_XML_ELEM *pstTel, 
                ZUINT iPref);

/* eax cab pcc set the birth-date date */
ZFUNC ZINT EaCab_PccBirthDateSetDate(ST_XML_ELEM *pstBirthDate, 
                ST_ZOS_USTR *pstDate);

/* eax cab pcc set the birth-date non-greg-date */
ZFUNC ZINT EaCab_PccBirthDateSetNgd(ST_XML_ELEM *pstBirthDate, 
                ST_XML_ELEM **ppstNgd);

/* eax cab pcc set the birth-date non-greg-date */
ZFUNC ZINT EaCab_PccBirthDateSetNgdX(ST_XML_ELEM *pstBirthDate, 
                ST_ZOS_USTR *pstNgd);

/* eax cab pcc set the element(given, middle, family, display-name, first) phonetic */
ZFUNC ZINT EaCab_PccElemSetPhonetic(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstPhonetic);

/* eax cab pcc set the element(given, middle, family, display-name, first) display-order */
ZFUNC ZINT EaCab_PccElemSetDispOrder(ST_XML_ELEM *pstElem, 
                ZUINT iOrder);

/* eax cab pcc set the address-data addr-string */
ZFUNC ZINT EaCab_PccAddrDataSetAddrStr(ST_XML_ELEM *pstAddrData, 
                ST_ZOS_USTR *pstAddrStr);

/* eax cab pcc set the tel-nb tel-string */
ZFUNC ZINT EaCab_PccTelNbSetTelStr(ST_XML_ELEM *pstTelNb, 
                ST_ZOS_USTR *pstTelStr);

/* eax cab pcc set the non-greg-date cal-type, type see EN_EA_CAB_PCC_CTYPE_TYPE */
ZFUNC ZINT EaCab_PccNgdSetCalType(ST_XML_ELEM *pstNgd, 
                ZUCHAR ucType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_CAB_PCC_H__ */


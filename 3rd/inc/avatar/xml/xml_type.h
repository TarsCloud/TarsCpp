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
  File name     : xml_type.h
  Module        : xml protocol typedef, structures and functions
  Author        : zocol
  Version       : 0.1
  Created on    : 2006-04-17
  Description   :
    Marcos and structure definitions required by the xml protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XML_TYPE_H__
#define _XML_TYPE_H__
/** 
 * @file xml_type.h
 * @brief XML type and structure definition.
 *
 * @see 
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief XML return value. */
#define XML_OK              ZOK      /**< @brief XML return ok. */
#define XML_FAIL            ZFAILED  /**< @brief XML return fail. */
#define XML_ERR             2        /**< @brief XML return error. */

/** @brief XML encoding type. */
typedef enum EN_XML_ENCODING_TYPE
{
    EN_XML_ENCODING_ASCII,           /**< @brief "Us-ascii" encoding. */
    EN_XML_ENCODING_UTF8,            /**< @brief "Utf-8" encoding .*/
    EN_XML_ENCODING_UNKNOWN          /**< @brief Unknown encoding. */
} EN_XML_ENCODING_TYPE;

/** @brief XML Misc type. */
typedef enum EN_XML_MISC_TYPE
{
    EN_XML_MISC_COMMENT,             /**< @brief Comment. */
    EN_XML_MISC_PI,                  /**< @brief Processing Instructions. */
    EN_XML_MISC_S                    /**< @brief White space. */
} EN_XML_MISC_TYPE;

/** @brief XML markupdecl type. */
typedef enum EN_XML_MARKDECL_TYPE
{
    EN_XML_MARKDECL_ELEM,             /**< @brief Element. */
    EN_XML_MARKDECL_ATTLST,           /**< @brief Attribute list. */
    EN_XML_MARKDECL_ENT,              /**< @brief Entity. */
    EN_XML_MARKDECL_NOTATION,         /**< @brief Notation. */
    EN_XML_MARKDECL_PI,               /**< @brief Processing Instructions. */
    EN_XML_MARKDECL_COMMENT           /**< @brief Comment. */
} EN_XML_MARKDECL_TYPE;

/** @brief XML content item type. */
typedef enum EN_XML_CONTENT_ITEM_TYPE
{
    EN_XML_CONTENT_ITEM_ELEM,         /**< @brief Element. */
    EN_XML_CONTENT_ITEM_CHARDATA,     /**< @brief Character data. */
    EN_XML_CONTENT_ITEM_REF,          /**< @brief Reference. */
    EN_XML_CONTENT_ITEM_CDSET,        /**< @brief A type of Cdata. */
    EN_XML_CONTENT_ITEM_PI,           /**< @brief Processing Instructions. */
    EN_XML_CONTENT_ITEM_COMMENT,      /**< @brief Comment. */
    EN_XML_CONTENT_ITEM_IGNWS         /**< @brief IGNWS. */
} EN_XML_CONTENT_ITEM_TYPE;

/** @brief XML string pair type. */
typedef enum EN_XML_STR_PAIR_TYPE
{
    EN_XML_STR_PAIR_DQUOTE,           /**< @brief Double quote. */
    EN_XML_STR_PAIR_SQUOTE            /**< @brief Single quote. */
} EN_XML_STR_PAIR_TYPE;

/** @brief XML contentspec type. */
typedef enum EN_XML_CONTENTSPEC_TYPE
{
    EN_XML_CONTENTSPEC_EMPTY,         /**< @brief Empty. */
    EN_XML_CONTENTSPEC_ANY,           /**< @brief Any. */
    EN_XML_CONTENTSPEC_MIXED,         /**< @brief Mixed content. */
    EN_XML_CONTENTSPEC_CHILD          /**< @brief Children. */
} EN_XML_CONTENTSPEC_TYPE; 

/** @brief XML symbol type. */
typedef enum EN_XML_SYM_TYPE
{ 
    EN_XML_SYM_QMASK,                 /**< @brief '?'. */
    EN_XML_SYM_STAR,                  /**< @brief '*'. */
    EN_XML_SYM_PLUS                   /**< @brief '+'. */
} EN_XML_SYM_TYPE;

/** @brief XML cp type. */
typedef enum EN_XML_CP_TYPE
{
    EN_XML_CP_NAME,                   /**< @brief Name. */              
    EN_XML_CP_CHOICE,                 /**< @brief Choice. */              
    EN_XML_CP_SEQ                     /**< @brief Seq. */                  
} EN_XML_CP_TYPE;

/** @brief XML DefaultDecl type. */
typedef enum EN_XML_DFTDECL_TYPE
{
    EN_XML_DFTDECL_REQUIRED,          /**< @brief Required. */             
    EN_XML_DFTDECL_IMPLIED,           /**< @brief Implied. */             
    EN_XML_DFTDECL_FIXED              /**< @brief Fixed. */
} EN_XML_DFTDECL_TYPE;

/** @brief XML AttType type. */
typedef enum EN_XML_ATT_TYPE_TYPE
{
    EN_XML_ATT_TYPE_STR_CDATA,        /**< @brief Cdata. */ 
    EN_XML_ATT_TYPE_TKN_ID,           /**< @brief ID. */ 
    EN_XML_ATT_TYPE_TKN_IDREF,        /**< @brief IDRef. */
    EN_XML_ATT_TYPE_TKN_IDREFS,       /**< @brief IDRefs. */
    EN_XML_ATT_TYPE_TKN_ENTITY,       /**< @brief Entity. */
    EN_XML_ATT_TYPE_TKN_ENTITIES,     /**< @brief Entities. */
    EN_XML_ATT_TYPE_TKN_NMTOKEN,      /**< @brief Name token. */
    EN_XML_ATT_TYPE_TKN_NMTOKENS,     /**< @brief Name tokens. */
    EN_XML_ATT_TYPE_ENUM_NOTATION,    /**< @brief Notation. */
    EN_XML_ATT_TYPE_ENUM_ENUM         /**< @brief Enumeration. */
} EN_XML_ATT_TYPE_TYPE;

/** @brief XML EntityValue type. */
typedef enum EN_XML_ENT_VAL_TYPE   
{
    EN_XML_ENT_VAL_GEN,               /**< @brief Gerenral Entity. */
    EN_XML_ENT_VAL_PEREF,             /**< @brief PEReference. */
    EN_XML_ENT_VAL_REF                /**< @brief Reference. */
} EN_XML_ENT_VAL_TYPE;

/** @brief XML list use the dlist. */
#define XML_TYPEDEF_LIST(_name) \
    ZOS_TYPEDEF_DLIST(_name)

/** @brief Typedef xml list */
typedef ST_ZOS_DLIST ST_XML_LIST;

/** @brief Typedef xml list node */
typedef ST_ZOS_DLIST_NODE ST_XML_LIST_NODE;

/** @brief XML S. */
typedef ST_ZOS_USTR ST_XML_S;

/** @brief XML Name ((Letter | '_' | ':') (NameChar)*). */
typedef ST_ZOS_USTR ST_XML_NAME;

/** @brief XML Name list */
XML_TYPEDEF_LIST(XML_NAME);

/** @brief XML Nmtoken. */
typedef ST_ZOS_USTR ST_XML_NMTKN;

/** @brief XML Nmtoken list. */
XML_TYPEDEF_LIST(XML_NMTKN);

/** @brief Typedef xml element */
typedef struct tagXML_ELEM ST_XML_ELEM;

/** @brief XML element list. */
XML_TYPEDEF_LIST(XML_ELEM);

/** @brief XML Comment ('<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'). */
typedef struct tagXML_COMMENT
{
    ST_XML_ELEM *pstParent;        /**< @brief Parent element. */
    ST_ZOS_USTR stData;            /**< @brief Comment data. */
} ST_XML_COMMENT;

/** @brief XML PITarget Name - (('X' | 'x') ('M' | 'm') ('L' | 'l')). */
typedef ST_ZOS_USTR ST_XML_PI_TARGET;

/** @brief XML QName. */
typedef struct tagXML_QNAME
{
    ZUCHAR *pucPrefix;             /**< @brief Prefix string. */
    ZUCHAR *pucLocal;              /**< @brief LocalPart string. */
    ZUINT iPrefixLen;            /**< @brief Prefix string length. */
    ZUINT iLocalLen;             /**< @brief LocalPart string length. */
} ST_XML_QNAME;

/** @brief XML QName list. */
XML_TYPEDEF_LIST(XML_QNAME);

/** @brief XML PI ('<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'). */
typedef struct tagXML_PI
{
    ST_XML_ELEM *pstParent;        /**< @brief Parent element. */
    ST_XML_PI_TARGET stPiTarget;   /**< @brief PITarget. */
    ST_ZOS_USTR stStr;             /**< @brief PI String. */
} ST_XML_PI;

/** @brief XML VersionNum. */
typedef struct tagXML_VER_NUM
{
    ZUCHAR ucPairType;             /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR aucSpare[3];            /**< @brief For 32 bit alignment. */
    ST_ZOS_USTR stStr;             /**< @brief VersionNum string. */
} ST_XML_VER_NUM;

/**
 * @brief XML VersionInfo (S 'version' Eq 
 *        ("'" VersionNum "'" | '"' VersionNum '"')).
 */
typedef struct tagXML_VER_INFO
{
    ST_XML_VER_NUM stVerNum;       /**< @brief VersionNum. */
} ST_XML_VER_INFO;

/** @brief XML EncodingDecl. */
typedef struct tagXML_ENCODING_DECL
{
    ZUCHAR ucPres;                 /**< @brief Present flag. */
    ZUCHAR ucPairType;             /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR aucSpare[2];            /**< @brief For 32 bit alignment. */
    ST_ZOS_USTR stEncName;         /**< @brief EncName. */
} ST_XML_ENCODING_DECL;

/** @brief XML SDDecl. */
typedef struct tagXML_SD_DECL
{
    ZUCHAR ucPres;                 /**< @brief Present flag. */
    ZUCHAR ucPairType;             /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR aucSpare[2];            /**< @brief For 32 bit alignment. */
    ST_ZOS_USTR stYesNo;           /**< @brief Yes or No. */
} ST_XML_SD_DECL;

/** @brief XML PubidLiteral. */
typedef struct tagXML_PUB_LIT
{
    ZUCHAR ucPairType;             /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR aucSpare[3];            /**< @brief For 32 bit alignment. */
    ST_ZOS_USTR stPubStr;          /**< @brief PubidChar string. */
} ST_XML_PUB_LIT;

/** @brief XML SystemLiteral */
typedef struct tagXML_SYS_LIT
{
    ZUCHAR ucPairType;             /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR aucSpare[3];            /**< @brief For 32 bit alignment. */
    ST_ZOS_USTR stSysStr;          /**< @brief SystemLiteral string. */
} ST_XML_SYS_LIT;

/**
 * @brief XML ExternalID ('SYSTEM' S SystemLiteral 
                    | 'PUBLIC' S PubidLiteral S SystemLiteral).
 */
typedef struct tagXML_EXTERNAL_ID
{
    ZUCHAR ucPres;                 /**< @brief Present flag. */
    ZUCHAR ucPubPres;              /**< @brief Public present flag. */
    ZUCHAR aucSpare[2];            /**< @brief For 32 bit alignment. */
    ST_XML_PUB_LIT stPubLit;       /**< @brief PubidLiteral. */
    ST_XML_SYS_LIT stSysLit;       /**< @brief SystemLiteral. */
} ST_XML_EXTERNAL_ID;

/** @brief XML PublicID */
typedef struct tagXML_PUB_ID
{
    ST_XML_PUB_LIT stPubLit;        /**< @brief PubidLiteral. */
} ST_XML_PUB_ID;

/** @brief XML Mixed */
typedef struct tagXML_MIXED
{
    ST_XML_QNAME_LST stQNameLst;    /**< @brief QName list. */
} ST_XML_MIXED;

/** @brief Typedef xml choice */
typedef struct tagXML_CHOICE ST_XML_CHOICE;

/** @brief Typedef xml seq */
typedef struct tagXML_SEQ ST_XML_SEQ;

/** @brief XML cp. */
typedef struct tagXML_CP
{
    ZUCHAR ucSymPres;               /**< @brief Symbol present flag. */
    ZUCHAR ucType;                  /**< @brief Type EN_XML_CP_TYPE. */
    ZUCHAR ucSymType;               /**< @brief Symbol type EN_XML_SYM_TYPE. */
    ZUCHAR aucSpare[1];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_QNAME stQName;       /**< @brief QName. */
        ST_XML_CHOICE *pstChoice;   /**< @brief Choice. */
        ST_XML_SEQ *pstSeq;         /**< @brief Seq. */
    } u;
} ST_XML_CP;

/** @brief XML cp list. */
XML_TYPEDEF_LIST(XML_CP);

/** @brief XML choice. */
struct tagXML_CHOICE
{
    ST_XML_CP stCp;                  /**< @brief Cp. */
    ST_XML_CP_LST stCpLst;           /**< @brief Cp list. */
};

/** @brief XML seq. */
struct tagXML_SEQ
{
    ST_XML_CP stCp;                  /**< @brief Cp. */
    ST_XML_CP_LST stCpLst;           /**< @brief Cp list. */
};

/** @brief XML children. */
typedef struct tagXML_CHILD
{
    ZUCHAR ucChoicePres;             /**< @brief Choice present flag. */
    ZUCHAR ucSymPres;                /**< @brief Symbol present flag. */
    ZUCHAR ucSymType;                /**< @brief Symbol type EN_XML_SYM_TYPE. */
    ZUCHAR aucSpare[1];              /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_CHOICE stChoice;      /**< @brief Choice. */
        ST_XML_SEQ stSeq;            /**< @brief Seq. */
    } u;
} ST_XML_CHILD;

/** @brief XML contentspec. */
typedef struct tagXML_CONTENT_SPEC
{
    ZUCHAR ucType;                   /**< @brief Type EN_XML_CONTENTSPEC_TYPE. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_MIXED stMixed;        /**< @brief Mixed. */
        ST_XML_CHILD stChild;        /**< @brief Children. */
    } u;
} ST_XML_CONTENT_SPEC;

/** @brief XML elementdecl. */
typedef struct tagXML_ELEM_DECL
{
    ST_XML_QNAME stQName;            /**< @brief QName. */
    ST_XML_CONTENT_SPEC stContentSpec; /**< @brief Contentspec. */
} ST_XML_ELEM_DECL;

/** @brief XML CharData. */
typedef struct tagXML_CHR_DATA
{
    ST_XML_ELEM *pstParent;         /**< @brief Parent element. */
    ST_ZOS_USTR stData;             /**< @brief CharData. */
} ST_XML_CHR_DATA;

/** @brief XML Reference. */
typedef struct tagXML_REF
{
    ZUCHAR ucEntRefPres;            /**< @brief EntityRef present flag. */
    ZUCHAR ucXDigitPres;            /**< @brief Hexadecimal present flag. */
    ZUCHAR aucSpare[2];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_NAME stEntRef;       /**< @brief EntityRef. */
        ST_ZOS_USTR stChrRef;       /**< @brief CharRef. */
    } u;
} ST_XML_REF;

/** @brief XML AttValue item. */
typedef struct tagXML_AV_ITEM
{
    ZUCHAR ucRefPres;               /**< @brief Reference present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_ZOS_USTR stVal;          /**< @brief Value string. */
        ST_XML_REF stRef;           /**< @brief Reference. */
    } u;
} ST_XML_AV_ITEM;

/** @brief XML AttValue item list. */
XML_TYPEDEF_LIST(XML_AV_ITEM);

/** @brief XML AttValue. */
typedef struct tagXML_ATT_VAL
{
    ZUCHAR ucPairType;              /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR ucVStrPres;              /**< @brief Value string present flag. */
    ZUCHAR aucSpare[2];             /**< @brief For 32 bit alignment. */
    ST_XML_AV_ITEM_LST stItemLst;   /**< @brief AttValue item list. */
    ST_ZOS_USTR stValStr;           /**< @brief Value string */
} ST_XML_ATT_VAL;

/** @brief XML FIXED. */
typedef struct tagXML_FIXED
{
    ZUCHAR ucFixedPres;             /**< @brief Choice present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    ST_XML_ATT_VAL stVal;           /**< @brief AttValue). */
} ST_XML_FIXED;

/** @brief XML DefaultDecl. */
typedef struct tagXML_DFT_DECL
{
    ZUCHAR ucType;                   /**< @brief DefaultDecl type EN_XML_DFTDECL_TYPE. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    ST_XML_FIXED stFixed;            /**< @brief (('#FIXED' S)? AttValue). */
} ST_XML_DFT_DECL;

/** @brief XML NotationType. */
typedef struct tagXML_NOTATION_TYPE
{
    ST_XML_NAME stName;              /**< @brief Name. */
    ST_XML_NAME_LST stNameLst;       /**< @brief Name list. */
} ST_XML_NOTATION_TYPE;

/** @brief XML Enumeration. */
typedef struct tagXML_ENUM
{
    ST_XML_NMTKN stNmtkn;           /**< @brief Nmtoken. */
    ST_XML_NMTKN_LST stNmtknLst;    /**< @brief Nmtoken list. */
} ST_XML_ENUM;

/** @brief XML AttType. */
typedef struct tagXML_ATT_TYPE
{
    ZUCHAR ucType;                  /**< @brief AttType type EN_XML_ATT_TYPE_TYPE. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_NOTATION_TYPE stNotation; /**< @brief NotationType. */
        ST_XML_ENUM stEnum;         /**< @brief Enumeration. */
    } u;
} ST_XML_ATT_TYPE;

/** @brief XML AttDef. */
typedef struct tagXML_ATT_DEF
{
    ST_XML_QNAME stQName;           /**< @brief QName. */
    ST_XML_ATT_TYPE stType;         /**< @brief AttType. */
    ST_XML_DFT_DECL stDftDecl;      /**< @brief DefaultDecl. */
} ST_XML_ATT_DEF;

/** @brief XML AttDef list. */
XML_TYPEDEF_LIST(XML_ATT_DEF);

/** @brief XML AttlistDecl. */
typedef struct tagXML_ATTLST_DECL
{
    ST_XML_QNAME stQName;           /**< @brief QName. */
    ST_XML_ATT_DEF_LST stAttDefLst; /**< @brief AttDef list. */
} ST_XML_ATTLST_DECL;

/** @brief XML PEReference. */
typedef struct tagXML_PE_REF
{
    ST_XML_NAME stName;             /**< @brief Name. */
} ST_XML_PE_REF;

/** @brief XML EntityValue item. */
typedef struct tagXML_EV_ITEM
{
    ZUCHAR ucType;                  /**< 
                                      * @brief EntityValue type 
                                      *        EN_XML_ENT_VAL_TYPE.
                                    */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment */
    union
    {
        ST_ZOS_USTR stVal;          /**< @brief value string. */
        ST_XML_PE_REF stPeRef;      /**< @brief PEReference. */
        ST_XML_REF stRef;           /**< @brief Reference. */
    } u;
} ST_XML_EV_ITEM;

/** @brief XML EntityValue item list. */
XML_TYPEDEF_LIST(XML_EV_ITEM);

/** @brief XML EntityValue. */
typedef struct tagXML_ENT_VAL
{
    ZUCHAR ucPairType;              /**< @brief Pair type EN_XML_STR_PAIR_TYPE. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    ST_XML_EV_ITEM_LST stItemLst;   /**< @brief EntityValue item list. */
} ST_XML_ENT_VAL;

/** @brief XML NDataDecl. */
typedef struct tagXML_NDATA_DECL
{
    ZUCHAR ucPres;                  /**< @brief Present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    ST_XML_NAME stName;             /**< @brief Name. */
} ST_XML_NDATA_DECL;

/** @brief XML Entity External NDataDecl . */
typedef struct tagXML_ENT_EXTERNAL_DECL
{
    ST_XML_EXTERNAL_ID stExternalId; /**< @brief ExternalID. */
    ST_XML_NDATA_DECL stNDataDecl;  /**< @brief NDataDecl. */
} ST_XML_ENT_EXTERNAL_DECL;

/** @brief XML EntityDef. */
typedef struct tagXML_ENT_DEF
{
    ZUCHAR ucEntValPres;            /**< @brief EntityValue present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_ENT_VAL stEntVal;    /**< @brief EntityValue. */
        ST_XML_ENT_EXTERNAL_DECL stExternal; /**< @brief External NDataDecl. */
    } u;
} ST_XML_ENT_DEF;

/** @brief XML GEDecl. */
typedef struct tagXML_GE_DECL
{
    ST_XML_NAME stName;             /**< @brief Name. */
    ST_XML_ENT_DEF stEntDef;        /**< @brief EntityDef. */
} ST_XML_GE_DECL;

/** @brief XML PEDef. */
typedef struct tagXML_PE_DEF
{
    ZUCHAR ucEntValPres;            /**< @brief EntityValue present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_ENT_VAL stEntVal;    /**< @brief EntityValue */
        ST_XML_EXTERNAL_ID stExternalId; /**< @brief ExternalID. */
    } u;
} ST_XML_PE_DEF;

/** @brief XML PEDecl. */
typedef struct tagXML_PE_DECL
{
    ST_XML_NAME stName;             /**< @brief Name. */
    ST_XML_PE_DEF stPeDef;          /**< @brief PEDef. */
} ST_XML_PE_DECL;

/** @brief XML EntityDecl. */
typedef struct tagXML_ENT_DECL
{
    ZUCHAR ucGePres;                /**< @brief GEDecl present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_GE_DECL stGeDecl;    /**< @brief GEDecl. */
        ST_XML_PE_DECL stPeDecl;    /**< @brief PEDecl. */
    } u;
} ST_XML_ENT_DECL;

/** @brief XML NotationDecl. */
typedef struct tagXML_NOTATION_DECL
{
    ZUCHAR ucExternalIdPres;        /**< @brief ExternalId present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    ST_XML_NAME stName;             /**< @brief Name. */
    union
    {
        ST_XML_EXTERNAL_ID stExternalId; /**< @brief ExternalID. */
        ST_XML_PUB_ID stPubId;      /**< @brief PublicID */
    } u;
} ST_XML_NOTATION_DECL;
 
/**
 * @brief XML markupdecl (elementdecl | AttlistDecl | EntityDecl 
                   | NotationDecl | PI | Comment).
*/
typedef struct tagXML_MARKUP_DECL
{
    ZUCHAR ucType;                  /**<
                                      * @brief Markupdecl type 
                                      *         EN_XML_MARKDECL_TYPE.
                                    */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_ELEM_DECL stElem;    /**< @brief Elementdecl. */
        ST_XML_ATTLST_DECL stAtt;   /**< @brief AttlistDecl. */
        ST_XML_ENT_DECL stEnt;      /**< @brief EntityDecl. */
        ST_XML_NOTATION_DECL stNotation; /**< @brief NotationDecl. */
        ST_XML_PI stPi;             /**< @brief PI. */
        ST_XML_COMMENT stComment;   /**< @brief Comment. */
    } u;
} ST_XML_MARKUP_DECL;

/** @brief XML DeclSep. */
typedef struct tagXML_DECL_SEP
{
    ZUCHAR ucPeRefPres;             /**< @brief PEReference present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    ST_ZOS_USTR stName;             /**< @brief PEReference Name */
} ST_XML_DECL_SEP;

/** @brief XML intSubset ((markupdecl | DeclSep)*). */
typedef struct tagXML_INT_SUBSET
{
    ZUCHAR ucMarkupPres;            /**< @brief Markupdecl present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_MARKUP_DECL stMarkupDecl;/**< @brief Markupdecl. */
        ST_XML_DECL_SEP stDeclSep;  /**< @brief DeclSep. */
    } u;
} ST_XML_INT_SUBSET;

/** @brief XML intSubset list. */
XML_TYPEDEF_LIST(XML_INT_SUBSET);

/** @brief XML XMLDecl ('<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'). */
typedef struct tagXML_DECL
{
    ZUCHAR ucPres;                  /**< @brief Present flag. */
    ZUCHAR aucSpare[3];             /**< @brief For 32 bit alignment. */
    ST_XML_VER_INFO stVerInfo;      /**< @brief VersionInfo. */
    ST_XML_ENCODING_DECL stEncodingDecl;/**< @brief EncodingDecl. */
    ST_XML_SD_DECL stSdDecl;        /**< @brief SDDecl. */
} ST_XML_DECL;

/** @brief XML Attribute. */
typedef struct tagXML_ATTR
{
    ST_XML_ELEM *pstParent;         /**< @brief Parent element. */
    ST_XML_QNAME stQName;           /**< @brief QName. */
    ST_XML_ATT_VAL stVal;           /**< @brief AttValue. */
} ST_XML_ATTR;

/** @brief XML Attribute list. */
XML_TYPEDEF_LIST(XML_ATTR);

/** @brief XML EmptyElemTag. */
typedef struct tagXML_EMPTY_ELEM_TAG
{
    ST_XML_QNAME stQName;           /**< @brief QName. */
    ST_XML_ATTR_LST stAttrLst;      /**< @brief Attribute list. */
} ST_XML_EMPTY_ELEM_TAG;

/** @brief XML STag. */
typedef struct tagXML_STAG
{
    ST_XML_QNAME stQName;            /**< @brief QName. */
    ST_XML_ATTR_LST stAttrLst;       /**< @brief Attribute list. */
} ST_XML_STAG; 

/** @brief XML CDSect. */
typedef struct tagXML_CD_SECT
{
    ST_XML_ELEM *pstParent;          /**< @brief Parent element. */
    ST_ZOS_USTR stStr;               /**< @brief (Char* - (Char* ']]>' Char*)) */
} ST_XML_CD_SECT;

/** @brief XML content item. */
typedef struct tagXML_CONTENT_ITEM
{
    ZUCHAR ucType;                   /**< @brief Content type
                                                 EN_XML_CONTENT_ITEM_TYPE. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment */
    union
    {
        ST_XML_ELEM *pstElem;        /**< @brief Element. */
        ST_XML_CHR_DATA *pstChrData; /**< @brief CharData. */
        ST_XML_REF *pstRef;          /**< @brief Reference. */
        ST_XML_CD_SECT *pstCdSect;   /**< @brief CDSect. */
        ST_XML_PI *pstPi;            /**< @brief PI. */
        ST_XML_COMMENT *pstComment;  /**< @brief Comment. */
        ST_ZOS_USTR *pstIgnWS;       /**< @brief Ignorable whitespace. */
    } u;    
} ST_XML_CONTENT_ITEM;

/** @brief XML content item list. */
XML_TYPEDEF_LIST(XML_CONTENT_ITEM);

/** @brief XML content. */
typedef struct tagXML_CONTENT
{
    ST_XML_CONTENT_ITEM_LST stItemLst;/**< @brief Item list. */
} ST_XML_CONTENT;

/** @brief XML ETag. */
typedef struct tagXML_ETAG
{
    ST_XML_QNAME stQName;            /**< @brief QName. */
} ST_XML_ETAG;

/** @brief XML element tag. */
typedef struct tagXML_ELEM_TAG
{
    ST_XML_STAG stStag;              /**< @brief STag. */
    ST_XML_CONTENT stContent;        /**< @brief Content. */
    ST_ZOS_USTR stContentStr;        /**< @brief Content string. */
    ST_XML_ETAG stEtag;              /**< @brief ETag. */
} ST_XML_ELEM_TAG; 

/** @brief XML Misc (Comment | PI | S). */
typedef struct tagXML_MISC
{
    ZUCHAR ucType;                   /**< @brief Misc type EN_XML_MISC_TYPE. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    union
    {
        ST_XML_COMMENT stComment;    /**< @brief Comment. */
        ST_XML_PI stPi;              /**< @brief PI. */
    } u;
} ST_XML_MISC;

/** @brief XML Misc list. */
XML_TYPEDEF_LIST(XML_MISC);

/**
 * @brief XML doctypedecl ('<!DOCTYPE' S Name (S ExternalID)? 
 *                   S? ('[' intSubset ']' S?)? '>').
 */
typedef struct tagXML_DOCTYPE_DECL
{
    ZUCHAR ucPres;                    /**< @brief ]Present flag. */
    ZUCHAR aucSpare[3];               /**< @brief For 32 bit alignment. */
    ST_XML_QNAME stQName;             /**< @brief QName. */
    ST_XML_EXTERNAL_ID stExternalId;  /**< @brief ExternalID. */
    ST_XML_INT_SUBSET_LST stSubsetLst;/**< @brief IntSubset list. */
} ST_XML_DOCTYPE_DECL;

/** @brief XML prolog (XMLDecl? Misc* (doctypedecl Misc*)?). */
typedef struct tagXML_PROLOG
{
    ZUCHAR ucPres;                   /**< @brief Present flag. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    ST_XML_DECL stDecl;              /**< @brief XMLDecl. */
    ST_XML_MISC_LST stMiscLst1;      /**< @brief Misc list 1. */
    ST_XML_DOCTYPE_DECL stDoctypeDecl; /**< @brief Doctypedecl. */
    ST_XML_MISC_LST stMiscLst2;      /**< @brief Misc list  2*/
} ST_XML_PROLOG;

/** @brief XML element. */
struct tagXML_ELEM
{
    ZUCHAR ucPres;                   /**< @brief Present flag. */
    ZUCHAR ucStrPres;                /**< @brief String present for encode */
    ZUCHAR ucLStrPres;               /**< @brief Large string present for encode */
    ZUCHAR ucEmptyPres;              /**< @brief EmptyElemTag present flag. */
    ZVOID *pParent;                  /**< @brief Parent element. */
    ZVOID *pUserData;                /**< @brief User data for specific usage. */
    union
    {
        ST_XML_EMPTY_ELEM_TAG stEmptyElemTag; /**< @brief EmptyElemTag. */
        ST_XML_ELEM_TAG stElemTag;   /**< @brief Element tag. */
        ST_ZOS_USTR stElemStr;       /**< @brief element string(encode used). */
        ST_ZOS_SLIST stElemLStr;     /**< @brief element large string(encode used). */
    } u;
};

/** @brief XML document (prolog element Misc*). */
typedef struct tagXML_DOC
{
    ZUCHAR ucPres;                   /**< @brief Present flag. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    ST_XML_PROLOG stProlog;          /**< @brief Prolog. */
    ST_XML_ELEM stElem;              /**< @brief Element. */
    ST_XML_MISC_LST stMiscLst;       /**< @brief Misc list. */
} ST_XML_DOC;

/** @brief XML message. */
typedef struct tagXML_MSG
{
    ZUCHAR ucPres;                   /**< @brief Present flag. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    ZSBUF zMemBuf;                   /**< @brief Memory buffer. */
    ZEBUF zMsgBuf;                   /**< @brief Message buffer. */
    ST_ZOS_USTR stMsgStr;            /**< @brief Message string(decode used). */
    ST_XML_DOC stDoc;                /**< @brief XML document. */
} ST_XML_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _XML_TYPE_H__ */


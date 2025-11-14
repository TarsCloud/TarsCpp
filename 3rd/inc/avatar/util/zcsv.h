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
  File name     : zcsv.h
  Module        : comma separate values functions
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2009-01-15    
  Description   :
      Data structure and function definitions required by the 
      comma separate values interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZCSV_H__
#define _ZCSV_H__

#ifdef __cplusplus
extern "C" {
#endif

/* zcsv file encoding */
typedef enum EN_ZCSV_ENCODING_TYPE
{
    EN_ZCSV_ENCODING_UTF8,          /* utf-8 */
    EN_ZCSV_ENCODING_UTF16,         /* utf-16 */
    EN_ZCSV_ENCODING_GBK,           /* gb-18030 */
    EN_ZCSV_ENCODING_SIZE
} EN_ZCSV_ENCODING_TYPE;

/* zcsv outlook field type */
typedef enum EN_ZCSV_OUTLOOK_TYPE
{
    EN_ZCSV_OUTLOOK_TITLE,           /* "Title" */
    EN_ZCSV_OUTLOOK_FIRST_NAME,      /* "First Name" */
    EN_ZCSV_OUTLOOK_MIDDLE_NAME,     /* "Middle Name" */
    EN_ZCSV_OUTLOOK_LAST_NAME,       /* "Last Name" */
    EN_ZCSV_OUTLOOK_SUFFIX,          /* "Suffix" */
    EN_ZCSV_OUTLOOK_COMPANY,         /* "Company" */
    EN_ZCSV_OUTLOOK_DEPARTMENT,      /* "Department" */
    EN_ZCSV_OUTLOOK_JOB_TITLE,       /* "Job Title" */
    EN_ZCSV_OUTLOOK_BIZ_STREET,      /* "Business Street" */
    EN_ZCSV_OUTLOOK_BIZ_STREET2,     /* "Business Street 2" */
    EN_ZCSV_OUTLOOK_BIZ_STREET3,     /* "Business Street 3" */
    EN_ZCSV_OUTLOOK_BIZ_CITY,        /* "Business City" */
    EN_ZCSV_OUTLOOK_BIZ_STATE,       /* "Business State" */
    EN_ZCSV_OUTLOOK_BIZ_POST_CODE,   /* "Business Postal Code" */
    EN_ZCSV_OUTLOOK_BIZ_COUNTRY,     /* "Business Country" */
    EN_ZCSV_OUTLOOK_HOME_STREET,     /* "Home Street" */
    EN_ZCSV_OUTLOOK_HOME_STREET2,    /* "Home Street 2" */
    EN_ZCSV_OUTLOOK_HOME_STREET3,    /* "Home Street 3" */
    EN_ZCSV_OUTLOOK_HOME_CITY,       /* "Home City" */
    EN_ZCSV_OUTLOOK_HOME_STATE,      /* "Home State" */
    EN_ZCSV_OUTLOOK_HOME_POST_CODE,  /* "Home Postal Code" */
    EN_ZCSV_OUTLOOK_HOME_COUNTRY,    /* "Home Country" */
    EN_ZCSV_OUTLOOK_OTHER_STREET,    /* "Other Street" */
    EN_ZCSV_OUTLOOK_OTHER_STREET2,   /* "Other Street 2" */
    EN_ZCSV_OUTLOOK_OTHER_STREET3,   /* "Other Street 3" */
    EN_ZCSV_OUTLOOK_OTHER_CITY,      /* "Other City" */
    EN_ZCSV_OUTLOOK_OTHER_STATE,     /* "Other State" */
    EN_ZCSV_OUTLOOK_OTHER_POST_CODE, /* "Other Postal Code" */
    EN_ZCSV_OUTLOOK_OTHER_COUNTRY,   /* "Other Country" */
    EN_ZCSV_OUTLOOK_ASSIST_PHONE,    /* "Assistant's Phone" */
    EN_ZCSV_OUTLOOK_BIZ_FAX,         /* "Business Fax" */
    EN_ZCSV_OUTLOOK_BIZ_PHONE,       /* "Business Phone" */
    EN_ZCSV_OUTLOOK_BIZ_PHONE2,      /* "Business Phone 2" */
    EN_ZCSV_OUTLOOK_CALLBACK,        /* "Callback" */
    EN_ZCSV_OUTLOOK_CAR_PHONE,       /* "Car Phone" */
    EN_ZCSV_OUTLOOK_COMPANY_MAIN_PHONE, /* "Company Main Phone" */
    EN_ZCSV_OUTLOOK_HOME_FAX,        /* "Home Fax" */
    EN_ZCSV_OUTLOOK_HOME_PHONE,      /* "Home Phone" */
    EN_ZCSV_OUTLOOK_HOME_PHONE2,     /* "Home Phone 2" */
    EN_ZCSV_OUTLOOK_ISDN,            /* "ISDN" */
    EN_ZCSV_OUTLOOK_MOBILE_PHONE,    /* "Mobile Phone" */
    EN_ZCSV_OUTLOOK_OTHER_FAX,       /* "Other Fax" */
    EN_ZCSV_OUTLOOK_OTHER_PHONE,     /* "Other Phone" */
    EN_ZCSV_OUTLOOK_PAGER,           /* "Pager" */
    EN_ZCSV_OUTLOOK_PRIMARY_PHONE,   /* "Primary Phone" */
    EN_ZCSV_OUTLOOK_RADIO_PHONE,     /* "Radio Phone" */
    EN_ZCSV_OUTLOOK_TTY_PHONE,       /* "TTY/TDD Phone" */
    EN_ZCSV_OUTLOOK_TELEX,           /* "Telex" */
    EN_ZCSV_OUTLOOK_ACCOUNT,         /* "Account" */
    EN_ZCSV_OUTLOOK_ANNIVERSARY,     /* "Anniversary" */
    EN_ZCSV_OUTLOOK_ASSIST_NAME,     /* "Assistant's Name" */
    EN_ZCSV_OUTLOOK_BILL_INFO,       /* "Billing Information" */
    EN_ZCSV_OUTLOOK_BIRTHDAY,        /* "Birthday" */
    EN_ZCSV_OUTLOOK_BIZ_ADDR_PO_BOX, /* "Business Address PO Box" */
    EN_ZCSV_OUTLOOK_CATEGORY,        /* "Categories" */
    EN_ZCSV_OUTLOOK_CHILDREN,        /* "Children" */
    EN_ZCSV_OUTLOOK_DIR_SERV,        /* "Directory Server" */
    EN_ZCSV_OUTLOOK_EMAIL_ADDR,      /* "E-mail Address" */
    EN_ZCSV_OUTLOOK_EMAIL_TYPE,      /* "E-mail Type" */
    EN_ZCSV_OUTLOOK_EMAIL_DISP_NAME, /* "E-mail Display Name" */
    EN_ZCSV_OUTLOOK_EMAIL2_ADDR,     /* "E-mail 2 Address" */
    EN_ZCSV_OUTLOOK_EMAIL2_TYPE,     /* "E-mail 2 Type" */
    EN_ZCSV_OUTLOOK_EMAIL2_DISP_NAME, /* "E-mail 2 Display Name" */
    EN_ZCSV_OUTLOOK_EMAIL3_ADDR,     /* "E-mail 3 Address" */
    EN_ZCSV_OUTLOOK_EMAIL3_TYPE,     /* "E-mail 3 Type" */
    EN_ZCSV_OUTLOOK_EMAIL3_DISP_NAME, /* "E-mail 3 Display Name" */
    EN_ZCSV_OUTLOOK_GENDER,          /* "Gender" */
    EN_ZCSV_OUTLOOK_GOVER_ID_NUMBER, /* "Government ID Number" */
    EN_ZCSV_OUTLOOK_HOBBY,           /* "Hobby" */
    EN_ZCSV_OUTLOOK_HOME_ADDR_PO_BOX,/* "Home Address PO Box" */
    EN_ZCSV_OUTLOOK_INITIALS,        /* "Initials" */
    EN_ZCSV_OUTLOOK_INTERNET_FREE_BUSY, /* "Internet Free Busy" */
    EN_ZCSV_OUTLOOK_KEYWORDS,        /* "Keywords" */
    EN_ZCSV_OUTLOOK_LANG,            /* "Language" */
    EN_ZCSV_OUTLOOK_LOCATION,        /* "Location" */
    EN_ZCSV_OUTLOOK_MANAGER_NAME,    /* "Manager's Name" */
    EN_ZCSV_OUTLOOK_MILEAGE,         /* "Mileage" */
    EN_ZCSV_OUTLOOK_NOTES,           /* "Notes" */
    EN_ZCSV_OUTLOOK_OFFICE_LOCATION, /* "Office Location" */
    EN_ZCSV_OUTLOOK_ORG_ID_NUMBER,   /* "Organizational ID Number" */
    EN_ZCSV_OUTLOOK_OTHER_ADDR_PO_BOX, /* "Other Address PO Box" */
    EN_ZCSV_OUTLOOK_PRIORITY,        /* "Priority" */
    EN_ZCSV_OUTLOOK_PRIVATE,         /* "Private" */
    EN_ZCSV_OUTLOOK_PROFESSION,      /* "Profession" */
    EN_ZCSV_OUTLOOK_REFER_BY,        /* "Referred By" */
    EN_ZCSV_OUTLOOK_SENSITIVITY,     /* "Sensitivity" */
    EN_ZCSV_OUTLOOK_SPOUSE,          /* "Spouse" */
    EN_ZCSV_OUTLOOK_USER1,           /* "User 1" */
    EN_ZCSV_OUTLOOK_USER2,           /* "User 2" */
    EN_ZCSV_OUTLOOK_USER3,           /* "User 3" */
    EN_ZCSV_OUTLOOK_USER4,           /* "User 4" */
    EN_ZCSV_OUTLOOK_WEB_PAGE,        /* "Web Page */
    EN_ZCSV_OUTLOOK_UNKNOWN,         /* unknown type */
} EN_ZCSV_OUTLOOK_TYPE;

/* zcsv id */
typedef ZVOID * ZCSV;

/* zcsv create */
ZFUNC ZINT Zcsv_Create(ZCSV *pzCsv);

/* zcsv delete */
ZFUNC ZINT Zcsv_Delete(ZCSV zCsv);

/* zcsv load from file */
ZFUNC ZINT Zcsv_LoadFile(ZCHAR *pcFileName, ZCSV *pzCsv);

/* zcsv load save file */
ZFUNC ZINT Zcsv_SaveFile(ZCSV zCsv, ZCHAR *pcFileName, ZUINT iEncoding);

/* zcsv get line size(start from line 1, also include line 1) */
ZFUNC ZINT Zcsv_GetLineSize(ZCSV zCsv, ZUINT *piSize);

/* zcsv get header field(line 1) size, */
ZFUNC ZINT Zcsv_GetHdrFieldSize(ZCSV zCsv, ZUINT *piSize);

/* zcsv get header field(line 1) value, type see EN_ZCSV_OUTLOOK_TYPE */
ZFUNC ZINT Zcsv_GetHdrField(ZCSV zCsv, ZUINT iFieldNo, ZUINT *piType, 
                ST_ZOS_SSTR *pstStr);

/* zcsv get line field size */
ZFUNC ZINT Zcsv_GetLineFieldSize(ZCSV zCsv, ZUINT iLineNo,
                ZUINT *piSize);

/* zcsv get line field value */
ZFUNC ZINT Zcsv_GetLineField(ZCSV zCsv, ZUINT iLineNo, ZUINT iFieldNo, 
                ST_ZOS_SSTR *pstStr);

/* zcsv get line field by type */
ZFUNC ZINT Zcsv_GetLineFieldByType(ZCSV zCsv, ZUINT iLineNo,
                ZUINT iType, ST_ZOS_SSTR *pstStr);

/* zcsv put header field(line 1) value, type see EN_ZCSV_OUTLOOK_TYPE */
ZFUNC ZINT Zcsv_PutHdrField(ZCSV zCsv, ZUINT iFieldNo, ZUINT iType);

/* zcsv put header field(line 1) value */
ZFUNC ZINT Zcsv_PutHdrFieldX(ZCSV zCsv, ZUINT iFieldNo, 
                ST_ZOS_SSTR *pstStr);

/* zcsv put line field(start from line 1, also include line 1) */
ZFUNC ZINT Zcsv_PutLineField(ZCSV zCsv, ZUINT iLineNo, ZUINT iFieldNo, 
                ST_ZOS_SSTR *pstStr);

/* zcsv put line field by type */
ZFUNC ZINT Zcsv_PutLineFieldByType(ZCSV zCsv, ZUINT iLineNo,
                ZUINT iType, ST_ZOS_SSTR *pstStr);

#ifdef __cplusplus
}
#endif

#endif /* _ZCSV_H__ */


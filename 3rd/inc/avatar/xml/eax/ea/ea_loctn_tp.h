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
  File name     : ea_loctn_tp.h
  Module        : eax location type
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax location type.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_LOCTN_TP_H__
#define _EA_LOCTN_TP_H__

#ifdef __cplusplus
extern "C"{
#endif

/************* urn:ietf:params:xml:ns:location-type *********/
typedef enum EN_EA_LOCTN_TPTN_TP_TYPE
{
    EN_EA_LOCTN_TP_AIRCRAFT,
    EN_EA_LOCTN_TP_AIRPORT,
    EN_EA_LOCTN_TP_ARENA,
    EN_EA_LOCTN_TP_AUTOMOBILE,
    EN_EA_LOCTN_TP_BANK,
    EN_EA_LOCTN_TP_BAR,
    EN_EA_LOCTN_TP_BICYLE,
    EN_EA_LOCTN_TP_BUS,
    EN_EA_LOCTN_TP_BUS_STATION,
    EN_EA_LOCTN_TP_CAFE,
    EN_EA_LOCTN_TP_CLASSROOM,
    EN_EA_LOCTN_TP_CLUB,
    EN_EA_LOCTN_TP_CONSTRUCTION,
    EN_EA_LOCTN_TP_CONVENTION_CENTER,
    EN_EA_LOCTN_TP_GOVERNMENT,
    EN_EA_LOCTN_TP_HOSPITAL,
    EN_EA_LOCTN_TP_HOTEL,
    EN_EA_LOCTN_TP_INDUSTRIAL,
    EN_EA_LOCTN_TP_LIBRARY,
    EN_EA_LOCTN_TP_MOTORCYCLE,
    EN_EA_LOCTN_TP_OFFICE,
    EN_EA_LOCTN_TP_OUTDOORS,
    EN_EA_LOCTN_TP_PARKING,
    EN_EA_LOCTN_TP_PLACE_OF_WORSHIP,
    EN_EA_LOCTN_TP_PRISON,
    EN_EA_LOCTN_TP_PUBLIC,
    EN_EA_LOCTN_TP_PUBLIC_TRANSPORT,
    EN_EA_LOCTN_TP_RESIDENCE,
    EN_EA_LOCTN_TP_RESTAURANT,
    EN_EA_LOCTN_TP_SCHOOL,
    EN_EA_LOCTN_TP_SHOPPING_AREA,
    EN_EA_LOCTN_TP_STADIUM,
    EN_EA_LOCTN_TP_STORE,
    EN_EA_LOCTN_TP_STREET,
    EN_EA_LOCTN_TP_THEATER,
    EN_EA_LOCTN_TP_TRAIN,
    EN_EA_LOCTN_TP_TRAIN_STATION,
    EN_EA_LOCTN_TP_TRUCK,
    EN_EA_LOCTN_TP_UNDERWAY,
    EN_EA_LOCTN_TP_UNKNOWN,
    EN_EA_LOCTN_TP_WAREHOUSE,
    EN_EA_LOCTN_TP_WATER,
    EN_EA_LOCTN_TP_WATERCRAFT 
} EN_EA_LOCTN_TPTN_TP_TYPE;

/* location type get type */
ZFUNC ZINT EaLoctn_TpGetType(ST_XML_ELEM *pstElem, 
                ZUINT *piType);

/* location type set type */
ZFUNC ZINT EaLoctn_TpSetType(ST_XML_ELEM *pstElem, 
                ZUINT iType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_LOCTN_TP_H__ */


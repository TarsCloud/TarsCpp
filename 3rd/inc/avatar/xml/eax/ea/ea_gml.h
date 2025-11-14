/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : ea_gml.h
  Module        : eax gml
  Author        : zocol
  Version       : 0.1
  Created on    : 2013-01-20
  Description   :
    Marcos and structure definitions required by the eax gml.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_GML_H__
#define _EA_GML_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** http://www.opengis.net/gml **************/
typedef enum EN_EA_GML_TYPE
{
    EN_EA_GML_POS,                  /* pos */
    EN_EA_GML_POINT,                /* Point */
    EN_EA_GML_RADIUS,               /* radius */
    EN_EA_GML_POSLIST,              /* posList */
    EN_EA_GML_SRSNAME,              /* srsName */
    EN_EA_GML_POLYGON,              /* Polygon */
    EN_EA_GML_EXTERIOR,             /* exterior */
    EN_EA_GML_LINEARRING,           /* LinearRing */
} EN_EA_GML_TYPE;

/* eax gml get pos */
ZFUNC ZINT EaGml_GetPos(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstPos);

/* eax gml get Point */
ZFUNC ZINT EaGml_GetPoint(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPoint);

/* eax gml get Polygon */
ZFUNC ZINT EaGml_GetPolygon(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPolygon);

/* eax gml get Point crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGml_PointGetCrsType(ST_XML_ELEM *pstPoint, 
                ZUCHAR *pucType);

/* eax gml get Point pos */
ZFUNC ZINT EaGml_PointGetPos(ST_XML_ELEM *pstPoint, 
                ST_ZOS_USTR **ppstPos);

/* eax gml get Polygon crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGml_PolygonGetCrsType(ST_XML_ELEM *pstPolygon, 
                ZUCHAR *pucType);

/* eax gml get Polygon exterior */
ZFUNC ZINT EaGml_PolygonGetExterior(ST_XML_ELEM *pstPolygon, 
                ST_XML_ELEM **ppstExterior);

/* eax gml get exterior LinearRing */
ZFUNC ZINT EaGml_ExteriorGetLinearRing(ST_XML_ELEM *pstExterior, 
                ST_XML_ELEM **ppstLinearRing);

/* eax gml get LinearRing pos */
ZFUNC ZINT EaGml_LinearRingGetFirstPos(ST_XML_ELEM *pstLinearRing, 
                ST_ZOS_USTR **ppstPos);

/* eax gml get LinearRing pos */
ZFUNC ZINT EaGml_LinearRingGetNextPos(ST_XML_ELEM *pstLinearRing, 
                ST_ZOS_USTR **ppstPos);

/* eax gml get LinearRing posList */
ZFUNC ZINT EaGml_LinearRingGetPosList(ST_XML_ELEM *pstLinearRing, 
                ST_ZOS_USTR **ppstPosLst);

/* eax gml set pos */
ZFUNC ZINT EaGml_SetPos(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstPos);

/* eax gml set Point */
ZFUNC ZINT EaGml_SetPoint(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPoint);

/* eax gml set Polygon */
ZFUNC ZINT EaGml_SetPolygon(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPolygon);

/* eax gml set Point crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGml_PointSetCrsType(ST_XML_ELEM *pstPoint, 
                ZUCHAR ucType);

/* eax gml set Point pos */
ZFUNC ZINT EaGml_PointSetPos(ST_XML_ELEM *pstPoint, 
                ST_ZOS_USTR *pstPos);

/* eax gml set Polygon crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGml_PolygonSetCrsType(ST_XML_ELEM *pstPolygon, 
                ZUCHAR ucType);

/* eax gml set Polygon exterior */
ZFUNC ZINT EaGml_PolygonSetExterior(ST_XML_ELEM *pstPolygon, 
                ST_XML_ELEM **ppstExterior);

/* eax gml set exterior LinearRing */
ZFUNC ZINT EaGml_ExteriorSetLinearRing(ST_XML_ELEM *pstExterior, 
                ST_XML_ELEM **ppstLinearRing);

/* eax gml set LinearRing pos */
ZFUNC ZINT EaGml_LinearRingSetPos(ST_XML_ELEM *pstLinearRing, 
                ST_ZOS_USTR *pstPos);

/* eax gml set LinearRing posList */
ZFUNC ZINT EaGml_LinearRingSetPosList(ST_XML_ELEM *pstLinearRing, 
                ST_ZOS_USTR *pstPosLst);

#ifdef __cplusplus
}
#endif

#endif /* _EA_GML_H__ */


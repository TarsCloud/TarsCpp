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
  File name     : ea_gs.h
  Module        : eax gs
  Author        : zocol
  Version       : 0.1
  Created on    : 2013-01-17
  Description   :
    Marcos and structure definitions required by the eax gs.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_GS_H__
#define _EA_GS_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** http://www.opengis.net/pidflo/1.0 **************/
typedef enum EN_EA_GS_TYPE
{
    EN_EA_GS_CIRCLE,                /* Circle */
    EN_EA_GS_ELLIPSE,               /* Ellipse */
    EN_EA_GS_ARCBAND,               /* ArcBand */
    EN_EA_GS_SPHERE,                /* Sphere */
    EN_EA_GS_ELLIPOSID,             /* Ellipsoid */
    EN_EA_GS_PRISM,                 /* Prism */
    EN_EA_GS_SEMIMAJORAXIS,         /* semiMajorAxis */
    EN_EA_GS_SEMIMINORAXIS,         /* semiMinorAxis */
    EN_EA_GS_VERTICALAXIS,          /* verticalAxis */
    EN_EA_GS_INNERRADIUS,           /* innerRadius */
    EN_EA_GS_OUTERRADIUS,           /* outerRadius */
    EN_EA_GS_ORIENTATION,           /* orientation */
    EN_EA_GS_STARTANGLE,            /* startAngle */
    EN_EA_GS_OPENINGANGLE,          /* openingAngle */
    EN_EA_GS_RADIUS,                /* radius */
    EN_EA_GS_BASE,                  /* base */
    EN_EA_GS_HEIGHT,                /* height */
} EN_EA_GS_TYPE;

/* eax gs get Circle */
ZFUNC ZINT EaGs_GetCircle(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstCircle);

/* eax gs get Ellipse */
ZFUNC ZINT EaGs_GetEllipse(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstEllipse);

/* eax gs get ArcBand */
ZFUNC ZINT EaGs_GetArcBand(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstArcBand);

/* eax gs get Sphere */
ZFUNC ZINT EaGs_GetSphere(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSphere);

/* eax gs get Ellipsoid */
ZFUNC ZINT EaGs_GetEllipsoid(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstEllipsoid);

/* eax gs get Prism */
ZFUNC ZINT EaGs_GetPrism(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPrism);

/* eax gs get Circle crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_CircleGetCrsType(ST_XML_ELEM *pstCircle, 
                ZUCHAR *pucType);

/* eax gs get Circle radius */
ZFUNC ZINT EaGs_CircleGetRadius(ST_XML_ELEM *pstCircle, 
                ST_XML_ELEM **ppstRadius);

/* eax gs get Ellipse crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_EllipseGetCrsType(ST_XML_ELEM *pstEllipse, 
                ZUCHAR *pucType);

/* eax gs get Ellipse semiMajorAxis */
ZFUNC ZINT EaGs_EllipseGetSemiMajorAxis(ST_XML_ELEM *pstEllipse, 
                ST_XML_ELEM **ppstSemiMajorAxis);

/* eax gs get Ellipse semiMinorAxis */
ZFUNC ZINT EaGs_EllipseGetSemiMinorAxis(ST_XML_ELEM *pstEllipse, 
                ST_XML_ELEM **ppstSemiMinorAxis);

/* eax gs get Ellipse orientation */
ZFUNC ZINT EaGs_EllipseGetOrientation(ST_XML_ELEM *pstEllipse, 
                ST_XML_ELEM **ppstOrientation);

/* eax gs get ArcBand crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_ArcBandGetCrsType(ST_XML_ELEM *pstArcBand, 
                ZUCHAR *pucType);

/* eax gs get ArcBand innerRadius */
ZFUNC ZINT EaGs_ArcBandGetInnerRadius(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstInnerRadius);

/* eax gs get ArcBand outerRadius */
ZFUNC ZINT EaGs_ArcBandGetOuterRadius(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstOuterRadius);

/* eax gs get ArcBand startAngle */
ZFUNC ZINT EaGs_ArcBandGetStartAngle(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstStartAngle);

/* eax gs get ArcBand openingAngle */
ZFUNC ZINT EaGs_ArcBandGetOpeningAngle(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstOpeningAngle);

/* eax gs get Sphere crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_SphereGetCrsType(ST_XML_ELEM *pstSphere, 
                ZUCHAR *pucType);

/* eax gs get Sphere radius */
ZFUNC ZINT EaGs_SphereGetRadius(ST_XML_ELEM *pstSphere, 
                ST_XML_ELEM **ppstRadius);

/* eax gs get Ellipsoid crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_EllipsoidGetCrsType(ST_XML_ELEM *pstEllipsoid, 
                ZUCHAR *pucType);

/* eax gs get Ellipsoid semiMajorAxis */
ZFUNC ZINT EaGs_EllipsoidGetSemiMajorAxis(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstSemiMajorAxis);

/* eax gs get Ellipsoid semiMinorAxis */
ZFUNC ZINT EaGs_EllipsoidGetSemiMinorAxis(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstSemiMinorAxis);

/* eax gs get Ellipsoid verticalAxis */
ZFUNC ZINT EaGs_EllipsoidGetVerticalAxis(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstVerticalAxis);

/* eax gs get Ellipsoid orientation */
ZFUNC ZINT EaGs_EllipsoidGetOrientation(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstOrientation);

/* eax gs get Prism crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_PrismGetCrsType(ST_XML_ELEM *pstPrism, 
                ZUCHAR *pucType);

/* eax gs get Prism base */
ZFUNC ZINT EaGs_PrismGetBase(ST_XML_ELEM *pstPrism, 
                ST_XML_ELEM **ppstBase);

/* eax gs get Prism height */
ZFUNC ZINT EaGs_PrismGetHeight(ST_XML_ELEM *pstPrism, 
                ST_XML_ELEM **ppstHeight);

/* eax gs get radius uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_RadiusGetUomType(ST_XML_ELEM *pstRadius, 
                ZUCHAR *pucType);

/* eax gs get semiMajorAxis uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_SemiMajorAxisGetUomType(ST_XML_ELEM *pstSemiMajorAxis, 
                ZUCHAR *pucType);

/* eax gs get semiMinorAxis uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_SemiMinorAxisGetUomType(ST_XML_ELEM *pstSemiMinorAxis, 
                ZUCHAR *pucType);

/* eax gs get verticalAxis uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_VerticalAxisGetUomType(ST_XML_ELEM *pstVerticalAxis, 
                ZUCHAR *pucType);

/* eax gs get orientation uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_OrientationGetUomType(ST_XML_ELEM *pstOrientation, 
                ZUCHAR *pucType);

/* eax gs get innerRadius uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_InnerRadiusGetUomType(ST_XML_ELEM *pstInnerRadius, 
                ZUCHAR *pucType);

/* eax gs get outerRadius uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_OuterRadiusGetUomType(ST_XML_ELEM *pstOuterRadius, 
                ZUCHAR *pucType);

/* eax gs get startAngle uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_StartAngleGetUomType(ST_XML_ELEM *pstStartAngle, 
                ZUCHAR *pucType);

/* eax gs get openingAngle uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_OpeningAngleGetUomType(ST_XML_ELEM *pstOpeningAngle, 
                ZUCHAR *pucType);

/* eax gs get base uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_BaseGetUomType(ST_XML_ELEM *pstBase, 
                ZUCHAR *pucType);

/* eax gs get height uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_HeightGetUomType(ST_XML_ELEM *pstHeight, 
                ZUCHAR *pucType);

/* eax gs set Circle */
ZFUNC ZINT EaGs_SetCircle(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstCircle);

/* eax gs set Ellipse */
ZFUNC ZINT EaGs_SetEllipse(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstEllipse);

/* eax gs set ArcBand */
ZFUNC ZINT EaGs_SetArcBand(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstArcBand);

/* eax gs set Sphere */
ZFUNC ZINT EaGs_SetSphere(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSphere);

/* eax gs set Ellipsoid */
ZFUNC ZINT EaGs_SetEllipsoid(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstEllipsoid);

/* eax gs set Prism */
ZFUNC ZINT EaGs_SetPrism(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPrism);

/* eax gs set Circle crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_CircleSetCrsType(ST_XML_ELEM *pstCircle, 
                ZUCHAR ucType);

/* eax gs set Circle radius */
ZFUNC ZINT EaGs_CircleSetRadius(ST_XML_ELEM *pstCircle, 
                ST_XML_ELEM **ppstRadius);

/* eax gs set Ellipse crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_EllipseSetCrsType(ST_XML_ELEM *pstEllipse, 
                ZUCHAR ucType);

/* eax gs set Ellipse semiMajorAxis */
ZFUNC ZINT EaGs_EllipseSetSemiMajorAxis(ST_XML_ELEM *pstEllipse, 
                ST_XML_ELEM **ppstSemiMajorAxis);

/* eax gs set Ellipse semiMinorAxis */
ZFUNC ZINT EaGs_EllipseSetSemiMinorAxis(ST_XML_ELEM *pstEllipse, 
                ST_XML_ELEM **ppstSemiMinorAxis);

/* eax gs set Ellipse orientation */
ZFUNC ZINT EaGs_EllipseSetOrientation(ST_XML_ELEM *pstEllipse, 
                ST_XML_ELEM **ppstOrientation);

/* eax gs set ArcBand crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_ArcBandSetCrsType(ST_XML_ELEM *pstArcBand, 
                ZUCHAR ucType);

/* eax gs set ArcBand innerRadius */
ZFUNC ZINT EaGs_ArcBandSetInnerRadius(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstInnerRadius);

/* eax gs set ArcBand outerRadius */
ZFUNC ZINT EaGs_ArcBandSetOuterRadius(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstOuterRadius);

/* eax gs set ArcBand startAngle */
ZFUNC ZINT EaGs_ArcBandSetStartAngle(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstStartAngle);

/* eax gs set ArcBand openingAngle */
ZFUNC ZINT EaGs_ArcBandSetOpeningAngle(ST_XML_ELEM *pstArcBand, 
                ST_XML_ELEM **ppstOpeningAngle);

/* eax gs set Sphere crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_SphereSetCrsType(ST_XML_ELEM *pstSphere, 
                ZUCHAR ucType);

/* eax gs set Sphere radius */
ZFUNC ZINT EaGs_SphereSetRadius(ST_XML_ELEM *pstSphere, 
                ST_XML_ELEM **ppstRadius);

/* eax gs set Ellipsoid crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_EllipsoidSetCrsType(ST_XML_ELEM *pstEllipsoid, 
                ZUCHAR ucType);

/* eax gs set Ellipsoid semiMajorAxis */
ZFUNC ZINT EaGs_EllipsoidSetSemiMajorAxis(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstSemiMajorAxis);

/* eax gs set Ellipsoid semiMinorAxis */
ZFUNC ZINT EaGs_EllipsoidSetSemiMinorAxis(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstSemiMinorAxis);

/* eax gs set Ellipsoid verticalAxis */
ZFUNC ZINT EaGs_EllipsoidSetVerticalAxis(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstVerticalAxis);

/* eax gs set Ellipsoid orientation */
ZFUNC ZINT EaGs_EllipsoidSetOrientation(ST_XML_ELEM *pstEllipsoid, 
                ST_XML_ELEM **ppstOrientation);

/* eax gs set Prism crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT EaGs_PrismSetCrsType(ST_XML_ELEM *pstPrism, 
                ZUCHAR ucType);

/* eax gs set Prism base */
ZFUNC ZINT EaGs_PrismSetBase(ST_XML_ELEM *pstPrism, 
                ST_XML_ELEM **ppstBase);

/* eax gs set Prism height */
ZFUNC ZINT EaGs_PrismSetHeight(ST_XML_ELEM *pstPrism, 
                ST_XML_ELEM **ppstHeight);

/* eax gs get radius uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_RadiusSetUomType(ST_XML_ELEM *pstRadius, 
                ZUCHAR ucType);

/* eax gs get semiMajorAxis uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_SemiMajorAxisSetUomType(ST_XML_ELEM *pstSemiMajorAxis, 
                ZUCHAR ucType);

/* eax gs get semiMinorAxis uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_SemiMinorAxisSetUomType(ST_XML_ELEM *pstSemiMinorAxis, 
                ZUCHAR ucType);

/* eax gs get verticalAxis uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_VerticalAxisSetUomType(ST_XML_ELEM *pstVerticalAxis, 
                ZUCHAR ucType);

/* eax gs get orientation uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_OrientationSetUomType(ST_XML_ELEM *pstOrientation, 
                ZUCHAR ucType);

/* eax gs get innerRadius uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_InnerRadiusSetUomType(ST_XML_ELEM *pstInnerRadius, 
                ZUCHAR ucType);

/* eax gs get outerRadius uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_OuterRadiusSetUomType(ST_XML_ELEM *pstOuterRadius, 
                ZUCHAR ucType);

/* eax gs get startAngle uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_StartAngleSetUomType(ST_XML_ELEM *pstStartAngle, 
                ZUCHAR ucType);

/* eax gs get openingAngle uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_OpeningAngleSetUomType(ST_XML_ELEM *pstOpeningAngle, 
                ZUCHAR ucType);

/* eax gs get base uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_BaseSetUomType(ST_XML_ELEM *pstBase, 
                ZUCHAR ucType);

/* eax gs get height uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT EaGs_HeightSetUomType(ST_XML_ELEM *pstHeight, 
                ZUCHAR ucType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_GS_H__ */


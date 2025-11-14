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
  File name     : ea_gsma_shareddata.h
  Module        : eax gsma shareddata
  Author        : shuai.xu
  Version       : 0.1
  Created on    : 2017-04-27
  Description   :
    Marcos and structure definitions required by the ea gsma shareddata.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_GSMA_SHAREDDATA_H__
#define _EA_GSMA_SHAREDDATA_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:gsma:params:xml:ns:rcs:rcs:shareddata **************/
typedef enum EN_EA_GSMA_SHAREDDATA_TYPE
{
    EN_EA_GSMA_SHAREDDATA_ACTIONS,         /* action */
    EN_EA_GSMA_SHAREDDATA_SEQ, /* seq */
    EN_EA_GSMA_SHAREDDATA_VERSION,       /* version*/
    EN_EA_GSMA_SHAREDDATA_CLOSE,          /* close */
    EN_EA_GSMA_SHAREDDATA_ID,                /* id */
    EN_EA_GSMA_SHAREDDATA_DRAWING,        /* drawing */
    EN_EA_GSMA_SHAREDDATA_WIDTH,    /* width */
    EN_EA_GSMA_SHAREDDATA_COLOUR,            /* color */
    EN_EA_GSMA_SHAREDDATA_ERASE,        /* earse */
    EN_EA_GSMA_SHAREDDATA_UNDO,         /* undo */
    EN_EA_GSMA_SHAREDDATA_POINTS,       /* points */
    EN_EA_GSMA_SHAREDDATA_POINT,        /* point */
    EN_EA_GSMA_SHAREDDATA_ENCODING,  /* encoding */
    EN_EA_GSMA_SHAREDDATA_BOUNDS,     /* bounds */
    EN_EA_GSMA_SHAREDDATA_MARKER,    /* marker */
    EN_EA_GSMA_SHAREDDATA_TITLE,        /* title */
    EN_EA_GSMA_SHAREDDATA_SNIPPET,   /* snippet */
    EN_EA_GSMA_SHAREDDATA_REMOVE,   /* remove */
    EN_EA_GSMA_SHAREDDATA_USER,     /* user */
    EN_EA_GSMA_SHAREDDATA_BACKGROUND_COLOUR, /* background_colour */
    EN_EA_GSMA_SHAREDDATA_IMAGE,        /* image */
} EN_EA_GSMA_SHAREDDATA_TYPE;

/* eax gsma shareddata get token id from string */
ZFUNC ZINT EaGsmaSharedData_TknStr2Id(ST_ZOS_USTR *pstStr, ZTKNID *pzTknId);

/* eax gsma shareddata get token string from id */
ZFUNC ZINT EaGsmaSharedData_TknId2Str(ZTKNID zTknId, ST_ZOS_USTR *pstStr);
                
/* eax gsma shareddata get the action */
ZFUNC ZINT EaGsmaSharedData_GetActions(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstAction);

/* eax gsma shareddata actions get the seq */
ZFUNC ZINT EaGsmaSharedData_ActionsGetSeq(ST_XML_ELEM *pstActions, 
                ZUINT *piSeq);

/* eax gsma shareddata action get the first element */
ZFUNC ZINT EaGsmaSharedData_ActionGetFirst(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstElem);

/* eax gsma shareddata action get the next element */
ZFUNC ZINT EaGsmaSharedData_ActionGetNext(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstElem);

/* eax gsma shareddata version get the id */
ZFUNC ZINT EaGsmaSharedData_VersionGetId(ST_XML_ELEM *pstVersion, 
                ZUINT *piId);
                
/* eax gsma shareddata get the action */
ZFUNC ZINT EaGsmaSharedData_ActionGetDrawing(ST_XML_ELEM *pstAction, 
               ST_XML_ELEM **ppstDrawing);

/* eax gsma shareddata drawing get the width */
ZFUNC ZINT EaGsmaSharedData_DrawingGetWidth(ST_XML_ELEM *pstDrawing, 
                ZFLOAT *piWidth);

/* eax gsma shareddata drawing get the colour */
ZFUNC ZINT EaGsmaSharedData_DrawingGetColour(ST_XML_ELEM *pstDrawing, 
                ZUINT *piColour);

/* eax gsma shareddata drawing get the earse */
ZFUNC ZINT EaGsmaSharedData_DrawingGetErase(ST_XML_ELEM *pstDrawing, 
                ZBOOL *pbErase);

/* eax gsma shareddata drawing get the points */
ZFUNC ZINT EaGsmaSharedData_DrawingGetPoints(ST_XML_ELEM *pstDrawing, 
                ST_XML_ELEM **pstPoints);

/* eax gsma shareddata points get the encoding */
ZFUNC ZINT EaGsmaSharedData_PointsGetEncoding(ST_XML_ELEM *pstPotins, 
                ST_ZOS_USTR **ppstEncoding);

/* eax gsma shareddata points get the data*/
ZFUNC ZINT EaGsmaSharedData_PointsGetData(ST_XML_ELEM *pstPotins, 
                ST_ZOS_USTR **ppstData);

/* eax gsma shareddata get the undo */
ZFUNC ZINT EaGsmaSharedData_ActionGetUndo(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstUndo);

/* eax gsma shareddata get the bounds */
ZFUNC ZINT EaGsmaSharedData_ActionGetBounds(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstBounds);

/* eax gsma shareddata bounds get the points */
ZFUNC ZINT EaGsmaSharedData_BoundsGetPoints(ST_XML_ELEM *pstDrawing, 
                ST_XML_ELEM **pstPoints);

/* eax gsma shareddata get the marker */
ZFUNC ZINT EaGsmaSharedData_ActionGetMarker(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstMarker);

/* rcs gsma shareddata marker get the title data*/
ZFUNC ZINT EaGsmaSharedData_MarkerGetTitleData(ST_XML_ELEM *pstMarker, 
                ST_ZOS_USTR **ppstTitle);

/* rcs gsma shareddata marker get the snippet data */
ZFUNC ZINT EaGsmaSharedData_MarkerGetSnippetData(ST_XML_ELEM *pstMarker, 
                ST_ZOS_USTR **ppstSnippet);

/* eax gsma shareddata marker get the point */
ZFUNC ZINT EaGsmaSharedData_MarkerGetPoint(ST_XML_ELEM *pstMarker, 
                ST_XML_ELEM **ppstPoint);

/* eax gsma shareddata point get the encoding */
ZFUNC ZINT EaGsmaSharedData_PointGetEncoding(ST_XML_ELEM *pstPoint, 
                ST_ZOS_USTR **ppstEncoding);

/* eax gsma shareddata point get the data*/
ZFUNC ZINT EaGsmaSharedData_PointGetData(ST_XML_ELEM *pstPoint, 
                ST_ZOS_USTR **ppstData);

/* rcs gsma shareddata marker get the id data */
ZFUNC ZINT EaGsmaSharedData_MarkerGetIdData(ST_XML_ELEM *pstMarker, 
                ST_ZOS_USTR **ppstId);

/* eax gsma shareddata get the remove */
ZFUNC ZINT EaGsmaSharedData_ActionGetRemove(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstRemove);

/* rcs gsma shareddata remove get the id data */
ZFUNC ZINT EaGsmaSharedData_RemoveGetIdData(ST_XML_ELEM *pstRemove, 
                ST_ZOS_USTR **ppstId);

/* eax gsma shareddata get the user */
ZFUNC ZINT EaGsmaSharedData_ActionGetUser(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstUser);

/* eax gsma shareddata user get the point */
ZFUNC ZINT EaGsmaSharedData_UserGetPoint(ST_XML_ELEM *pstUser, 
                ST_XML_ELEM **ppstPoint);

/* eax gsma shareddata get the background colour */
ZFUNC ZINT EaGsmaSharedData_ActionGetBackGroudColour(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstBkGrdColour);

/* eax gsma shareddata backgroud colour get the colour */
ZFUNC ZINT EaGsmaSharedData_BackGroudColourGetColour(ST_XML_ELEM *pstBkGrdColour, 
                ZUINT *piColour);

/* eax gsma shareddata get the image */
ZFUNC ZINT EaGsmaSharedData_ActionGetImage(ST_XML_ELEM *pstAction, 
                ST_XML_ELEM **ppstImage);

/* eax gsma shareddata image get the encoding */
ZFUNC ZINT EaGsmaSharedData_ImageGetEncoding(ST_XML_ELEM *pstImage, 
                ST_ZOS_USTR **ppstEncoding);

/* eax gsma shareddata image get the data */
ZFUNC ZINT EaGsmaSharedData_ImageGetData(ST_XML_ELEM *pstImage, 
                ST_ZOS_USTR **ppstData);

/* eax gsma shareddata set the actions */
ZFUNC ZINT EaGsmaSharedData_SetActions(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstEnv);

/* eax gsma shareddata version set the id */
ZFUNC ZINT EaGsmaSharedData_ActionsSetSeq(ST_XML_ELEM *pstActions, ZUINT iSeq);

/* eax gsma shareddata actions set the close */
ZFUNC ZINT EaGsmaSharedData_ActionsSetClose(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata actions set the version */
ZFUNC ZINT EaGsmaSharedData_ActionsSetVersion(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata version set the id */
ZFUNC ZINT EaGsmaSharedData_VersionSetId(ST_XML_ELEM *pstVersion, ZUINT iId);

/* eax gsma shareddata actions set the drawing */
ZFUNC ZINT EaGsmaSharedData_ActionsSetDrawing(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata drawing set the width */
ZFUNC ZINT EaGsmaSharedData_DrawingSetWidth(ST_XML_ELEM *pstDrawing, ZFLOAT fWidth);

/* eax gsma shareddata drawing set the colour */
ZFUNC ZINT EaGsmaSharedData_DrawingSetColour(ST_XML_ELEM *pstDrawing, ZUINT iColour);

/* eax gsma shareddata drawing set the erase */
ZFUNC ZINT EaGsmaSharedData_DrawingSetErase(ST_XML_ELEM *pstDrawing, ZBOOL bErase);

/* eax gsma shareddata drawing set the points */
ZFUNC ZINT EaGsmaSharedData_DrawingSetPoints(ST_XML_ELEM *pstDrawing, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata points set the encoding */
ZFUNC ZINT EaGsmaSharedData_PointsSetEncoding(ST_XML_ELEM *pstPoints, ST_ZOS_USTR *pstEncoding);

/* eax gsma shareddata points set data */
ZFUNC ZINT EaGsmaSharedData_PointsSetData(ST_XML_ELEM *pstPoints, 
                ST_ZOS_USTR *pstData);

/* eax gsma shareddata actions set the undo */
ZFUNC ZINT EaGsmaSharedData_ActionsSetUndo(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata actions set the bounds */
ZFUNC ZINT EaGsmaSharedData_ActionsSetBounds(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata bounds set the points */
ZFUNC ZINT EaGsmaSharedData_BoundsSetPoints(ST_XML_ELEM *pstDrawing, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata actions set the marker */
ZFUNC ZINT EaGsmaSharedData_ActionsSetMarker(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata marker set title */
ZFUNC ZINT EaGsmaSharedData_MarkerSetTitle(ST_XML_ELEM *pstMarker, 
                ST_ZOS_USTR *pstTitle);

/* eax gsma shareddata marker set snippet */
ZFUNC ZINT EaGsmaSharedData_MarkerSetSnippet(ST_XML_ELEM *pstMarker, 
                ST_ZOS_USTR *pstSnippet);

/* eax gsma shareddata marker set id */
ZFUNC ZINT EaGsmaSharedData_MarkerSetId(ST_XML_ELEM *pstMarker, 
                ST_ZOS_USTR *pstId);

/* eax gsma shareddata marker set the point */
ZFUNC ZINT EaGsmaSharedData_MarkerSetPoint(ST_XML_ELEM *pstMarker, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata point set the encoding */
ZFUNC ZINT EaGsmaSharedData_PointSetEncoding(ST_XML_ELEM *pstPoint, 
                ST_ZOS_USTR *pstEncoding);

/* eax gsma shareddata point set data */
ZFUNC ZINT EaGsmaSharedData_PointSetData(ST_XML_ELEM *pstPoint, 
                ST_ZOS_USTR *pstData);

/* eax gsma shareddata actions set the user */
ZFUNC ZINT EaGsmaSharedData_ActionsSetUser(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata user set the point */
ZFUNC ZINT EaGsmaSharedData_UserSetPoint(ST_XML_ELEM *pstUser, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata actions set the remove */
ZFUNC ZINT EaGsmaSharedData_ActionsSetRemove(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata remove set id */
ZFUNC ZINT EaGsmaSharedData_RemoveSetId(ST_XML_ELEM *pstRemove, 
                ST_ZOS_USTR *pstId);

/* eax gsma shareddata actions set the back groud colour */
ZFUNC ZINT EaGsmaSharedData_ActionsSetBackGroundColour(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata back groud colour set the colour */
ZFUNC ZINT EaGsmaSharedData_BackGroundColourSetColour(ST_XML_ELEM *pstBkGrdColour, ZUINT iColour);

/* eax gsma shareddata actions set the image */
ZFUNC ZINT EaGsmaSharedData_ActionsSetImage(ST_XML_ELEM *pstActions, 
                ST_XML_ELEM **ppstData);

/* eax gsma shareddata image set the encoding */
ZFUNC ZINT EaGsmaSharedData_ImageSetEncoding(ST_XML_ELEM *pstImage, 
                ST_ZOS_USTR *pstEncoding);

/* eax gsma shareddata image set data */
ZFUNC ZINT EaGsmaSharedData_ImageSetData(ST_XML_ELEM *pstImage, 
                ST_ZOS_USTR *pstData);

#ifdef __cplusplus
}
#endif

#endif /* _EA_GSMA_CALLDATA_H__ */


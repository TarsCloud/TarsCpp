/************************************************************************

        Copyright (c) 2005-2011 by Juphoon System Software, Inc.
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
  File name     : ea_imdn.h
  Module        : eax imdn
  Author        : zocol
  Version       : 0.1
  Created on    : 2011-03-01
  Description   :
    Marcos and structure definitions required by the eax imdn.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_IMDN_H__
#define _EA_IMDN_H__

#ifdef __cplusplus
extern "C"{
#endif

/* imdn get the imdn */
ZFUNC ZINT EaImdn_GetImdn(ST_XML_MSG *pstMsg, ST_XML_ELEM **ppstImdn);

/* imdn get the message-id */
ZFUNC ZINT EaImdn_ImdnGetMsgId(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR **ppstMsgId);

/* imdn get the datetime */
ZFUNC ZINT EaImdn_ImdnGetDateTime(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR **ppstDateTime);

/* imdn get the recipient-uri */
ZFUNC ZINT EaImdn_ImdnGetReciUri(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR **ppstReciUri);

/* imdn get the original-recipient-uri */
ZFUNC ZINT EaImdn_ImdnGetOrigReciUri(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR **ppstOrigReciUri);

/* imdn get the delivery-notification */
ZFUNC ZINT EaImdn_ImdnGetDeliNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstDeliNtf);

/* imdn get the display-notification */
ZFUNC ZINT EaImdn_ImdnGetDispNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstDispNtf);

/* imdn get the transmit-notification */
ZFUNC ZINT EaImdn_ImdnGetTrasmitNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstTransmitNtf);

/* imdn get the burn-notification */
ZFUNC ZINT EaImdn_ImdnGetBurnNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstBurnNtf);

/* imdn get the processing-notification */
ZFUNC ZINT EaImdn_ImdnGetProcNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstProcNtf);

/* imdn get the interworking-notification */
ZFUNC ZINT EaImdn_ImdnGetInterWorkNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstInterWorkNtf);

/* imdn delivery-notification get the status */
ZFUNC ZINT EaImdn_DeliNtfGetDeliStat(ST_XML_ELEM *pstDeliNtf,
                ST_XML_ELEM **ppstDeliStat);

/* imdn display-notification get the status */
ZFUNC ZINT EaImdn_DispNtfGetDispStat(ST_XML_ELEM *pstDispNtf,
                ST_XML_ELEM **ppstDispStat);

/* imdn burn-notification get the status */
ZFUNC ZINT EaImdn_BurnNtfGetBurnStat(ST_XML_ELEM *pstBurnNtf,
                ST_XML_ELEM **ppstBurnStat);

/* imdn processing-notification get the status */
ZFUNC ZINT EaImdn_ProcNtfGetProcStat(ST_XML_ELEM *pstProcNtf,
                ST_XML_ELEM **ppstProcStat);

/* imdn interworking-notification get the status */
ZFUNC ZINT EaImdn_InterWorkNtfGetIWStat(ST_XML_ELEM *pstDeliNtf,
                ST_XML_ELEM **ppstInterWorkStat);

/* imdn transmit-notification get the status */
ZFUNC ZINT EaImdn_TransmitNtfGetStat(ST_XML_ELEM *pstTransmitNtf,
                ST_XML_ELEM **ppstTransmitStat);

/* imdn delivery-notification status get the delivered */
ZFUNC ZINT EaImdn_DeliStatGetDelivered(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstDelivered);

/* imdn delivery-notification status get the forwarded */
ZFUNC ZINT EaImdn_DeliStatGetForwarded(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstForwarded);

/* imdn delivery-notification status get the failed */
ZFUNC ZINT EaImdn_DeliStatGetFailed(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstFailed);
                
/* imdn delivery-notification commonDispositionStatus get the forbidden */
ZFUNC ZINT EaImdn_DeliStatGetForbidden(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstForbidden);

/* imdn delivery-notification commonDispositionStatus get the error */
ZFUNC ZINT EaImdn_DeliStatGetError(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstError);

/* imdn delivery-notification commonDispositionStatus get the traffic forbidden */
ZFUNC ZINT EaImdn_DeliStatGetTrfcForbidden(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstTrfcForbidden);

/* imdn display-notification status get the displayed */
ZFUNC ZINT EaImdn_DispStatGetDisplayed(ST_XML_ELEM *pstDispStat,
                ST_XML_ELEM **ppstDisplayed);

/* imdn burn-notification status get the burned */
ZFUNC ZINT EaImdn_BurnStatGetBurned(ST_XML_ELEM *pstBurnStat,
                ST_XML_ELEM **ppstBurned);

/* imdn delivery-notification commonDispositionStatus get the forbidden */
ZFUNC ZINT EaImdn_DispStatGetForbidden(ST_XML_ELEM *pstDispStat,
                ST_XML_ELEM **ppstForbidden);
                
/* imdn delivery-notification commonDispositionStatus get the error */
ZFUNC ZINT EaImdn_DispStatGetError(ST_XML_ELEM *pstDispStat,
                ST_XML_ELEM **ppstError);

/* imdn processing-notification status get the processed */
ZFUNC ZINT EaImdn_ProcStatGetProcessed(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstProcessed);

/* imdn processing-notification status get the stored */
ZFUNC ZINT EaImdn_ProcStatGetStored(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstStored);

/* imdn commonDispositionStatus get the forbidden */
ZFUNC ZINT EaImdn_ProcStatGetForbidden(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstForbidden);

/* imdn commonDispositionStatus get the error */
ZFUNC ZINT EaImdn_ProcStatGetError(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstError);

/* imdn interworking-notification status get the legacy-sms */
ZFUNC ZINT EaImdn_InterWorkStatGetSms(ST_XML_ELEM *pstInterWorkStat,
                ST_XML_ELEM **ppstLegacySms);

/* imdn interworking-notification status get the legacy-mms */
ZFUNC ZINT EaImdn_InterWorkStatGetMms(ST_XML_ELEM *pstInterWorkStat,
                ST_XML_ELEM **ppstLegacyMms);

/* imdn transmit-notification status get the accepted */
ZFUNC ZINT EaImdn_TransmitStatGetAcpted(ST_XML_ELEM *pstTransmitStat,
                ST_XML_ELEM **ppstAcpted);

/* imdn set the imdn */
ZFUNC ZINT EaImdn_SetImdn(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs,
                ST_XML_ELEM **ppstImdn);

/* imdn set the message-id */
ZFUNC ZINT EaImdn_ImdnSetMsgId(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR *pstMsgId);

/* imdn set the datetime */
ZFUNC ZINT EaImdn_ImdnSetDateTime(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR *pstDateTime);

/* imdn set the recipient-uri */
ZFUNC ZINT EaImdn_ImdnSetReciUri(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR *pstReciUri);

/* imdn set the original-recipient-uri */
ZFUNC ZINT EaImdn_ImdnSetOrigReciUri(ST_XML_ELEM *pstImdn,
                ST_ZOS_USTR *pstOrigReciUri);

/* imdn set the delivery-notification */
ZFUNC ZINT EaImdn_ImdnSetDeliNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstDeliNtf);

/* imdn set the display-notification */
ZFUNC ZINT EaImdn_ImdnSetDispNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstDispNtf);

/* imdn set the transmit-notification */
ZFUNC ZINT EaImdn_ImdnSetTransmitNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstTransmitNtf);

/* imdn set the burn-notification */
ZFUNC ZINT EaImdn_ImdnSetBurnNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstBurnNtf);

/* imdn set the processing-notification */
ZFUNC ZINT EaImdn_ImdnSetProcNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstProcNtf);

/* imdn set the interworking-notification */
ZFUNC ZINT EaImdn_ImdnSetInterWorkNtf(ST_XML_ELEM *pstImdn,
                ST_XML_ELEM **ppstInterWorkNtf);

/* imdn delivery-notification set the status */
ZFUNC ZINT EaImdn_DeliNtfSetDeliStat(ST_XML_ELEM *pstDeliNtf,
                ST_XML_ELEM **ppstDeliStat);

/* imdn display-notification set the status */
ZFUNC ZINT EaImdn_DispNtfSetDispStat(ST_XML_ELEM *pstDispNtf,
                ST_XML_ELEM **ppstDispStat);

/* imdn burn-notification set the status */
ZFUNC ZINT EaImdn_BurnNtfSetBurnStat(ST_XML_ELEM *pstBurnNtf,
                ST_XML_ELEM **ppstBurnStat);

/* imdn processing-notification set the status */
ZFUNC ZINT EaImdn_ProcNtfSetProcStat(ST_XML_ELEM *pstProcNtf,
                ST_XML_ELEM **ppstProcStat);

/* imdn interworking-notification set the status */
ZFUNC ZINT EaImdn_InterWorkNtfSetIWStat(ST_XML_ELEM *pstInterWorkNtf,
                ST_XML_ELEM **ppstInterWorkStat);

/* imdn transmit-notification set the status */
ZFUNC ZINT EaImdn_TransmitNtfSetStat(ST_XML_ELEM *pstTransmitNtf,
                ST_XML_ELEM **ppstTransmitStat);

/* imdn delivery-notification status set the delivered */
ZFUNC ZINT EaImdn_DeliStatSetDelivered(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstDelivered);
                
/* imdn delivery-notification status set the failed */
ZFUNC ZINT EaImdn_DeliStatSetFailed(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstFailed);

/* imdn delivery-notification commonDispositionStatus set the forbidden */
ZFUNC ZINT EaImdn_DeliStatSetForbidden(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstForbidden);

/* imdn delivery-notification commonDispositionStatus set the error */
ZFUNC ZINT EaImdn_DeliStatSetError(ST_XML_ELEM *pstDeliStat,
                ST_XML_ELEM **ppstError);

/* imdn display-notification status set the displayed */
ZFUNC ZINT EaImdn_DispStatSetDisplayed(ST_XML_ELEM *pstDispStat,
                ST_XML_ELEM **ppstDisplayed);

/* imdn burn-notification status set the burned */
ZFUNC ZINT EaImdn_BurnStatSetBurned(ST_XML_ELEM *pstBurnStat,
                ST_XML_ELEM **ppstBurned);

/* imdn display-notification commonDispositionStatus set the forbidden */
ZFUNC ZINT EaImdn_DispStatSetForbidden(ST_XML_ELEM *pstDispStat,
                ST_XML_ELEM **ppstForbidden);

/* imdn display-notification commonDispositionStatus set the error */
ZFUNC ZINT EaImdn_DispStatSetError(ST_XML_ELEM *pstDispStat,
                ST_XML_ELEM **ppstError);

/* imdn processing-notification status set the processed */
ZFUNC ZINT EaImdn_ProcStatSetProcessed(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstProcessed);

/* imdn processing-notification status set the stored */
ZFUNC ZINT EaImdn_ProcStatSetStored(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstStored);

/* imdn commonDispositionStatus set the forbidden */
ZFUNC ZINT EaImdn_ProcStatSetForbidden(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstForbidden);

/* imdn commonDispositionStatus set the error */
ZFUNC ZINT EaImdn_ProcStatSetError(ST_XML_ELEM *pstProcStat,
                ST_XML_ELEM **ppstError);

/* imdn interworking-notification status set the legacy-sms */
ZFUNC ZINT EaImdn_InterWorkStatSetSms(ST_XML_ELEM *pstInterWorkStat,
                ST_XML_ELEM **ppstLegacySms);

/* imdn interworking-notification status set the legacy-mms */
ZFUNC ZINT EaImdn_InterWorkStatSetMms(ST_XML_ELEM *pstInterWorkStat,
                ST_XML_ELEM **ppstLegacyMms);

/* imdn transmit-notification status set the accepted*/
ZFUNC ZINT EaImdn_TransmitStatSetAcpted(ST_XML_ELEM *pstTransmitStat,
                ST_XML_ELEM **ppstAcpted);

#ifdef __cplusplus
}
#endif

#endif /* _EA_IMDN_H__ */


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
  File name     : zaka.c
  Module        : IMS authentication and key agreement
  Author        : mike.wang
  Created on    : 2013-01-30
  Description   :
    Function implement required by IMS AKAv1-MD5 in mrf authorization.
    Reference: 3GPP TS 35.206v11

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZAKA_H_
#define _ZAKA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ims aka algorithm f1 */ 
ZVOID Zaka_F1(ZUCHAR aucK[16], ZUCHAR aucRand[16], ZUCHAR aucSqn[6], 
              ZUCHAR aucAmf[2], ZUCHAR aucMacA[8]); 
              
/* ims aka algorithm f2345 */ 
ZVOID Zaka_F2345(ZUCHAR aucK[16], ZUCHAR aucRand[16], ZUCHAR aucRes[8], 
                 ZUCHAR aucCk[16], ZUCHAR aucIk[16], ZUCHAR aucAk[6]); 
                 
/* ims aka algorithm f1* */ 
ZVOID Zaka_F1star(ZUCHAR aucK[16], ZUCHAR aucRand[16], ZUCHAR aucSqn[6], 
                  ZUCHAR aucAmf[2], ZUCHAR aucMacS[8]); 

/* ims aka algorithm f5* */ 
ZVOID Zaka_F5star(ZUCHAR aucK[16], ZUCHAR aucRand[16], ZUCHAR aucAk[6]); 

/* ims aka function to compute OPc from aka op and K */ 
ZVOID Zaka_ComputeOpC(ZUCHAR aucOpC[16]); 

/* ims aka rijndael key schedule function. */ 
ZVOID Zaka_RjdlKeySchedule(ZUCHAR aucKey[16]); 

/* ims aka rijndael encryption function */ 
ZVOID Zaka_RjdlEncrypt(ZUCHAR aucInput[16], ZUCHAR aucOutput[16]); 

#ifdef __cplusplus
}
#endif

#endif /* _ZAKA_H_ */

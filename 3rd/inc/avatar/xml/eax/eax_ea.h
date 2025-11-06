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
  File name     : eax_xx.h
  Module        : eax
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EAX_EA_H__
#define _EAX_EA_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "ea/ea_comm_ply.h"             /* eax common-policy interfaces */
#include "ea/ea_conf_info.h"            /* eax conference-info interfaces */
#include "ea/ea_public_group.h"         /* eax public-group interfaces */
#include "ea/ea_public_group_list.h"    /* eax public-group-list interfaces */
#include "ea/ea_dlg_info.h"             /* eax dialog-info interfaces */
#include "ea/ea_im_isc.h"               /* eax im-iscomposing interfaces */
#include "ea/ea_im_his.h"               /* eax im history-list interfaces */
#include "ea/ea_patch_ops.h"            /* eax xml-patch-ops interfaces */
#include "ea/ea_pidf.h"                 /* eax pidf interfaces */
#include "ea/ea_pidf_caps.h"            /* eax pidf caps interfaces */
#include "ea/ea_pidf_cipid.h"           /* eax pidf cipid interfaces */
#include "ea/ea_pidf_diff.h"            /* eax pidf-diff interfaces */
#include "ea/ea_pidf_dm.h"              /* eax pidf dm interfaces */
#include "ea/ea_pidf_rpid.h"            /* eax pidf rpid interfaces */
#include "ea/ea_pidf_tstat.h"           /* eax pidf timed-status interfaces */
#include "ea/ea_pidf_ext.h"             /* eax pidf extension interfaces */
#include "ea/ea_pidf_ext1.h"            /* eax pidf extension 1.1 interfaces */
#include "ea/ea_pidf_gp.h"              /* eax pidf geopriv10 interfaces */
#include "ea/ea_poc_lsrv.h"             /* eax poc list-service interfaces */
#include "ea/ea_poc_rules.h"            /* eax poc-rules interfaces */
#include "ea/ea_poc_sets.h"             /* eax poc-settings interfaces */
#include "ea/ea_pres_rules.h"           /* eax pres-rules interfaces */
#include "ea/ea_prs_pres.h"             /* eax oma-pres interfaces */
#include "ea/ea_prs_rules.h"            /* eax prs pres-rules interfaces */
#include "ea/ea_res_lsts.h"             /* eax resource-lists interfaces */
#include "ea/ea_rlmi.h"                 /* eax rlmi interfaces */
#include "ea/ea_rls_srvs.h"             /* eax rsrv typedefs */
#include "ea/ea_wat_info.h"             /* eax watcherinfo interfaces */
#include "ea/ea_xcap_diff.h"            /* eax xcap-diff interfaces */
#include "ea/ea_xcap_dir.h"             /* eax xcap-directory interfaces */
#include "ea/ea_xcap_err.h"             /* eax xcap-error interfaces */
#include "ea/ea_xdm_ausg.h"             /* eax oma-appusage interfaces */
#include "ea/ea_xdm_cply.h"             /* eax xdm common-policy interfaces */
#include "ea/ea_xdm_uusg.h"             /* eax oma-uriusage interfaces */
#include "ea/ea_xdm_xdir.h"             /* eax xdm xcap-directory interfaces */
#include "ea/ea_xdm_xerr.h"             /* eax xdm xcap-error interfaces */
#include "ea/ea_xdm_ext.h"              /* eax xdm extensions interfaces */
#include "ea/ea_im_ss.h"                /* eax im service interfaces */
#include "ea/ea_loctn_tp.h"             /* eax location interfaces */
#include "ea/ea_reg_info.h"             /* eax reginfo interfaces */
#include "ea/ea_prs_ctt.h"              /* eax prs pres-content interfaces */
#include "ea/ea_imdn.h"                 /* eax imdn interfaces */
#include "ea/ea_cpy_ctrl.h"             /* eax copy control interfaces */
#include "ea/ea_rcs_geo.h"              /* eax rcs geo interfaces */
#include "ea/ea_rcs_fthttp.h"           /* eax rcs fthttp interfaces */
#include "ea/ea_rcs_rram.h"             /* eax rcs rram interfaces */
#include "ea/ea_gml.h"                  /* eax gml interfaces */
#include "ea/ea_gs.h"                   /* eax pidflo 1.0 interfaces */
#include "ea/ea_tl.h"                   /* eax netapi terminallocation 1 interfaces */
#include "ea/ea_msg_info.h"             /* eax msginfo interface */
#include "ea/ea_cab_pcc.h"              /* eax cab pcc interface */
#include "ea/ea_tdc_card.h"             /* eax tdc card interface */
#include "ea/ea_gsma_calldata.h"      /* eax gsma calldata interface */
#include "ea/ea_gsma_shareddata.h"   /* eax gsma shareddata interface */
#include "ea/ea_gsma_groupdata.h"       /* eax gsma groupdata interface */

#ifdef __cplusplus
}
#endif

#endif /* _EAX_EA_H__ */


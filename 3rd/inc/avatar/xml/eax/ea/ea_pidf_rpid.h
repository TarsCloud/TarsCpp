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
  File name     : ea_pidf_rpid.h
  Module        : eax pidf rpid
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf rpid.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_RPID_H__
#define _EA_PIDF_RPID_H__

#ifdef __cplusplus
extern "C"{
#endif

/*lint -save -e793 */
/************* urn:ietf:params:xml:ns:pidf:rpid **********/
typedef enum EN_EA_PIDF_RPID_TYPE
{
    EN_EA_PIDF_RPID_ACTIVE,       /* active */
    EN_EA_PIDF_RPID_IDLE,         /* idle */
    EN_EA_PIDF_RPID_ACTVS,        /* activities */
    EN_EA_PIDF_RPID_FROM,         /* from */
    EN_EA_PIDF_RPID_UNTIL,        /* until */
    EN_EA_PIDF_RPID_NOTE,         /* note */
    EN_EA_PIDF_RPID_UNKNOWN,      /* unknown */
    EN_EA_PIDF_RPID_APPOINTMENT,  /* appointment */
    EN_EA_PIDF_RPID_AWAY,         /* away */
    EN_EA_PIDF_RPID_BREAKFAST,    /* breakfast */
    EN_EA_PIDF_RPID_BUSY,         /* busy */
    EN_EA_PIDF_RPID_DINNER,       /* dinner */
    EN_EA_PIDF_RPID_HOLIDAY,      /* holiday */
    EN_EA_PIDF_RPID_IN_TRANSIT,   /* in-transit */
    EN_EA_PIDF_RPID_LOOKING_FOR_WORK, /* looking-for-work */
    EN_EA_PIDF_RPID_MEAL,         /* meal */
    EN_EA_PIDF_RPID_MEETING,      /* meeting */
    EN_EA_PIDF_RPID_ON_THE_PHONE, /* on-the-phone */
    EN_EA_PIDF_RPID_PERFM,        /* performance */
    EN_EA_PIDF_RPID_PERMANENT_ABSENCE, /* permanent-absence */
    EN_EA_PIDF_RPID_PLAYING,      /* playing */
    EN_EA_PIDF_RPID_PRESENT,      /* presentation */
    EN_EA_PIDF_RPID_SHOPPING,     /* shopping */
    EN_EA_PIDF_RPID_SLEEPING,     /* sleeping */
    EN_EA_PIDF_RPID_SPECTATOR,    /* spectator */
    EN_EA_PIDF_RPID_STEERING,     /* steering */
    EN_EA_PIDF_RPID_TRAVEL,       /* travel */
    EN_EA_PIDF_RPID_TV,           /* tv */
    EN_EA_PIDF_RPID_VACATION,     /* vacation */
    EN_EA_PIDF_RPID_WORKING,      /* working */
    EN_EA_PIDF_RPID_WORKSHIP,     /* worship */
    EN_EA_PIDF_RPID_OTHER,        /* other */
    EN_EA_PIDF_RPID_ID,           /* id */
    EN_EA_PIDF_RPID_CLASS,        /* class */
    EN_EA_PIDF_RPID_MOOD,         /* mood */
    EN_EA_PIDF_RPID_AFRAID,       /* afraid */
    EN_EA_PIDF_RPID_AMAZED,       /* amazed */
    EN_EA_PIDF_RPID_ANGRY,        /* angry */
    EN_EA_PIDF_RPID_ANNOYED,      /* annoyed */
    EN_EA_PIDF_RPID_ANXIOUS,      /* anxious */
    EN_EA_PIDF_RPID_ASHAMED,      /* ashamed */
    EN_EA_PIDF_RPID_BORED,        /* bored */
    EN_EA_PIDF_RPID_BRAVE,        /* brave */
    EN_EA_PIDF_RPID_CALM,         /* calm */
    EN_EA_PIDF_RPID_COLD,         /* cold */
    EN_EA_PIDF_RPID_CONFUSED,     /* confused */
    EN_EA_PIDF_RPID_CONTENTED,    /* contented */
    EN_EA_PIDF_RPID_CRANKY,       /* cranky */
    EN_EA_PIDF_RPID_CURIOUS,      /* curious */
    EN_EA_PIDF_RPID_DEPRESSED,    /* depressed */
    EN_EA_PIDF_RPID_DISAPPOINTED, /* disappointed */
    EN_EA_PIDF_RPID_DISGUSTED,    /* disgusted */
    EN_EA_PIDF_RPID_DISTRACTED,   /* distracted */
    EN_EA_PIDF_RPID_EMBARRASSED,  /* embarrassed */
    EN_EA_PIDF_RPID_EXCITED,      /* excited */
    EN_EA_PIDF_RPID_FLIRTATIOUS,  /* flirtatious */
    EN_EA_PIDF_RPID_FRUSTRATED,   /* frustrated */
    EN_EA_PIDF_RPID_GRUMPY,       /* grumpy */
    EN_EA_PIDF_RPID_GUILTY,       /* guilty */
    EN_EA_PIDF_RPID_HAPPY,        /* happy */
    EN_EA_PIDF_RPID_HOT,          /* hot */
    EN_EA_PIDF_RPID_HUMBLED,      /* humbled */
    EN_EA_PIDF_RPID_HUMILIATED,   /* humiliated */
    EN_EA_PIDF_RPID_HUNGRY,       /* hungry */
    EN_EA_PIDF_RPID_HURT,         /* hurt */
    EN_EA_PIDF_RPID_IMPRESSED,    /* impressed */
    EN_EA_PIDF_RPID_IN_AWE,       /* in_awe */
    EN_EA_PIDF_RPID_IN_LOVE,      /* in_love */
    EN_EA_PIDF_RPID_INDIGNANT,    /* indignant */
    EN_EA_PIDF_RPID_INTERESTED,   /* interested */
    EN_EA_PIDF_RPID_INVINCIBLE,   /* invincible */
    EN_EA_PIDF_RPID_JEALOUS,      /* jealous */
    EN_EA_PIDF_RPID_LONELY,       /* lonely */
    EN_EA_PIDF_RPID_MEAN,         /* mean */
    EN_EA_PIDF_RPID_MOODY,        /* moody */
    EN_EA_PIDF_RPID_NERVOUS,      /* nervous */
    EN_EA_PIDF_RPID_NEUTRAL,      /* neutral */
    EN_EA_PIDF_RPID_OFFENDED,     /* offended */
    EN_EA_PIDF_RPID_PLAYFUL,      /* playful */
    EN_EA_PIDF_RPID_PROUD,        /* proud */
    EN_EA_PIDF_RPID_RELIEVED,     /* relieved */
    EN_EA_PIDF_RPID_REMORSEFUL,   /* remorseful */
    EN_EA_PIDF_RPID_RESTLESS,     /* restless */
    EN_EA_PIDF_RPID_SAD,          /* sad */
    EN_EA_PIDF_RPID_SARCASTIC,    /* sarcastic */
    EN_EA_PIDF_RPID_SERIOUS,      /* serious */
    EN_EA_PIDF_RPID_SHOCKED,      /* shocked */
    EN_EA_PIDF_RPID_SHY,          /* shy */
    EN_EA_PIDF_RPID_SICK,         /* sick */
    EN_EA_PIDF_RPID_SLEEPY,       /* sleepy */
    EN_EA_PIDF_RPID_STRESSED,     /* stressed */
    EN_EA_PIDF_RPID_SURPRISED,    /* surprised */
    EN_EA_PIDF_RPID_THIRSTY,      /* thirsty */
    EN_EA_PIDF_RPID_WORRIED,      /* worried */
    EN_EA_PIDF_RPID_PLACE_IS,     /* place-is */
    EN_EA_PIDF_RPID_AUDIO,        /* audio */
    EN_EA_PIDF_RPID_NOISY,        /* noisy */
    EN_EA_PIDF_RPID_OK,           /* ok */
    EN_EA_PIDF_RPID_QUIET,        /* quiet */
    EN_EA_PIDF_RPID_VIDEO,        /* video */
    EN_EA_PIDF_RPID_TOOBRIGHT,    /* toobright */
    EN_EA_PIDF_RPID_DARK,         /* dark */
    EN_EA_PIDF_RPID_TEXT,         /* text */
    EN_EA_PIDF_RPID_UNCOMFORTABLE, /* uncomfortable */
    EN_EA_PIDF_RPID_INAPPROPRIATE, /* inappropriate */
    EN_EA_PIDF_RPID_PLACE_T,      /* place-type */
    EN_EA_PIDF_RPID_PRIVACY,      /* privacy */
    EN_EA_PIDF_RPID_RELAT,        /* relationship */
    EN_EA_PIDF_RPID_ASSISTANT,    /* assistant */
    EN_EA_PIDF_RPID_ASSOCIATE,    /* associate */
    EN_EA_PIDF_RPID_FAMILY,       /* family */
    EN_EA_PIDF_RPID_FRIEND,       /* friend */
    EN_EA_PIDF_RPID_SELF,         /* self */
    EN_EA_PIDF_RPID_SUPERVISOR,   /* supervisor */
    EN_EA_PIDF_RPID_SRV_CLASS,    /* service-class */
    EN_EA_PIDF_RPID_COURIER,      /* courier */
    EN_EA_PIDF_RPID_ELECTRONIC,   /* electronic */
    EN_EA_PIDF_RPID_FREIGHT,      /* freight */
    EN_EA_PIDF_RPID_IN_PERSON,    /* in-person */
    EN_EA_PIDF_RPID_POSTAL,       /* postal */
    EN_EA_PIDF_RPID_SPHERE,       /* sphere */
    EN_EA_PIDF_RPID_HOME,         /* home */
    EN_EA_PIDF_RPID_WORK,         /* work */
    EN_EA_PIDF_RPID_STAT_ICON,    /* status-icon */
    EN_EA_PIDF_RPID_TIME_OFFSET,  /* time-offset */
    EN_EA_PIDF_RPID_DESC,         /* description */
    EN_EA_PIDF_RPID_USR_INPUT,    /* user-input */
    EN_EA_PIDF_RPID_IDLE_THRESHOLD, /* idle-threshold */
    EN_EA_PIDF_RPID_LAST_INPUT    /* last-input */
} EN_EA_PIDF_RPID_TYPE;
/*lint -restore */

/* pidf rpid activities value type */
typedef enum EN_EA_PIDF_RPID_ACT_TYPE
{
    EN_EA_PIDF_RPID_ACT_APPOINT,     /* 0 */
    EN_EA_PIDF_RPID_ACT_AWAY,
    EN_EA_PIDF_RPID_ACT_BREAKFAST,
    EN_EA_PIDF_RPID_ACT_BUSY,
    EN_EA_PIDF_RPID_ACT_DINNER,
    EN_EA_PIDF_RPID_ACT_HOLIDAY,     /* 5 */
    EN_EA_PIDF_RPID_ACT_IN_TRANSIT,
    EN_EA_PIDF_RPID_ACT_LOOK_FOR_WORK,
    EN_EA_PIDF_RPID_ACT_MEAL,
    EN_EA_PIDF_RPID_ACT_MEETING,
    EN_EA_PIDF_RPID_ACT_ON_THE_PHONE, /* 10 */
    EN_EA_PIDF_RPID_ACT_PERFORM,
    EN_EA_PIDF_RPID_ACT_PERFORM_ABSENCE,
    EN_EA_PIDF_RPID_ACT_PRESENT,
    EN_EA_PIDF_RPID_ACT_SHOPPING,
    EN_EA_PIDF_RPID_ACT_SLEEPING,    /* 15 */
    EN_EA_PIDF_RPID_ACT_SPECTATOR,
    EN_EA_PIDF_RPID_ACT_STEERING,
    EN_EA_PIDF_RPID_ACT_TRAVEL,
    EN_EA_PIDF_RPID_ACT_TV,
    EN_EA_PIDF_RPID_ACT_VACATION,    /* 20 */
    EN_EA_PIDF_RPID_ACT_WORKING,
    EN_EA_PIDF_RPID_ACT_WORKSHIP,
    EN_EA_PIDF_RPID_ACT_UNKNOWN,
    EN_EA_PIDF_RPID_ACT_OTHER
} EN_EA_PIDF_RPID_ACT_TYPE;

/* pidf rpid mood value type */
typedef enum EN_EA_PIDF_RPID_MOOD_TYPE
{
    EN_EA_PIDF_RPID_MOOD_AFRAID,     /* 0 */
    EN_EA_PIDF_RPID_MOOD_AMAZED,
    EN_EA_PIDF_RPID_MOOD_ANGRY,
    EN_EA_PIDF_RPID_MOOD_ANNOYED,
    EN_EA_PIDF_RPID_MOOD_ASHAMED,
    EN_EA_PIDF_RPID_MOOD_BORED,      /* 5 */
    EN_EA_PIDF_RPID_MOOD_BRAVE,
    EN_EA_PIDF_RPID_MOOD_CALM,
    EN_EA_PIDF_RPID_MOOD_COLD,
    EN_EA_PIDF_RPID_MOOD_CONFUSED,
    EN_EA_PIDF_RPID_MOOD_CONTENTED,  /* 10 */
    EN_EA_PIDF_RPID_MOOD_CRANKY,
    EN_EA_PIDF_RPID_MOOD_CURIOUS,
    EN_EA_PIDF_RPID_MOOD_DEPRESSED,
    EN_EA_PIDF_RPID_MOOD_DISAPPOINTED,
    EN_EA_PIDF_RPID_MOOD_DISGUSTED,  /* 15 */
    EN_EA_PIDF_RPID_MOOD_DISTRACTED,
    EN_EA_PIDF_RPID_MOOD_EMBARRASSED,
    EN_EA_PIDF_RPID_MOOD_EXCITED,
    EN_EA_PIDF_RPID_MOOD_FLIRTATIOUS,
    EN_EA_PIDF_RPID_MOOD_FRUSTRATED, /* 20 */
    EN_EA_PIDF_RPID_MOOD_GRUMPY,
    EN_EA_PIDF_RPID_MOOD_GUILTY,
    EN_EA_PIDF_RPID_MOOD_HAPPY,
    EN_EA_PIDF_RPID_MOOD_HOT,
    EN_EA_PIDF_RPID_MOOD_HUMBLED,    /* 25 */
    EN_EA_PIDF_RPID_MOOD_HUMILIATED,
    EN_EA_PIDF_RPID_MOOD_HUNGRY,
    EN_EA_PIDF_RPID_MOOD_HURT,
    EN_EA_PIDF_RPID_MOOD_IMPRESSED,
    EN_EA_PIDF_RPID_MOOD_IN_AWE,     /* 30 */
    EN_EA_PIDF_RPID_MOOD_IN_LOVE,
    EN_EA_PIDF_RPID_MOOD_INDIGNANT,
    EN_EA_PIDF_RPID_MOOD_INTERESTED,
    EN_EA_PIDF_RPID_MOOD_INVINCIBLE,
    EN_EA_PIDF_RPID_MOOD_JEALOUS,    /* 35 */
    EN_EA_PIDF_RPID_MOOD_LONELY,
    EN_EA_PIDF_RPID_MOOD_MEAN,
    EN_EA_PIDF_RPID_MOOD_MOODY,
    EN_EA_PIDF_RPID_MOOD_NERVOUS,
    EN_EA_PIDF_RPID_MOOD_NEUTRAL,    /* 40 */
    EN_EA_PIDF_RPID_MOOD_OFFENDED,
    EN_EA_PIDF_RPID_MOOD_PLAYFUL,
    EN_EA_PIDF_RPID_MOOD_PROUD,
    EN_EA_PIDF_RPID_MOOD_RELIEVED,
    EN_EA_PIDF_RPID_MOOD_REMORSEFUL, /* 45 */
    EN_EA_PIDF_RPID_MOOD_RESTLESS,
    EN_EA_PIDF_RPID_MOOD_SAD,
    EN_EA_PIDF_RPID_MOOD_SARCASTIC,
    EN_EA_PIDF_RPID_MOOD_SERIOUS,
    EN_EA_PIDF_RPID_MOOD_SHOCKED,    /* 50 */
    EN_EA_PIDF_RPID_MOOD_SHY,
    EN_EA_PIDF_RPID_MOOD_SICK,
    EN_EA_PIDF_RPID_MOOD_SLEEPY,
    EN_EA_PIDF_RPID_MOOD_STRESSED,
    EN_EA_PIDF_RPID_MOOD_SURPRISED,  /* 55 */
    EN_EA_PIDF_RPID_MOOD_THIRSTY,
    EN_EA_PIDF_RPID_MOOD_WORRIED,
    EN_EA_PIDF_RPID_MOOD_UNKNOWN,
    EN_EA_PIDF_RPID_MOOD_OTHER
} EN_EA_PIDF_RPID_MOOD_TYPE;

/* pidf rpid audio type */
typedef enum EN_EA_PIDF_RPID_AUDIO_TYPE
{
    EN_EA_PIDF_RPID_AUDIO_NOISY,        
    EN_EA_PIDF_RPID_AUDIO_OK,
    EN_EA_PIDF_RPID_AUDIO_QUIET,
    EN_EA_PIDF_RPID_AUDIO_UNKNOWN
} EN_EA_PIDF_RPID_AUDIO_TYPE;

/* pidf rpid video type */
typedef enum EN_EA_PIDF_RPID_VIDEO_TYPE
{
    EN_EA_PIDF_RPID_VIDEO_TOO_BRIGHT,        
    EN_EA_PIDF_RPID_VIDEO_OK,
    EN_EA_PIDF_RPID_VIDEO_DARK,
    EN_EA_PIDF_RPID_VIDEO_UNKNOWN
} EN_EA_PIDF_RPID_VIDEO_TYPE;

/* pidf rpid text type */
typedef enum EN_EA_PIDF_RPID_TXT_TYPE
{
    EN_EA_PIDF_RPID_TXT_UNCOMFORT,        
    EN_EA_PIDF_RPID_TXT_INAPPROPRIATE,
    EN_EA_PIDF_RPID_TXT_OK,
    EN_EA_PIDF_RPID_TXT_UNKNOWN
} EN_EA_PIDF_RPID_TXT_TYPE;

/* pidf rpid privacy value type */
typedef enum EN_EA_PIDF_RPID_PRIVACY_TYPE
{
    EN_EA_PIDF_RPID_PRIVACY_AUDIO,        
    EN_EA_PIDF_RPID_PRIVACY_TXT,
    EN_EA_PIDF_RPID_PRIVACY_VIDEO,
    EN_EA_PIDF_RPID_PRIVACY_UNKNOWN
} EN_EA_PIDF_RPID_PRIVACY_TYPE;

/* pidf rpid relationship value type */
typedef enum EN_EA_PIDF_RPID_RELATE_TYPE
{
    EN_EA_PIDF_RPID_RELATE_ASSISTANT, /* 0 */
    EN_EA_PIDF_RPID_RELATE_ASSOCIATE,
    EN_EA_PIDF_RPID_RELATE_FAMILY,
    EN_EA_PIDF_RPID_RELATE_FRIEND,
    EN_EA_PIDF_RPID_RELATE_SELF,
    EN_EA_PIDF_RPID_RELATE_SUPERVISOR, /* 5 */
    EN_EA_PIDF_RPID_RELATE_UNKNOWN
} EN_EA_PIDF_RPID_RELATE_TYPE;

/* pidf rpid service-class value type */
typedef enum EN_EA_PIDF_RPID_SCLASS_TYPE
{
    EN_EA_PIDF_RPID_SCLASS_COURIER, /* 0 */
    EN_EA_PIDF_RPID_SCLASS_ELECTRONIC,
    EN_EA_PIDF_RPID_SCLASS_FREIGHT,
    EN_EA_PIDF_RPID_SCLASS_IN_PERSON,
    EN_EA_PIDF_RPID_SCLASS_POSTAL,
    EN_EA_PIDF_RPID_SCLASS_UNKNOWN  /* 5 */
} EN_EA_PIDF_RPID_SCLASS_TYPE;

/* pidf rpid sphere value type */
typedef enum EN_EA_PIDF_RPID_SPHERE_TYPE
{
    EN_EA_PIDF_RPID_SPHERE_HOME,
    EN_EA_PIDF_RPID_SPHERE_WORK,
    EN_EA_PIDF_RPID_SPHERE_UNKNNOWN
} EN_EA_PIDF_RPID_SPHERE_TYPE;

/* pidf rpid activeIdle value type */
typedef enum EN_EA_PIDF_RPID_ACTIDLE_TYPE
{
    EN_EA_PIDF_RPID_ACTIDLE_ACTIVE,
    EN_EA_PIDF_RPID_ACTIDLE_IDLE
} EN_EA_PIDF_RPID_ACTIDLE_TYPE;

/* pidf rpid get the activities */
ZFUNC ZINT EaPidf_RpidGetActs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstActs);

/* pidf rpid get the first activities note */
ZFUNC ZINT EaPidf_RpidActsGetFirstNote(ST_XML_ELEM *pstActs, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next activities note */
ZFUNC ZINT EaPidf_RpidActsGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the first activities value */
ZFUNC ZINT EaPidf_RpidActsGetFirstVal(ST_XML_ELEM *pstActs, 
                ST_XML_ELEM **ppstVal);

/* pidf rpid get the next activities value */
ZFUNC ZINT EaPidf_RpidActsGetNextVal(ST_XML_ELEM *pstVal, 
                ST_XML_ELEM **ppstVal);

/* pidf rpid get the activities from */
ZFUNC ZINT EaPidf_RpidActsGetFrom(ST_XML_ELEM *pstActs, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the activities until */
ZFUNC ZINT EaPidf_RpidActsGetUntil(ST_XML_ELEM *pstActs, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the activities id */
ZFUNC ZINT EaPidf_RpidActsGetId(ST_XML_ELEM *pstActs, ST_ZOS_USTR **ppstId);

/* pidf rpid get the activitie value type EN_EA_PIDF_RPID_ACT_TYPE */
ZFUNC ZINT EaPidf_RpidActValGetType(ST_XML_ELEM *pstVal, ZUCHAR *pucType);

/* pidf rpid get the class */
ZFUNC ZINT EaPidf_RpidGetClass(ST_XML_ELEM *pstElem, ST_ZOS_USTR **ppstClass);

/* pidf rpid get the mood */
ZFUNC ZINT EaPidf_RpidGetMood(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstMood);

/* pidf rpid get the first mood note */
ZFUNC ZINT EaPidf_RpidMoodGetFirstNote(ST_XML_ELEM *pstMood, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next mood note */
ZFUNC ZINT EaPidf_RpidMoodGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the first mood value */
ZFUNC ZINT EaPidf_RpidMoodGetFirstVal(ST_XML_ELEM *pstMood, 
                ST_XML_ELEM **ppstVal);

/* pidf rpid get the next mood value */
ZFUNC ZINT EaPidf_RpidMoodGetNextVal(ST_XML_ELEM *pstVal, 
                ST_XML_ELEM **ppstVal);

/* pidf rpid get the mood from */
ZFUNC ZINT EaPidf_RpidMoodGetFrom(ST_XML_ELEM *pstMood, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the mood until */
ZFUNC ZINT EaPidf_RpidMoodGetUntil(ST_XML_ELEM *pstMood, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the mood id */
ZFUNC ZINT EaPidf_RpidMoodGetId(ST_XML_ELEM *pstMood, ST_ZOS_USTR **ppstId);

/* pidf rpid get the mood value type EN_EA_PIDF_RPID_MOOD_TYPE */
ZFUNC ZINT EaPidf_RpidMoodValGetType(ST_XML_ELEM *pstVal, 
                ZUCHAR *pucType);

/* pidf rpid get the place-is */
ZFUNC ZINT EaPidf_RpidGetPlaceIs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPlaceIs);

/* pidf rpid get the first mood note */
ZFUNC ZINT EaPidf_RpidPlaceIsGetFirstNote(ST_XML_ELEM *pstPlaceIs, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next mood note */
ZFUNC ZINT EaPidf_RpidPlaceIsGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the place-is audio type EN_EA_PIDF_RPID_AUDIO_TYPE */
ZFUNC ZINT EaPidf_RpidPlaceIsGetAudio(ST_XML_ELEM *pstPlaceIs, 
                ZUCHAR *pucType);

/* pidf rpid get the place-is video type EN_EA_PIDF_RPID_VIDEO_TYPE */
ZFUNC ZINT EaPidf_RpidPlaceIsGetVideo(ST_XML_ELEM *pstPlaceIs, 
                ZUCHAR *pucType);

/* pidf rpid get the place-is text type EN_EA_PIDF_RPID_TXT_TYPE */
ZFUNC ZINT EaPidf_RpidPlaceIsGetTxt(ST_XML_ELEM *pstPlaceIs, 
                ZUCHAR *pucType);

/* pidf rpid get the place-is from */
ZFUNC ZINT EaPidf_RpidPlaceIsGetFrom(ST_XML_ELEM *pstPlaceIs, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the place-is until */
ZFUNC ZINT EaPidf_RpidPlaceIsGetUntil(ST_XML_ELEM *pstPlaceIs, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the place-is id */
ZFUNC ZINT EaPidf_RpidPlaceIsGetId(ST_XML_ELEM *pstPlaceIs, 
                ST_ZOS_USTR **ppstId);

/* pidf rpid get the place-type */
ZFUNC ZINT EaPidf_RpidGetPlaceType(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPlaceType);

/* pidf rpid get the first place-type note */
ZFUNC ZINT EaPidf_RpidPlaceTypeGetFirstNote(ST_XML_ELEM *pstPlaceType, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next place-type note */
ZFUNC ZINT EaPidf_RpidPlaceTypeGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the place-type from */
ZFUNC ZINT EaPidf_RpidPlaceTypeGetFrom(ST_XML_ELEM *pstPlaceType, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the place-type until */
ZFUNC ZINT EaPidf_RpidPlaceTypeGetUntil(ST_XML_ELEM *pstPlaceType, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the place-type id */
ZFUNC ZINT EaPidf_RpidPlaceTypeGetId(ST_XML_ELEM *pstPlaceType, 
                ST_ZOS_USTR **ppstId);

/* pidf rpid get the privacy */
ZFUNC ZINT EaPidf_RpidGetPrivacy(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPrivacy);

/* pidf rpid get the first privacy note */
ZFUNC ZINT EaPidf_RpidPrivacyGetFirstNote(ST_XML_ELEM *pstPrivacy, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next privacy note */
ZFUNC ZINT EaPidf_RpidPrivacyGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the privacy value EN_EA_PIDF_RPID_PRIVACY_TYPE */
ZFUNC ZINT EaPidf_RpidPrivacyGetVal(ST_XML_ELEM *pstPrivacy, 
                ZUCHAR ucType, ZBOOL *pbExist);

/* pidf rpid get the privacy from */
ZFUNC ZINT EaPidf_RpidPrivacyGetFrom(ST_XML_ELEM *pstPrivacy, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the privacy until */
ZFUNC ZINT EaPidf_RpidPrivacyGetUntil(ST_XML_ELEM *pstPrivacy, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the privacy id */
ZFUNC ZINT EaPidf_RpidPrivacyGetId(ST_XML_ELEM *pstPrivacy, 
                ST_ZOS_USTR **ppstId);

/* pidf rpid get the relationship */
ZFUNC ZINT EaPidf_RpidGetRelate(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRelate);

/* pidf rpid get the first relationship note */
ZFUNC ZINT EaPidf_RpidRelateGetFirstNote(ST_XML_ELEM *pstRelate, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next relationship note */
ZFUNC ZINT EaPidf_RpidRelateGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the relationship value EN_EA_PIDF_RPID_RELATE_TYPE */
ZFUNC ZINT EaPidf_RpidRelateGetVal(ST_XML_ELEM *pstRelate, 
                ZUCHAR ucType, ZBOOL *pbExist);

/* pidf rpid get the service-class */
ZFUNC ZINT EaPidf_RpidGetSClass(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSClass);

/* pidf rpid get the first service-class note */
ZFUNC ZINT EaPidf_RpidSClassGetFirstNote(ST_XML_ELEM *pstSClass, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the next service-class note */
ZFUNC ZINT EaPidf_RpidSClassGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid get the service-class value EN_EA_PIDF_RPID_SCLASS_TYPE */
ZFUNC ZINT EaPidf_RpidSClassGetVal(ST_XML_ELEM *pstSClass, 
                ZUCHAR ucType, ZBOOL *pbExist);

/* pidf rpid get the sphere */
ZFUNC ZINT EaPidf_RpidGetSphere(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSphere);

/* pidf rpid get the sphere value EN_EA_PIDF_RPID_SPHERE_TYPE */
ZFUNC ZINT EaPidf_RpidSphereGetVal(ST_XML_ELEM *pstSphere, 
                ZUCHAR ucType, ZBOOL *pbExist);

/* pidf rpid get the sphere from */
ZFUNC ZINT EaPidf_RpidSphereGetFrom(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the sphere until */
ZFUNC ZINT EaPidf_RpidSphereGetUntil(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the sphere id */
ZFUNC ZINT EaPidf_RpidSphereGetId(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR **ppstId);

/* pidf rpid get the status-icon */
ZFUNC ZINT EaPidf_RpidGetIcon(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstIcon);

/* pidf rpid get the status-icon from */
ZFUNC ZINT EaPidf_RpidIconGetFrom(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the status-icon until */
ZFUNC ZINT EaPidf_RpidIconGetUntil(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the status-icon id */
ZFUNC ZINT EaPidf_RpidIconGetId(ST_XML_ELEM *pstIcon, ST_ZOS_USTR **ppstId);

/* pidf rpid get the time-offset */
ZFUNC ZINT EaPidf_RpidGetTimeOffset(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstTimeOffset);

/* pidf rpid get the time-offset from */
ZFUNC ZINT EaPidf_RpidTimeOffsetGetFrom(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR **ppstFrom);

/* pidf rpid get the time-offset until */
ZFUNC ZINT EaPidf_RpidTimeOffsetGetUntil(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR **ppstUntil);

/* pidf rpid get the time-offset description */
ZFUNC ZINT EaPidf_RpidTimeOffsetGetDesc(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR **ppstDesc);

/* pidf rpid get the time-offset id */
ZFUNC ZINT EaPidf_RpidTimeOffsetGetId(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR **ppstId);

/* pidf rpid get the user-input */
ZFUNC ZINT EaPidf_RpidGetUsrInput(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstUsrInput);

/* pidf rpid get the user-input idle-threshold */
ZFUNC ZINT EaPidf_RpidUsrInputGetIdle(ST_XML_ELEM *pstUsrInput, 
                ZUINT *piIdle);

/* pidf rpid get the user-input last-input */
ZFUNC ZINT EaPidf_RpidUsrInputGetLastInput(ST_XML_ELEM *pstUsrInput, 
                ST_ZOS_USTR **ppstLastInput);

/* pidf rpid get the user-input id */
ZFUNC ZINT EaPidf_RpidUsrInputGetId(ST_XML_ELEM *pstUsrInput, 
                ST_ZOS_USTR **ppstId);

/* pidf rpid get the user-input activeIdle EN_EA_PIDF_RPID_ACTIDLE_TYPE */
ZFUNC ZINT EaPidf_RpidUsrInputGetActIdle(ST_XML_ELEM *pstUsrInput, 
                ZUCHAR *pucType);

/* pidf rpid set the activities */
ZFUNC ZINT EaPidf_RpidSetActs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstActs);

/* pidf rpid set the activities note */
ZFUNC ZINT EaPidf_RpidActsSetNote(ST_XML_ELEM *pstActs, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the activities value EN_EA_PIDF_RPID_ACT_TYPE */
ZFUNC ZINT EaPidf_RpidActsSetVal(ST_XML_ELEM *pstActs, 
                ZUCHAR ucType);

/* pidf rpid set the activities from */
ZFUNC ZINT EaPidf_RpidActsSetFrom(ST_XML_ELEM *pstActs, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the activities until */
ZFUNC ZINT EaPidf_RpidActsSetUntil(ST_XML_ELEM *pstActs, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the activities id */
ZFUNC ZINT EaPidf_RpidActsSetId(ST_XML_ELEM *pstActs, ST_ZOS_USTR *pstId);

/* pidf rpid set the class */
ZFUNC ZINT EaPidf_RpidSetClass(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstClass);

/* pidf rpid set the mood */
ZFUNC ZINT EaPidf_RpidSetMood(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstMood);

/* pidf rpid set the mood note */
ZFUNC ZINT EaPidf_RpidMoodSetNote(ST_XML_ELEM *pstMood, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the mood value EN_EA_PIDF_RPID_MOOD_TYPE */
ZFUNC ZINT EaPidf_RpidMoodSetVal(ST_XML_ELEM *pstMood, 
                ZUCHAR ucType);

/* pidf rpid set the mood from */
ZFUNC ZINT EaPidf_RpidMoodSetFrom(ST_XML_ELEM *pstMood, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the mood until */
ZFUNC ZINT EaPidf_RpidMoodSetUntil(ST_XML_ELEM *pstMood, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the mood id */
ZFUNC ZINT EaPidf_RpidMoodSetId(ST_XML_ELEM *pstMood, ST_ZOS_USTR *pstId);

/* pidf rpid set the mood value type EN_EA_PIDF_RPID_MOOD_TYPE */
ZFUNC ZINT EaPidf_RpidMoodValSetType(ST_XML_ELEM *pstVal, 
                ZUCHAR ucType);

/* pidf rpid set the place-is */
ZFUNC ZINT EaPidf_RpidSetPlaceIs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPlaceIs);

/* pidf rpid set the place-is note */
ZFUNC ZINT EaPidf_RpidPlaceIsSetNote(ST_XML_ELEM *pstPlaceIs, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the place-is audio type EN_EA_PIDF_RPID_AUDIO_TYPE */
ZFUNC ZINT EaPidf_RpidPlaceIsSetAudio(ST_XML_ELEM *pstPlaceIs, 
                ZUCHAR ucType);

/* pidf rpid set the place-is video type EN_EA_PIDF_RPID_VIDEO_TYPE */
ZFUNC ZINT EaPidf_RpidPlaceIsSetVideo(ST_XML_ELEM *pstPlaceIs, 
                ZUCHAR ucType);

/* pidf rpid set the place-is text type EN_EA_PIDF_RPID_TXT_TYPE */
ZFUNC ZINT EaPidf_RpidPlaceIsSetTxt(ST_XML_ELEM *pstPlaceIs, 
                ZUCHAR ucType);

/* pidf rpid set the place-is from */
ZFUNC ZINT EaPidf_RpidPlaceIsSetFrom(ST_XML_ELEM *pstPlaceIs, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the place-is until */
ZFUNC ZINT EaPidf_RpidPlaceIsSetUntil(ST_XML_ELEM *pstPlaceIs, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the place-is id */
ZFUNC ZINT EaPidf_RpidPlaceIsSetId(ST_XML_ELEM *pstPlaceIs, 
                ST_ZOS_USTR *pstId);

/* pidf rpid set the place-type */
ZFUNC ZINT EaPidf_RpidSetPlaceType(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPlaceType);

/* pidf rpid set the place-type note */
ZFUNC ZINT EaPidf_RpidPlaceTypeSetNote(ST_XML_ELEM *pstPlaceType, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the place-type from */
ZFUNC ZINT EaPidf_RpidPlaceTypeSetFrom(ST_XML_ELEM *pstPlaceType, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the place-type until */
ZFUNC ZINT EaPidf_RpidPlaceTypeSetUntil(ST_XML_ELEM *pstPlaceType, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the place-type id */
ZFUNC ZINT EaPidf_RpidPlaceTypeSetId(ST_XML_ELEM *pstPlaceType, 
                ST_ZOS_USTR *pstId);

/* pidf rpid set the privacy */
ZFUNC ZINT EaPidf_RpidSetPrivacy(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPrivacy);

/* pidf rpid set the privacy note */
ZFUNC ZINT EaPidf_RpidPrivacySetNote(ST_XML_ELEM *pstPrivacy, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the privacy value EN_EA_PIDF_RPID_PRIVACY_TYPE */
ZFUNC ZINT EaPidf_RpidPrivacySetVal(ST_XML_ELEM *pstPrivacy, 
                ZUCHAR ucType);

/* pidf rpid set the privacy from */
ZFUNC ZINT EaPidf_RpidPrivacySetFrom(ST_XML_ELEM *pstPrivacy, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the privacy until */
ZFUNC ZINT EaPidf_RpidPrivacySetUntil(ST_XML_ELEM *pstPrivacy, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the privacy id */
ZFUNC ZINT EaPidf_RpidPrivacySetId(ST_XML_ELEM *pstPrivacy, 
                ST_ZOS_USTR *pstId);

/* pidf rpid set the relationship */
ZFUNC ZINT EaPidf_RpidSetRelate(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRelate);

/* pidf rpid set the relationship note */
ZFUNC ZINT EaPidf_RpidRelateSetNote(ST_XML_ELEM *pstRelate, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the relationship value EN_EA_PIDF_RPID_RELATE_TYPE */
ZFUNC ZINT EaPidf_RpidRelateSetVal(ST_XML_ELEM *pstRelate, 
                ZUCHAR ucType);

/* pidf rpid set the service-class */
ZFUNC ZINT EaPidf_RpidSetSClass(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSClass);

/* pidf rpid set the service-class note */
ZFUNC ZINT EaPidf_RpidSClassSetNote(ST_XML_ELEM *pstSClass, 
                ST_XML_ELEM **ppstNote);

/* pidf rpid set the service-class value EN_EA_PIDF_RPID_SCLASS_TYPE */
ZFUNC ZINT EaPidf_RpidSClassSetVal(ST_XML_ELEM *pstSClass, 
                ZUCHAR ucType);

/* pidf rpid set the sphere */
ZFUNC ZINT EaPidf_RpidSetSphere(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSphere);

/* pidf rpid set the sphere value EN_EA_PIDF_RPID_SPHERE_TYPE */
ZFUNC ZINT EaPidf_RpidSphereSetVal(ST_XML_ELEM *pstSphere, 
                ZUCHAR ucType);

/* pidf rpid set the sphere from */
ZFUNC ZINT EaPidf_RpidSphereSetFrom(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the sphere until */
ZFUNC ZINT EaPidf_RpidSphereSetUntil(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the sphere id */
ZFUNC ZINT EaPidf_RpidSphereSetId(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR *pstId);

/* pidf rpid set the status-icon */
ZFUNC ZINT EaPidf_RpidSetIcon(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstIcon);

/* pidf rpid set the status-icon from */
ZFUNC ZINT EaPidf_RpidIconSetFrom(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the status-icon until */
ZFUNC ZINT EaPidf_RpidIconSetUntil(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the status-icon id */
ZFUNC ZINT EaPidf_RpidIconSetId(ST_XML_ELEM *pstIcon, ST_ZOS_USTR *pstId);

/* pidf rpid set the time-offset */
ZFUNC ZINT EaPidf_RpidSetTimeOffset(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstTimeOffset);

/* pidf rpid set the time-offset from */
ZFUNC ZINT EaPidf_RpidTimeOffsetSetFrom(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR *pstFrom);

/* pidf rpid set the time-offset until */
ZFUNC ZINT EaPidf_RpidTimeOffsetSetUntil(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR *pstUntil);

/* pidf rpid set the time-offset description */
ZFUNC ZINT EaPidf_RpidTimeOffsetSetDesc(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR *pstDesc);

/* pidf rpid set the time-offset id */
ZFUNC ZINT EaPidf_RpidTimeOffsetSetId(ST_XML_ELEM *pstTimeOffset, 
                ST_ZOS_USTR *pstId);

/* pidf rpid set the user-input */
ZFUNC ZINT EaPidf_RpidSetUsrInput(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstUsrInput);

/* pidf rpid set the user-input idle-threshold */
ZFUNC ZINT EaPidf_RpidUsrInputSetIdle(ST_XML_ELEM *pstUsrInput, 
                ZUINT iIdle);

/* pidf rpid set the user-input last-input */
ZFUNC ZINT EaPidf_RpidUsrInputSetLastInput(ST_XML_ELEM *pstUsrInput, 
                ST_ZOS_USTR *pstLastInput);

/* pidf rpid set the user-input id */
ZFUNC ZINT EaPidf_RpidUsrInputSetId(ST_XML_ELEM *pstUsrInput, 
                ST_ZOS_USTR *pstId);

/* pidf rpid set the user-input activeIdle EN_EA_PIDF_RPID_ACTIDLE_TYPE */
ZFUNC ZINT EaPidf_RpidUsrInputSetActIdle(ST_XML_ELEM *pstUsrInput, 
                ZUCHAR ucType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_RPID_H__ */


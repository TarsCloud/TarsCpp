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
  File name     : zsh_cmd.h
  Module        : zos shell command
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-09-30    
  Description   :
   Function implement required by zos shell command

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZSH_CMD_H__
#define _ZSH_CMD_H__

/** 
 * @file zsh_cmd.h
 * @brief Zos shell command functions.
 *  
 * In this file, it defined the operation function
 * of zos shell command, including add, remove, run
 * and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief Type defination of zos shell command function.
 *
 * @see ST_ZSH_CMD_HELP
 */
typedef ZINT (*PFN_ZSHCMD)(ZINT iArgc, ZCHAR *apcArgv[]);

/**
 * @brief Zos shell command help struct.
 *
 * @see Zsh_CmdAdd
 */
typedef struct tagZSH_CMD_HELP
{
    ZCHAR *pcName;                   /**< command name */
    ZCHAR *pcDesc;                   /**< command description */
    ZCHAR *pcSynop;                  /**< command synopsis */
    PFN_ZSHCMD pfnCmd;               /**< command function */
} ST_ZSH_CMD_HELP;

/** @{ */
/* Zsh Inner-Commands */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdHelp; /**< @brief show command help information */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdExit; /**< @brief exit */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdEnv; /**< @brief show all enviroment variables */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdEcho; /**< @brief show one enviroment variable */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdExport; /**< @brief set enviroment variable */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdUnset; /**< @brief unset enviroment variable */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdDf; /**< @brief show zos dbuf statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdDm; /**< @brief show zos dbuf dump statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdDu; /**< @brief show zos dump statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdLl; /**< @brief set zos log level */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdLo; /**< @brief show zos log buffer */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdLp; /**< @brief set zos log print */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdMm; /**< @brief show zos mem statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdPmm; /**< @brief show zos power mem statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdMt; /**< @brief show zos mem trace */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdMs; /**< @brief show zos message statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdMp; /**< @brief show zos print statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdTk; /**< @brief show zos task statistics */
extern ZCONST ST_ZSH_CMD_HELP g_stZshCmdTkid; /**< @brief show zos task id statistics */
/** @} */

/**
 * @brief Add a command to command list.
 *
 * @param [in] pstHelp Command information.
 *
 * @retval ZOK      Add command successfully.
 * @retval ZFAILED  Add command failed.
 *
 * Call this function to add a command to command list,
 * then you can run command from zsh prompt line.
 *
 * @see Zsh_CmdAddX
 */
ZFUNC ZINT Zsh_CmdAdd(ZCONST ST_ZSH_CMD_HELP *pstHelp);

/**
 * @brief Add a command to command list with different parameter format.
 *
 * @param [in] pcName Command name.
 * @param [in] pcDesc Command description,
 * @param [in] pcSynop Command synopsis.
 * @param [in] pfnCmd Command function.
 *
 * @retval ZOK      Add command successfully.
 * @retval ZFAILED  Add command failed.
 *
 * Call this function to add a command to command list,
 * then you can run command from zsh prompt line.
 *
 * @see Zsh_CmdAdd
 */
ZFUNC ZINT Zsh_CmdAddX(ZCHAR *pcName, ZCHAR *pcDesc, ZCHAR *pcSynop,
                PFN_ZSHCMD pfnCmd);

/**
 * @brief Remove a command to command list.
 *
 * @param [in] pcName Command name.
 *
 * @retval ZOK      Remove command successfully.
 * @retval ZFAILED  Remove command failed.
 *
 * Call this function to remove a command to command list.
 *
 * @see Zsh_CmdRmv
 */
ZFUNC ZINT Zsh_CmdRmv(ZCHAR *pcName);

/**
 * @brief Remove all command to command list.
 *
 * @retval ZOK      Remove command successfully.
 * @retval ZFAILED  Remove command failed.
 *
 * @note Call this function to remove all command to command list.
 *
 * @see Zsh_CmdRmv
 */
ZFUNC ZINT Zsh_CmdRmvAll(ZFUNC_VOID);

/**
 * @brief Get the count of command in command list.
 *
 * @return Count of command.
 */
ZFUNC ZINT Zsh_CmdGetCount(ZFUNC_VOID);

/**
 * @brief Get command information by index.
 *
 * @param [in] iIndex Command index, which is the order it added.
 *
 * @return If index is valid, return command information, or return ZNULL.
 *
 * Get command information by index.
 *
 * @see Zsh_CmdFindHelp
 */
ZFUNC ST_ZSH_CMD_HELP * Zsh_CmdGetHelp(ZINT iIndex);

/**
 * @brief Search command information by name.
 *
 * @param [in] pcName The command name search for.
 *
 * @return If found, return command information, or return ZNULL.
 *
 * Get command information by index.
 *
 * @see Zsh_CmdGetHelp
 */
ZFUNC ST_ZSH_CMD_HELP * Zsh_CmdFindHelp(ZCHAR *pcName);

/**
 * @brief Run command with specific name.
 *
 * @param [in] pcCmd Command name.
 *
 * @retval ZSH_RET_QUIT Indicate the system should be quit.
 * @retval ZOK Otherwise.
 *
 * Run command with specific name.
 *
 * @see Zsh_CmdRunX
 */
ZFUNC ZINT Zsh_CmdRun(ZCHAR *pcCmd);

/**
 * @brief Run command with specific name.
 *
 * @param [in] pstCmd Command name.
 *
 * @retval ZSH_RET_QUIT Indicate the system should be quit.
 * @retval ZOK Otherwise.
 *
 * Run command with specific name.
 *
 * @see Zsh_CmdRun
 */
ZFUNC ZINT Zsh_CmdRunX(ST_ZOS_SSTR *pstCmd);

/**
 * @brief Run command with specific name.
 *
 * @param [in] iArgc Argument count.
 * @param [in] apcArgv Argument string table.
 * @param [in] pcOptStr Option string.
 * @param [out] ppcArg Current argument string.
 *
 * @retval -1 Indicate the end of argument table.
 * @retval '?' Indicate unknown argument.
 * @retval Otherwise The option character
 *
 * The function is used to prase the argument string table.
 * First, you should reset g_iZshOptInd to 0.
 * Then repeat call this function until it returns -1, 
 * which indicate the end of argument string table.
 *
 * If prase successfully, it return the corresponding character in [pcOptStr].
 * If the character in option string is followed by :, then the argument string
 * will assgined to [ppcArg]. \n
 *
 * @section example Example:
 * @code
 * g_iZshOptInd = 0;
 * while ((opt = Zsh_CmdGetOpt(argc, argv, "nt:", &argstr)) != -1)
 * {
 *     switch (opt)
 *     {
 *     case 'n':
 *         flag = 1;
 *         break;
 *     case 't':
 *         val = atoi(argstr);
 *         break;
 *     default:
 *         fprintf(stderr, "Usage: %s [-t nsec] [-n] \r\n", argv[0]);
 *         exit(0);
 *     }
 * }
 * @endcode
 *
 * @note Learn more detail from getopt manpage in linux
 */
ZFUNC ZINT Zsh_CmdGetOpt(ZINT iArgc, ZCHAR *apcArgv[], ZCHAR *pcOptStr, 
                ZCHAR **ppcArg);

#ifdef __cplusplus
}
#endif

#endif /* _ZSH_CMD_H__ */



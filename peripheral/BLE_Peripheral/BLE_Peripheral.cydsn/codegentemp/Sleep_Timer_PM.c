/*******************************************************************************
* File Name: Sleep_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Sleep_Timer.h"

static Sleep_Timer_backupStruct Sleep_Timer_backup;


/*******************************************************************************
* Function Name: Sleep_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sleep_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Sleep_Timer_SaveConfig(void) 
{
    #if (!Sleep_Timer_UsingFixedFunction)
        Sleep_Timer_backup.TimerUdb = Sleep_Timer_ReadCounter();
        Sleep_Timer_backup.InterruptMaskValue = Sleep_Timer_STATUS_MASK;
        #if (Sleep_Timer_UsingHWCaptureCounter)
            Sleep_Timer_backup.TimerCaptureCounter = Sleep_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Sleep_Timer_UDB_CONTROL_REG_REMOVED)
            Sleep_Timer_backup.TimerControlRegister = Sleep_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Sleep_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sleep_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Sleep_Timer_RestoreConfig(void) 
{   
    #if (!Sleep_Timer_UsingFixedFunction)

        Sleep_Timer_WriteCounter(Sleep_Timer_backup.TimerUdb);
        Sleep_Timer_STATUS_MASK =Sleep_Timer_backup.InterruptMaskValue;
        #if (Sleep_Timer_UsingHWCaptureCounter)
            Sleep_Timer_SetCaptureCount(Sleep_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Sleep_Timer_UDB_CONTROL_REG_REMOVED)
            Sleep_Timer_WriteControlRegister(Sleep_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Sleep_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sleep_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Sleep_Timer_Sleep(void) 
{
    #if(!Sleep_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Sleep_Timer_CTRL_ENABLE == (Sleep_Timer_CONTROL & Sleep_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Sleep_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Sleep_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Sleep_Timer_Stop();
    Sleep_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Sleep_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sleep_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Sleep_Timer_Wakeup(void) 
{
    Sleep_Timer_RestoreConfig();
    #if(!Sleep_Timer_UDB_CONTROL_REG_REMOVED)
        if(Sleep_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Sleep_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

/*******************************************************************************
* File Name: Clock_Output.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Clock_Output.h"

static Clock_Output_BACKUP_STRUCT  Clock_Output_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Clock_Output_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Clock_Output_SUT.c usage_Clock_Output_Sleep_Wakeup
*******************************************************************************/
void Clock_Output_Sleep(void)
{
    #if defined(Clock_Output__PC)
        Clock_Output_backup.pcState = Clock_Output_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Clock_Output_backup.usbState = Clock_Output_CR1_REG;
            Clock_Output_USB_POWER_REG |= Clock_Output_USBIO_ENTER_SLEEP;
            Clock_Output_CR1_REG &= Clock_Output_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Clock_Output__SIO)
        Clock_Output_backup.sioState = Clock_Output_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Clock_Output_SIO_REG &= (uint32)(~Clock_Output_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Clock_Output_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Clock_Output_Sleep() for an example usage.
*******************************************************************************/
void Clock_Output_Wakeup(void)
{
    #if defined(Clock_Output__PC)
        Clock_Output_PC = Clock_Output_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Clock_Output_USB_POWER_REG &= Clock_Output_USBIO_EXIT_SLEEP_PH1;
            Clock_Output_CR1_REG = Clock_Output_backup.usbState;
            Clock_Output_USB_POWER_REG &= Clock_Output_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Clock_Output__SIO)
        Clock_Output_SIO_REG = Clock_Output_backup.sioState;
    #endif
}


/* [] END OF FILE */

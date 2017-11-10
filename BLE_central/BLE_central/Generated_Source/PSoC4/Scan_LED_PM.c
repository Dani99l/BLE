/*******************************************************************************
* File Name: Scan_LED.c  
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
#include "Scan_LED.h"

static Scan_LED_BACKUP_STRUCT  Scan_LED_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Scan_LED_Sleep
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
*  \snippet Scan_LED_SUT.c usage_Scan_LED_Sleep_Wakeup
*******************************************************************************/
void Scan_LED_Sleep(void)
{
    #if defined(Scan_LED__PC)
        Scan_LED_backup.pcState = Scan_LED_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Scan_LED_backup.usbState = Scan_LED_CR1_REG;
            Scan_LED_USB_POWER_REG |= Scan_LED_USBIO_ENTER_SLEEP;
            Scan_LED_CR1_REG &= Scan_LED_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Scan_LED__SIO)
        Scan_LED_backup.sioState = Scan_LED_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Scan_LED_SIO_REG &= (uint32)(~Scan_LED_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Scan_LED_Wakeup
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
*  Refer to Scan_LED_Sleep() for an example usage.
*******************************************************************************/
void Scan_LED_Wakeup(void)
{
    #if defined(Scan_LED__PC)
        Scan_LED_PC = Scan_LED_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Scan_LED_USB_POWER_REG &= Scan_LED_USBIO_EXIT_SLEEP_PH1;
            Scan_LED_CR1_REG = Scan_LED_backup.usbState;
            Scan_LED_USB_POWER_REG &= Scan_LED_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Scan_LED__SIO)
        Scan_LED_SIO_REG = Scan_LED_backup.sioState;
    #endif
}


/* [] END OF FILE */

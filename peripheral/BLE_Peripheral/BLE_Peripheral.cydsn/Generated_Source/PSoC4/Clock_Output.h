/*******************************************************************************
* File Name: Clock_Output.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Clock_Output_H) /* Pins Clock_Output_H */
#define CY_PINS_Clock_Output_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Clock_Output_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Clock_Output_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Clock_Output_Read(void);
void    Clock_Output_Write(uint8 value);
uint8   Clock_Output_ReadDataReg(void);
#if defined(Clock_Output__PC) || (CY_PSOC4_4200L) 
    void    Clock_Output_SetDriveMode(uint8 mode);
#endif
void    Clock_Output_SetInterruptMode(uint16 position, uint16 mode);
uint8   Clock_Output_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Clock_Output_Sleep(void); 
void Clock_Output_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Clock_Output__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Clock_Output_DRIVE_MODE_BITS        (3)
    #define Clock_Output_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Clock_Output_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Clock_Output_SetDriveMode() function.
         *  @{
         */
        #define Clock_Output_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Clock_Output_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Clock_Output_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Clock_Output_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Clock_Output_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Clock_Output_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Clock_Output_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Clock_Output_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Clock_Output_MASK               Clock_Output__MASK
#define Clock_Output_SHIFT              Clock_Output__SHIFT
#define Clock_Output_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Clock_Output_SetInterruptMode() function.
     *  @{
     */
        #define Clock_Output_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Clock_Output_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Clock_Output_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Clock_Output_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Clock_Output__SIO)
    #define Clock_Output_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Clock_Output__PC) && (CY_PSOC4_4200L)
    #define Clock_Output_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Clock_Output_USBIO_DISABLE              ((uint32)(~Clock_Output_USBIO_ENABLE))
    #define Clock_Output_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Clock_Output_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Clock_Output_USBIO_ENTER_SLEEP          ((uint32)((1u << Clock_Output_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Clock_Output_USBIO_SUSPEND_DEL_SHIFT)))
    #define Clock_Output_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Clock_Output_USBIO_SUSPEND_SHIFT)))
    #define Clock_Output_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Clock_Output_USBIO_SUSPEND_DEL_SHIFT)))
    #define Clock_Output_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Clock_Output__PC)
    /* Port Configuration */
    #define Clock_Output_PC                 (* (reg32 *) Clock_Output__PC)
#endif
/* Pin State */
#define Clock_Output_PS                     (* (reg32 *) Clock_Output__PS)
/* Data Register */
#define Clock_Output_DR                     (* (reg32 *) Clock_Output__DR)
/* Input Buffer Disable Override */
#define Clock_Output_INP_DIS                (* (reg32 *) Clock_Output__PC2)

/* Interrupt configuration Registers */
#define Clock_Output_INTCFG                 (* (reg32 *) Clock_Output__INTCFG)
#define Clock_Output_INTSTAT                (* (reg32 *) Clock_Output__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Clock_Output_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Clock_Output__SIO)
    #define Clock_Output_SIO_REG            (* (reg32 *) Clock_Output__SIO)
#endif /* (Clock_Output__SIO_CFG) */

/* USBIO registers */
#if !defined(Clock_Output__PC) && (CY_PSOC4_4200L)
    #define Clock_Output_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Clock_Output_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Clock_Output_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Clock_Output_DRIVE_MODE_SHIFT       (0x00u)
#define Clock_Output_DRIVE_MODE_MASK        (0x07u << Clock_Output_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Clock_Output_H */


/* [] END OF FILE */

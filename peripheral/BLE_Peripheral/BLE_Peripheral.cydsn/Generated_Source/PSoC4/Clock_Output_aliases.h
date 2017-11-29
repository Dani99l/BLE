/*******************************************************************************
* File Name: Clock_Output.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Clock_Output_ALIASES_H) /* Pins Clock_Output_ALIASES_H */
#define CY_PINS_Clock_Output_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Clock_Output_0			(Clock_Output__0__PC)
#define Clock_Output_0_PS		(Clock_Output__0__PS)
#define Clock_Output_0_PC		(Clock_Output__0__PC)
#define Clock_Output_0_DR		(Clock_Output__0__DR)
#define Clock_Output_0_SHIFT	(Clock_Output__0__SHIFT)
#define Clock_Output_0_INTR	((uint16)((uint16)0x0003u << (Clock_Output__0__SHIFT*2u)))

#define Clock_Output_INTR_ALL	 ((uint16)(Clock_Output_0_INTR))


#endif /* End Pins Clock_Output_ALIASES_H */


/* [] END OF FILE */

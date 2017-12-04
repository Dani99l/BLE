/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "wdt_timer.h"

CY_ISR(WDT_ISR)
{
    //  LED toggle to test if ISR code is running
        output_pin_1_Write(0u); 
        CyDelay(200);
         output_pin_1_Write(1u);
        CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
}

void setup_wdt(){
    
     /* Setup WDT Counter 0 to generate interrupt on match */ 
    CySysWdtSetMode(CY_SYS_WDT_COUNTER0,CY_SYS_WDT_MODE_INT); 

    /* Write match count */ 
    CySysWdtSetMatch(CY_SYS_WDT_COUNTER0, PERIOD); 
     
    /* Enable clear of counter on match */ 
    CySysWdtSetClearOnMatch(CY_SYS_WDT_COUNTER0, 1u); 

    /* Enable WDT counter 0 */ 
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);

//    // Test if WDT resets...
//    output_pin_2_Write(0u);
//    CyDelay(2000);
//    output_pin_2_Write(1u);

    /* Map interrupt vector to the WDT_ISR */ 
    isr_WDT_StartEx(WDT_ISR);  
    /* Enable Global Interrupt */ 
    CyGlobalIntEnable;
    
     /* 200us delay to ensure WDT configuration is complete */ 
    CyDelayUs(200); 
}

/* [] END OF FILE */
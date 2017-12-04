/* ========================================
 *
 * All function to enter to sleep mode
 * ========================================
*/

#include "Sleep.h"
#include "wdt_timer.h"
#define LOW_POWER_MODE
int counter;

//void deepsleep_ble(){
//    
//     /* Variable declarations */
////    CYBLE_BLESS_STATE_T blePower;
//    uint8 interruptStatus ;
////    
//   /* Disable global interrupts to avoid any other tasks from interrupting this section of code*/
//     interruptStatus  = CyEnterCriticalSection();
//    
//    
//    #ifdef LOW_POWER_MODE
//        output_pin_2_Write(1);
//    #endif 
//    
//   // blePower = CyBle_GetBleSsState();
//    //CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
//  //  blePower = CyBle_GetBleSsState();
//  //  CyBle_EnterLPM(CYBLE_BLESS_SLEEP);
////    while(blePower != CYBLE_BLESS_STATE_DEEPSLEEP){
//    
//    output_pin_2_Write(1);
//    
//    while(CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP)!=CYBLE_BLESS_DEEPSLEEP){
//        CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
//           output_pin_1_Write(0);
//    }
////         output_pin_1_Write(1);
////    }
//    
//    #ifdef LOW_POWER_MODE
//        output_pin_2_Write(0);
//    #endif
//    CyGlobalIntEnable;
//    
//     /* Get current state of BLE sub system to check if it has successfully entered deep sleep state */
////    blePower = CyBle_GetBleSsState();
//     
////      
////    if((blePower == CYBLE_BLESS_STATE_DEEPSLEEP || blePower == CYBLE_BLESS_STATE_ECO_ON))
////    {    
////    
////        #ifdef LOW_POWER_MODE
////            output_pin_1_Write(1);
////        #endif /* End of #if DEBUG_ENABLE */
////        
////        CySysPmDeepSleep();
////         
////        #ifdef LOW_POWER_MODE
////            output_pin_1_Write(0);
////        #endif /* End of #if DEBUG_ENABLE */
////    }
////   
////    else if((blePower != CYBLE_BLESS_STATE_EVENT_CLOSE))
////    {
////        /* change HF clock source from IMO to ECO, as IMO can be stopped to save power */
////                    CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_ECO); 
////                    /* stop IMO for reducing power consumption */
////                    CySysClkImoStop(); 
////                    /* put the CPU to sleep */
////        #ifdef LOW_POWER_MODE
////                    output_pin_1_Write(0);
////        #endif /* End of #if DEBUG_ENABLE */      
////
////                    CySysPmSleep();
////                    
////        #ifdef LOW_POWER_MODE
////                    output_pin_1_Write(1);
////        #endif /* End of #if DEBUG_ENABLE */           
////                    /* starts execution after waking up, start IMO */
////                    CySysClkImoStart();
////                    /* change HF clock source back to IMO */
////                    CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_IMO);
////                    
////    }
//   CyExitCriticalSection(interruptStatus);
//
//   //CyBle_ProcessEvents();
//}
void deepsleep_ble(){
    
     /* Variable declarations */
    CYBLE_BLESS_STATE_T blePower;
    uint8 interruptStatus ;
    
   /* Disable global interrupts to avoid any other tasks from interrupting this section of code*/
    interruptStatus  = CyEnterCriticalSection();
    
    
    #ifdef DEBUG_ble
        output_pin_2_Write(1);
    #endif /* End of #if DEBUG_ENABLE */
    
    CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
    
    #ifdef DEBUG_ble
        output_pin_2_Write(0);
    #endif /* End of #if DEBUG_ENABLE */
    
     /* Get current state of BLE sub system to check if it has successfully entered deep sleep state */
    blePower = CyBle_GetBleSsState();
     
      
    if((blePower == CYBLE_BLESS_STATE_DEEPSLEEP || blePower == CYBLE_BLESS_STATE_ECO_ON))
    {    
    
        #ifdef DEBUG_ble
            output_pin_1_Write(1);
        #endif /* End of #if DEBUG_ENABLE */
        
        CySysPmDeepSleep();
         
        #ifdef DEBUG_ble
            output_pin_1_Write(0);
        #endif /* End of #if DEBUG_ENABLE */
    }
   
    else if((blePower != CYBLE_BLESS_STATE_EVENT_CLOSE))
    {
        /* change HF clock source from IMO to ECO, as IMO can be stopped to save power */
                    CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_ECO); 
                    /* stop IMO for reducing power consumption */
                    CySysClkImoStop(); 
                    /* put the CPU to sleep */
        #ifdef DEBUG_ble
                    output_pin_1_Write(0);
        #endif /* End of #if DEBUG_ENABLE */      

                    CySysPmSleep();
                    
        #ifdef DEBUG_ble
                    output_pin_1_Write(1);
        #endif /* End of #if DEBUG_ENABLE */           
                    /* starts execution after waking up, start IMO */
                    CySysClkImoStart();
                    /* change HF clock source back to IMO */
                    CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_IMO);
                    
    }
    CyExitCriticalSection(interruptStatus );

    CyBle_ProcessEvents();
}

void sleep_ble(){

    
     /* put the CPU to sleep */
    UART_UartPutString("\n\r Going to sleep \n\r ");
    CyDelay(10);
    UART_SCB_IRQ_Disable();
    I2C_1_SCB_IRQ_Disable();
    BLE_bless_isr_Disable();
    setup_wdt(); //waits 32000u
    
    for(int i=0; i< 10; i++){
        
         CySysPmSleep();
    }
    
}

void deep_sleep_ble(){

    
     /* put the CPU to sleep */
    UART_UartPutString("\n\r Going to deep sleep \n\r ");
    CyDelay(10);
    UART_SCB_IRQ_Disable();
    I2C_1_SCB_IRQ_Disable();
    BLE_bless_isr_Disable();
    setup_wdt(); //waits  2s
   for(int i=0; i< 90; i++){
    
        CySysPmDeepSleep();
        output_pin_3_Write(1);
   }
    output_pin_3_Write(0);
}
/* [] END OF FILE */

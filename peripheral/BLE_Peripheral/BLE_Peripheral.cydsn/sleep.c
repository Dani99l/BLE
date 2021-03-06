/* ========================================
 *
 * All function to enter to sleep mode
 * ========================================
*/

#include "Sleep.h"
#include "wdt_timer.h"
#define LOW_POWER_MODE
int counter;


void sleep_ble(){

    
     /* put the CPU to sleep */
    UART_UartPutString("\n\r Going to sleep \n\r ");
    CyDelay(10);
    UART_SCB_IRQ_Disable();
    I2C_1_SCB_IRQ_Disable();
    BLE_bless_isr_Disable();
    setup_wdt(); //waits 32000u
    
    for(int i=0; i<2; i++){
        
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
   // setup_wdt(); //waits  2s
   for(int i=0; i< 2; i++){
         setup_wdt(); //waits  2s
        CySysPmDeepSleep();
       // output_pin_3_Write(1);
   }
    CyGlobalIntEnable;
    
    UART_Start();
    //output_pin_3_Write(0);
}

void wakeup_deepSleep()
{
    // it wakes up from deep or sleep mode from the interruption generated by the watchdog
    
    
    
    
}


void setTimerTosleep(){
        //call this function in START
        uint32 cc;
        UART_UartPutString("\n\r Input time window to sleep [in seconds] \n\r ");  
        cc=UART_UartGetByte();
        
        timeToSleep=cc;
        ciclos=cc/2;
        //
}
/* [] END OF FILE */

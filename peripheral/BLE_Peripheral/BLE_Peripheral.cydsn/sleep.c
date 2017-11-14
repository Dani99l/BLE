/* ========================================
 *
 * All function to enter to sleep mode
 * ========================================
*/

#include "Sleep.h"

#define LOW_POWER_MODE
int counter;
   
void goSleep(){
    
    #ifdef LOW_POWER_MODE    
            CYBLE_LP_MODE_T         lpMode;
            CYBLE_BLESS_STATE_T     blessState;
    #endif
  counter=0;
  setTimerTosleep();   
  while(counter<10)
    {              
       
                CySysPmSleep();
                CyGlobalIntEnable;
    }
    
        
        /*******************************************************************
        *  Process all pending BLE events in the stack
        *******************************************************************/       
        HandleBleProcessing();
        CyBle_ProcessEvents();
}

void setTimerTosleep(){
  
    while(1){
        Sleep_Timer_Init();
        Sleep_Timer_Start();
        if(Sleep_Timer_ReadCapture()<10){
          counter++;    
        }
    }
}

/* [] END OF FILE */

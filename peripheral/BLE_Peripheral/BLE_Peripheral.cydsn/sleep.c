/* ========================================
 *
 * All function to enter to sleep mode
 * ========================================
*/

#include "Sleep.h"


void goSleep(){
    
    
    CyBle_EnterLPM(CYBLE_BLESS_SLEEP);
    CyBle_ExitLPM();
    
}

void setTimerTosleep(){
    
    
    
}

/* [] END OF FILE */

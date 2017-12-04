/* ========================================
 *
 * contains function prototypes to BLE sleep mode
 * ========================================
*/

/***************************************
##Stack includes
***************************************/

#include "BLE_Stack.h"
#include "project.h"

/***************************************
##Function Prototypes
***************************************/

void setTimerTosleep();
void sleep_ble();
void deep_sleep_ble();
void wakeup_deepSleep();

/***************************************
##  Variables
***************************************/

int timeToSleep;
uint8 ciclos;
/* [] END OF FILE */

/*******************************************************************************
* File Name: app_Ble.c
*
* Description:
*  Common BLE application code for client devices.
*
*******************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "app_ble.h"

/* MTU size to be used by Client and Server after MTU exchange */
uint16 mtuSize      = CYBLE_GATT_MTU;   


/*******************************************************************************
* Function Name: HandleBleProcessing
********************************************************************************
*
* Summary:
*   Handles the BLE state machine for intiating different procedures
*   during different states of BLESS.
*
* Parameters:
*   None.
*
* Return:
*   None.
*
*******************************************************************************/

CYBLE_STATE_T cyBle_last_state;

void HandleBleProcessing(void)
{    
    uint8 txDataClientConfigDesc[2];
   
    CYBLE_GATT_HANDLE_VALUE_PAIR_T  attrValue = { 
                                                    {(uint8 *)txDataClientConfigDesc, 2, 2}, 
                                                    CYBLE_SERVER_UART_SERVER_UART_TX_DATA_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE
                                                };
    switch (cyBle_state)
    {
        case CYBLE_STATE_ADVERTISING:
            if (cyBle_last_state != cyBle_state) {
                #ifdef PRINT_MESSAGE_LOG   
                UART_UartPutString("\n\r CYBLE_STATE_ADVERTISING \n\r ");
                #endif            
            }
            break;
        
        case CYBLE_STATE_CONNECTED:
        if (cyBle_last_state != cyBle_state) {
                #ifdef PRINT_MESSAGE_LOG   
                UART_UartPutString("\n\r CYBLE_STATE_CONNECTED \n\r ");
                #endif
        }
            /* read CCCD for TX characteristic for checking if notifications are enabled*/
            CyBle_GattsReadAttributeValue(&attrValue, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
            
            /* if stack is free, handle UART traffic */
            if(CyBle_GattGetBusStatus() != CYBLE_STACK_STATE_BUSY)
            {
                HandleUartTxTraffic((uint16)txDataClientConfigDesc[0]);
            }
            break;
                
        case CYBLE_STATE_DISCONNECTED:
            if (cyBle_last_state != cyBle_state) {
                #ifdef PRINT_MESSAGE_LOG   
                UART_UartPutString("\n\r CYBLE_STATE_DISCONNECTED \n\r ");
                #endif
            }
            txDataClientConfigDesc[0] = NOTIFICATON_DISABLED;
            txDataClientConfigDesc[1] = NOTIFICATON_DISABLED;
            
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);

            break;
       
        case CYBLE_STATE_INITIALIZING:
            if (cyBle_last_state != cyBle_state) {
                #ifdef PRINT_MESSAGE_LOG   
                UART_UartPutString("\n\r CYBLE_STATE_INITIALIZING \n\r ");
                #endif            
            }
            break;
        case CYBLE_STATE_STOPPED:
            if (cyBle_last_state != cyBle_state) {
                #ifdef PRINT_MESSAGE_LOG   
                UART_UartPutString("\n\r CYBLE_STATE_STOPPED \n\r ");
                #endif                            
            }
            break;
        default:
            break;       
    }
    cyBle_last_state = cyBle_state;
}




/* [] END OF FILE */

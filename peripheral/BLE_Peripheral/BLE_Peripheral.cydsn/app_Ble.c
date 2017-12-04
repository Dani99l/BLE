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
                    /* RESET Uart and flush all buffers */

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
            
            UART_Stop();
            UART_SpiUartClearTxBuffer();
            UART_SpiUartClearRxBuffer();
            UART_Start();
            
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



/*******************************************************************************
* Function Name: AppCallBack
********************************************************************************
*
* Summary:
*   Call back function for BLE stack to handle BLESS events
*
* Parameters:
*   event       - the event generated by stack
*   eventParam  - the parameters related to the corresponding event
*
* Return:
*   None.
*
*******************************************************************************/
void AppCallBack(uint32 event, void *eventParam)
{   
    CYBLE_GATT_ERR_CODE_T           errorCode;
    CYBLE_GATTS_WRITE_REQ_PARAM_T   *writeReqParam;
    
    switch (event)
    {
        case CYBLE_EVT_STACK_ON:
                #ifdef PRINT_MESSAGE_LOG   
                 UART_UartPutString("\n\r CYBLE_EVT_STACK_ON \n\r ");
                #endif            
            break;
            
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
                #ifdef PRINT_MESSAGE_LOG   
               //  UART_UartPutString("\n\r CYBLE_EVT_GAP_DEVICE_DISCONNECTED \n\r ");
                #endif
            break;
            
        case CYBLE_EVT_GATT_CONNECT_IND:
                #ifdef PRINT_MESSAGE_LOG   
                 UART_UartPutString("\n\r CYBLE_EVT_GATT_CONNECT_IND \n\r ");
                #endif                
            break;
        
        case CYBLE_EVT_GATTS_WRITE_CMD_REQ:
        /** 'Write Command' Request from client device.*/
                #ifdef PRINT_MESSAGE_LOG   
                 UART_UartPutString("\n\r CYBLE_EVT_GATTS_WRITE_CMD_REQ \n\r ");
                #endif 
                HandleUartRxTraffic((CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam);
            break;
        
        case CYBLE_EVT_GATTS_XCNHG_MTU_REQ:
            /** 'GATT MTU Exchange Request'*/
                
            #ifdef PRINT_MESSAGE_LOG       
                 UART_UartPutString("\n\r CYBLE_EVT_GATTS_XCNHG_MTU_REQ \n\r ");
            #endif
                
            if(CYBLE_GATT_MTU > ((CYBLE_GATT_XCHG_MTU_PARAM_T *)eventParam)->mtu)
            {
                mtuSize = ((CYBLE_GATT_XCHG_MTU_PARAM_T *)eventParam)->mtu;
            }
            else
            {
                mtuSize = CYBLE_GATT_MTU;
            }
            
            break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
                 #ifdef PRINT_MESSAGE_LOG       
                 UART_UartPutString("\n\r CYBLE_EVT_GATTS_WRITE_REQ \n\r ");
                 #endif
          
            break;
        
        default:
            break;
    }
}
    

/* [] END OF FILE */

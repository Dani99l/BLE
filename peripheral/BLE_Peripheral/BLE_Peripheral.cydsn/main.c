/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
* Works as the slave(periferical) sensor, connects with the master(server);
* based on example code from cypress -Project 20
*
*******************************************************************************/

#include "main.h"
#include "stdio.h"


int main()
{       
          
   #ifdef LOW_POWER_MODE    
            CYBLE_LP_MODE_T         lpMode;
          //  CYBLE_BLESS_STATE_T     blessState
    #endif
    start();
    startBLE();
  
    while(1){
       state_machine();
    }

}
    

// State machine allows to read the sensors, set the ble component and send data to the periferical

void state_machine(){  
     switch(mode){
        
     case START:
        // init interrupts
        start();
        UART_UartPutString("\n\r START  \n\r ");
        mode=SENSOR_PACKET;
        break;
        
     case SENSOR_PACKET:
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r SENSOR \n\r ");     
        #endif
        I2C_1_Start(); 
        createPacket();
        mode=TX;
        break;
        
     case TX:
        
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r After TX mode \n\r ");
        #endif
        
        tx();

        mode=SLEEP;
        break;

     case SLEEP:
        
        //go to sleep mode untill next time to operate! 
        //check sleep time functions
        goSleep();
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r After sleep mode \n\r ");
        #endif
        mode=START;
        break;
     
     default:
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r DEGUB-> Blocked in DEFAULD Switch modes");
        #endif
        
        break;
    }
}
        
void tx(){
    
       startBLE();
    
       int check=0;
    
        while(check<1){
            
               while(CyBle_GetState()!= CYBLE_STATE_CONNECTED ){
                   HandleBleProcessing();
                   CyBle_ProcessEvents();
                   UART_UartPutString("\n\r ++  \n\r ");
               }
              // CyDelay(5);
                sendtoble();
               if(CyBle_GattGetBusStatus() != CYBLE_STACK_STATE_BUSY){
                    
                    check++;
                    UART_UartPutString("\n\r 00  \n\r ");
                }
            
                 UART_UartPutString("\n\r After  \n\r ");
    //            if (count>3) {
    //                count=0;
    //              //CyBle_Stop();
    //            }
            
    //            if(CyBle_GetState()== CYBLE_STATE_STOPPED) { 
    //                start();
    //                startBLE();
    //            }
                    
                HandleBleProcessing();
                CyBle_ProcessEvents();
                
        }
        stopBLE();
}

void Getsensor_data(){
       
    #ifdef DEBUG_ble  
     UART_UartPutString("\n\r Getsensor_data function \n\r ");     
    #endif
    
     if(lasTemp==sensor.temperature){
        
        sameTemp++;
    }
    
     if(lastHum==sensor.humidity){
        
        sameHum++;
    }
    lasTemp=sensor.temperature;
    lastHum=sensor.humidity;
}

void init_globalVariables(){
    
    count=0;
    lasTemp=0;
    lastHum=0;
    sameTemp=0;
    sameHum=0;
    sensor.ID=0;
    sensor.sequence=0;
    sensor.humidity=0;
    sensor.pressure=0;
    sensor.temperature=0;
    
}

void createPacket(){
 
    // Packet is a buffer with total data that will be sent 
    // It depends on MTU(maximum bytes allowed )
    
    sensor.humidity= getHum();
    sensor.temperature = getTemp();
    sensor.pressure=3;
    sensor.ID=2;
    sensor.sequence=count;
    count++;
    
    if(count>=250){
        count=0;
    }
    #ifdef DEBUG_ble  
     UART_UartPutString("\n\r After construct packet \n\r ");     
    #endif
    
//    buffer[PACKET_LENGHT]=(uint8)sensor.humidity;
//    buffer[PACKET_LENGHT-1]=(uint8)sensor.temperature;
//    buffer[PACKET_LENGHT-2]=(uint8)sensor.pressure;
//    buffer[PACKET_LENGHT-3]=(uint8)sensor.ID;
//    buffer[PACKET_LENGHT-4]=(uint8)sensor.sequence;    

}


void startBLE(){
   //This function init the BLE component, this means, BLE STACK 
   // AppCallBack manages the state BLE machine
    CyBle_Start(AppCallBack);
}

void stopBLE(){
    //Stops any proccesing in BLE Stack
    CyBle_Stop();
}

void start(){
  
    CyGlobalIntEnable;
    
    UART_Start();
    init_globalVariables();
    
    UART_UartPutString("\n\r --------------------- \n\r ");
    UART_UartPutString("\n\r Start peripheral role \n\r ");
    UART_UartPutString("\n\r --------------------- \n\r ");
}


void sendtoble()
{
    UART_UartPutString("\n\r papapap \n\r ");
   //this functions sends data from uart RX and pushes it to the server 
    
    CYBLE_GATTS_HANDLE_VALUE_NTF_T      uartTxDataNtf;
   // uint16  uartTxDataLength;
    
//    buffer[PACKET_LENGHT-5]=(uint8)sensor.humidity;
//    buffer[PACKET_LENGHT-4]=(uint8)sensor.temperature;
//    buffer[PACKET_LENGHT-3]=(uint8)sensor.pressure;
//    buffer[PACKET_LENGHT-2]=(uint8)sensor.ID;
//    buffer[PACKET_LENGHT-1]=(uint8)sensor.sequence;    
      
    buffer[PACKET_LENGHT-5]=5;
    buffer[PACKET_LENGHT-4]=4;
    buffer[PACKET_LENGHT-3]=3;
    buffer[PACKET_LENGHT-2]=2;
    buffer[PACKET_LENGHT-1]=1;  
    
    uartTxDataNtf.value.val  = buffer;
    uartTxDataNtf.value.len  = sizeof(buffer);
    uartTxDataNtf.attrHandle = CYBLE_SERVER_UART_SERVER_UART_TX_DATA_CHAR_HANDLE;
  
    do{
        UART_UartPutString("\n\r -- \n\r ");
        api=CyBle_GattsNotification(cyBle_connHandle, &uartTxDataNtf);
        CyBle_ProcessEvents();
    }while(api!=CYBLE_ERROR_OK);  
}
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
          
    init_globalVariables();
    start();

    while(1){
       state_machine();
    }
}
    

// State machine allows to read the sensors, set the ble component and send data to the periferical

void state_machine(){  
     switch(mode){
        
     case START:
        // init interrupts
       //start();
        UART_UartPutString("\n\r START  \n\r ");
        
        CyDelay(500);
        mode=SENSOR_PACKET;
        break;
        
     case SENSOR_PACKET:
        #ifdef PRINT_MESSAGE_LOG   
            UART_UartPutString("\n\r SENSOR \n\r ");     
        #endif
        
       
         createPacket();
        
        I2C_1_Stop();
        CyDelay(5);
        mode=TX;
        break;
        
     case TX:
        
        tx();
        
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r  TX  \n\r ");
        #endif

        mode=SLEEP;
        break;

     case SLEEP:
        {
        //go to sleep mode untill next time to operate! 
        //check sleep time functions
       char cc;
        
        cc=UART_UartGetChar();
        if(cc=='s'){
           sleep_ble();
         #ifdef PRINT_MESSAGE_LOG   
            UART_UartPutString("\n\r After sleep mode \n\r ");
        #endif
        }
        

        mode=START;
        }
        break;
     
     default:
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r DEGUB-> Blocked in DEFAULD Switch modes");
        #endif
        mode=START;
        break;
    }
}
        
void tx(){
       start();
       startBLE();
       CyDelay(5);
       int check=0;
       flag=1;
        while(flag && check<1){

            if(CyBle_GattGetBusStatus() != CYBLE_STACK_STATE_BUSY){
                sendtoble();
                check++;
               // UART_UartPutString("\n\r 00  \n\r ");
            }
            
            UART_UartPutString("\n\r After  \n\r ");
            HandleBleProcessing();
            CyBle_ProcessEvents();
                
        }
          HandleBleProcessing();
          CyBle_ProcessEvents();
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
    
    CyDelay(500);
    sensor.last_sequence=sensor.sequence;
    CyDelay(500);
    sensor.temperature = (uint8)getTemp();
    CyDelay(500);
    sensor.humidity= (uint8)getHum();
    sensor.pressure=3;
    sensor.ID=2;
    sensor.sequence=count;
    sensor.packets_lost=sensor.sequence-sensor.last_sequence;
    count++;
    
    if(count>=1000){
        count=0;
    }

    
    buffer[PACKET_LENGHT-1]=(uint8)sensor.humidity;
    buffer[PACKET_LENGHT-2]=(uint8)sensor.temperature;
    buffer[PACKET_LENGHT-3]=(uint8)sensor.ID;
    buffer[PACKET_LENGHT-4]=(uint8)sensor.packets_lost;
    buffer[PACKET_LENGHT-5]=(uint8)sensor.last_sequence;    
    buffer[PACKET_LENGHT-6]=(uint8)sensor.sequence;    
}


void startBLE(){
   //This function init the BLE component, this means, BLE STACK 
   // AppCallBack manages the state BLE machine
    
    CyBle_Start(AppCallBack);
    HandleBleProcessing();
    CyBle_ProcessEvents();
}

void stopBLE(){
    //Stops any proccesing in BLE Stack
    CyBle_Shutdown(); 
}

//int8 printRSSI(){
//    int8 rssi=0;
//    
//    rssi=CyBle_GetRssi();
//    rssi=rssi&0xff;
//    rssi=rssi-256;
//    UART_UartPutString((ultoa(rssi)));
//    
//    UART_UartPutString(" \n\r ");
//    UART_UartPutString(" dbm ");
//    UART_UartPutString(" \n\r ");
//    rssi=0;
//    return (rssi&0Xff);
//}



void start(){
  
    CyGlobalIntEnable;
    
    UART_Start();
    
    UART_UartPutString("\n\r --------------------- \n\r ");
    UART_UartPutString("\n\r Start peripheral role \n\r ");
    UART_UartPutString("\n\r --------------------- \n\r ");
}


void sendtoble()
{
    
   //this functions sends data from uart RX and pushes it to the server 
    
    CYBLE_GATTS_HANDLE_VALUE_NTF_T      uartTxDataNtf;
    
    uartTxDataNtf.value.val  = buffer;
    uartTxDataNtf.value.len  = sizeof(buffer);
    uartTxDataNtf.attrHandle = CYBLE_SERVER_UART_SERVER_UART_TX_DATA_CHAR_HANDLE;
  
    do{
        UART_UartPutString("\n\r -- \n\r ");
        api=CyBle_GattsNotification(cyBle_connHandle, &uartTxDataNtf);
        CyBle_ProcessEvents();
        flag=0;
    }while(api!=CYBLE_ERROR_OK && CyBle_GetState()== CYBLE_STATE_CONNECTED);  
}
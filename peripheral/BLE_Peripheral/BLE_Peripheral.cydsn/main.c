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
CY_ISR(WAKE_UP){
    isr_1_ClearPending();
}

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
        mode=SENSOR_PACKET;
        CyDelay(500);
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
        UART_UartPutString("\n\r After TX mode \n\r ");
        #endif

        mode=SLEEP;
        break;

     case SLEEP:
        //go to sleep mode untill next time to operate! 
        //check sleep time functions
       // goSleep();
        sleep_ble();
        #ifdef PRINT_MESSAGE_LOG   
        UART_UartPutString("\n\r After sleep mode \n\r ");
        #endif
        mode=START;
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
        //stopBLE();
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
    count++;
    
    if(count>=250){
        count=0;
    }

    
    buffer[PACKET_LENGHT-1]=(uint8)sensor.humidity;
    buffer[PACKET_LENGHT-2]=(uint8)sensor.temperature;
    buffer[PACKET_LENGHT-3]=(uint8)sensor.pressure;
    buffer[PACKET_LENGHT-4]=(uint8)sensor.ID;
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

void sleep_ble(){
    
     /* Variable declarations */
    CYBLE_BLESS_STATE_T blePower;
    uint8 interruptStatus ;
    
   CyBle_EnterLPM(CYBLE_BLESS_SLEEP);
   CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
    isr_1_StartEx(WAKE_UP);
    
    CyDelay(5);
    interruptStatus=CyEnterCriticalSection();
    blePower=CyBle_GetBleSsState();
    
      if((blePower == CYBLE_BLESS_STATE_DEEPSLEEP || blePower == CYBLE_BLESS_STATE_ECO_ON)){
            output_pin_1_Write(0);
           // CyDelay(1000);
            CySysPmDeepSleep();
            output_pin_1_Write(1);
            
            
        }
   
    while(CyBle_ExitLPM() != CYBLE_BLESS_ACTIVE){
        UART_UartPutString("\n\r exit lpmode \n\r ");
    };
    
    
    /* Enable interrupts */
    CyExitCriticalSection(interruptStatus );
 
    CyBle_ProcessEvents();
}

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
   // uint16  uartTxDataLength;
    
//    buffer[PACKET_LENGHT-5]=(uint8)sensor.humidity;
//    buffer[PACKET_LENGHT-4]=(uint8)sensor.temperature;
//    buffer[PACKET_LENGHT-3]=(uint8)sensor.pressure;
//    buffer[PACKET_LENGHT-2]=(uint8)sensor.ID;
//    buffer[PACKET_LENGHT-1]=(uint8)sensor.sequence;    
//      
//    buffer[PACKET_LENGHT-5]=5;
//    buffer[PACKET_LENGHT-4]=4;
//    buffer[PACKET_LENGHT-3]=3;
//    buffer[PACKET_LENGHT-2]=2;
//    buffer[PACKET_LENGHT-1]=1;  
//    
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
/* ========================================
 *
 * HTU21D Sensor 
 *  I2C interface communication

 * ========================================
*/

#include "htu21d.h"
#include "I2C_1.h"


int getTemp(){
    static uint8 softreset[0x01]={0xfe}; // Soft reset code
    static uint8 I2CReadBuffer[0x04]; //The uncompensated temperature read from the sensor is stored here
    static uint8 I2CWriteBuffer[0x01] = {0xE3}; //Command for temperature conversion - Register address and value
    static uint8 SetReadAddressBuffer[0x01] = {0x40}; //Register address from which the data is read
    //CyDelay(15);
    /* Do a write operation with the memory address bytes to get temperature data */
	I2C_1_I2CMasterWriteBuf(DEVICEADDRESS, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    CyDelay(5); //Wait for 5ms temperature conversion
 
    /* Read temperature from the sensor and store it in a buffer */
	I2C_1_I2CMasterReadBuf(DEVICEADDRESS, (uint8 *) I2CReadBuffer, sizeof(I2CReadBuffer), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
	UT = (I2CReadBuffer[0x00] << 0x08) | I2CReadBuffer[0x01]; //Return uncompensated temperature value
    
                            // Conversion //
    temp=UT;
    temp *= 175.72;
    temp /= 65536;
    temp -= 46.85;
    return (uint8)temp;
}

int getHum(){
    
    static uint8 I2CReadBuffer[0x04]; //The uncompensated humidity read from the sensor is stored here
    static uint8 I2CWriteBuffer[0x01] = {0xE5}; //Command for hui conversion - Register address and value1
    static uint8 SetReadAddressBuffer[0x01] = {0x40}; //Register address from which the data is read
    
    /* Do a write operation with the memory address bytes to get humidity data */
	I2C_1_I2CMasterWriteBuf(DEVICEADDRESS, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    CyDelay(5); //Wait for 5ms huidity conversion
    
    /* Read pressure from the sensor and store it in a buffer */
	I2C_1_I2CMasterReadBuf(DEVICEADDRESS, (uint8 *) I2CReadBuffer, sizeof(I2CReadBuffer), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
	UH =  (I2CReadBuffer[0x00] << 0x08) | I2CReadBuffer[0x01]; //Return uncompensated HUM value
    
                        // Conversion //
    hum = UH;
    hum *= 125;
    hum /= 65536;
    hum -= 6;
    
    return (uint8)hum;
}

/* [] END OF FILE */

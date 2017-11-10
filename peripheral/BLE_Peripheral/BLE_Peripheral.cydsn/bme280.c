/* ========================================
 *
 *  Device driver to read temperature, humidity and pressure from BME280 Sensor using I2C interface
 *
 * ========================================
*/

#include "bme280.h"
#include "I2C_1.h"
#include "project.h"
#include "teste.h"
#include "math.h"  

/*************************Variables Declaration*************************************************************************/
uint8 SensorCalibrationData[0x16]; //Calibration data is stored in this array
long X1, X2, X3, B3, B5, B6, UT, UP; //Variables to store calibration coefficients
unsigned long B4, B7; //Variables to store calibration coefficients
short AC1, AC2, AC3, B1, B2, MC, MD; //Variables to store calibration coefficients
unsigned short AC4, AC5, AC6; //Variables to store calibration coefficients
long UT, UP; //Variables to store uncompensated temperature and pressure data
long Temperature, Pressure, Altitude; //Variables to store the true temperature, pressure and altitude data
double PressureAtSeaLevel = 101325.0; //Pressure at sea level in Pascal
uint8 CalibrationDataAvailable = FALSE; //This flag is to whether the calibration data is available


int gettemp280(void)
{
    //I2C_1_Start(); 
    I2C_1_scl_SetDriveMode(I2C_1_scl_DM_OD_LO);
    I2C_1_sda_SetDriveMode(I2C_1_sda_DM_OD_LO);
    static uint8 I2CReadBufferT[0x02]; //The uncompensated temperature read from the sensor is stored here
    static uint8 I2CWriteBufferT[0x01] = {0xF4}; //Command for temperature conversion - Register address and value
    static uint8 SetReadAddressBufferT[0x01] = {0xF6}; //Register address from which the data is read
    
    /* Do a write operation with the memory address bytes to get temperature data */
	I2C_1_I2CMasterWriteBuf(I2CSlaveAddress, (uint8 *) I2CWriteBufferT, sizeof(I2CWriteBufferT), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    //CyDelay(5); //Wait for 5ms temperature conversion
    
//    /* In order to do a read from a specific address, do a write operation with the memory address bytes alone */
//	I2C_1_I2CMasterWriteBuf(I2CSlaveAddress, (uint8 *) SetReadAddressBufferT, sizeof(SetReadAddressBufferT), I2C_I2C_MODE_COMPLETE_XFER);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    /* Read temperature from the sensor and store it in a buffer */
	I2C_1_I2CMasterReadBuf(I2CSlaveAddress, (uint8 *) I2CReadBufferT, sizeof(I2CReadBufferT), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
	UT280 = (I2CReadBufferT[0x00] << 0x08) | I2CReadBufferT[0x01]; //Return uncompensated temperature value

    return UT280;
}
/***********************************************************************************************************************/

/*************************************************************************************************************************
* Function Name: ReadUncompensatedPressure
**************************************************************************************************************************
* Summary: This function writes the pressure conversion command to the sensor and
* waits for conversion to complete, and then reads the converted data (uncompensated pressure) and stores it in a variable.
*
* Parameters:
*  void
*
* Return:
*  void
*
*************************************************************************************************************************/
int getpress280(void)
{   I2C_1_scl_SetDriveMode(I2C_1_sda_DM_OD_LO);
    I2C_1_sda_SetDriveMode(I2C_1_sda_DM_OD_LO);
    I2C_1_Start(); 
    static uint8 I2CReadBufferP[0x03]; //The uncompensated pressure read from the sensor is stored here
    static uint8 I2CWriteBufferP[0x01] = {0xF7}; //0x34 + (OSS << 0x06)}; //Command for pressure conversion - Register address and value
    static uint8 SetReadAddressBufferP[0x01] = {0xF6}; //Register address from which the data is read
    
    /* Do a write operation with the memory address bytes to get pressure data */
	I2C_1_I2CMasterWriteBuf(I2CSlaveAddress, (uint8 *) I2CWriteBufferP, sizeof(I2CWriteBufferP), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    CyDelay(26); //Wait for 26ms pressure conversion
    
    /* In order to do a read from a specific address, do a write operation with the memory address bytes alone */
	I2C_1_I2CMasterWriteBuf(I2CSlaveAddress, (uint8 *) SetReadAddressBufferP, sizeof(SetReadAddressBufferP), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    /* Read pressure from the sensor and store it in a buffer */
	I2C_1_I2CMasterReadBuf(I2CSlaveAddress, (uint8 *) I2CReadBufferP, sizeof(I2CReadBufferP), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
	UP280 = ((I2CReadBufferP[0x00] << 0x10) | (I2CReadBufferP[0x01] << 0x08) | I2CReadBufferP[0x02]) >> (0x08 - OSS); //Return uncompensated pressure value
    
    return UP280;
}

int readPressure(){
    
    return Pressure;
}
int readTemp(){
    
    return Temperature;
}
void ReadSensorCalibrationData(void)
{
    
    static uint8 I2CWriteBuffer[0x01] = {0xAA}; //Location from which the calibration data is to be read
    //I2C_1_Start(); 
    /* Do a write operation with the memory address bytes to get calibration data */
	I2C_1_I2CMasterWriteBuf(I2CSlaveAddress, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    /* Read calibration from the sensor EEPROM and store it in a buffer */
	I2C_1_I2CMasterReadBuf(I2CSlaveAddress, (uint8 *) SensorCalibrationData, sizeof(SensorCalibrationData), I2C_I2C_MODE_COMPLETE_XFER);
    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
    
    CalibrationDataAvailable = TRUE; //Set sensor calibration data available flag
    
    /* Assign calibration data to coefficient variables as described in sensor datasheet */
    AC1 = (SensorCalibrationData[0x00] << 0x08) | SensorCalibrationData[0x01];
    AC2 = (SensorCalibrationData[0x02] << 0x08) | SensorCalibrationData[0x03];
    AC3 = (SensorCalibrationData[0x04] << 0x08) | SensorCalibrationData[0x05];
    AC4 = (SensorCalibrationData[0x06] << 0x08) | SensorCalibrationData[0x07];
    AC5 = (SensorCalibrationData[0x08] << 0x08) | SensorCalibrationData[0x09];
    AC6 = (SensorCalibrationData[0x0A] << 0x08) | SensorCalibrationData[0x0B];
    B1 = (SensorCalibrationData[0x0C] << 0x08) | SensorCalibrationData[0x0D];
    B2 = (SensorCalibrationData[0x0E] << 0x08) | SensorCalibrationData[0x0F];
    MC = (SensorCalibrationData[0x012] << 0x08) | SensorCalibrationData[0x013];
    MD = (SensorCalibrationData[0x014] << 0x08) | SensorCalibrationData[0x015];
  //  I2C_1_Stop();
}

void ReadAndCalculateSensorData(void)
{
    
//    /* Read the sensor calibration data if not already available */
//	if(!CalibrationDataAvailable)
//    {
//        ReadSensorCalibrationData(); //Read sensor calibration data
//        
//    }
    
    gettemp280(); //Read the uncompensated temperature value
    getpress280(); //Read the uncompensated pressure value
    
    /* Calculate the true temperature as described in sensor datasheet */
    X1 = (UT280 - AC6) * AC5 / pow(2, 15);
    X2 = MC * pow(2, 11) / (X1 + MD);
    B5 = X1 + X2;
  //  Temperature = (B5 + 8) / pow(2, 4);
   
    Temperature= gettemp280();
    UART_UartPutString("\n\r TEMP \n\r ");

    UART_UartPutString(ltoa(Temperature));
    
    
    /* Calculate the true barometric pressure as described in sensor datasheet */
    B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / pow(2, 12))) / pow(2, 11);
    X2 = AC2 * B6 / pow(2, 11);
    X3 = X1 + X2;
    B3 = (((AC1 * 4 + X3) << OSS) + 2) / 4;
    X1 = AC3 * B6 / pow(2, 13);
    X2 = (B1 * (B6 * B6 / pow(2, 12))) / pow(2, 16);
    X3 = ((X1 + X2) + 2) / pow(2, 2);
    B4 = (AC4 * (unsigned long)(X3 + 32768)) / pow(2, 15);
    B7 = ((unsigned long)UP280 - B3) * (50000 >> OSS);
    if(B7 < 0x80000000)
    {
        Pressure = (B7 *2) / B4;
    }
    else 
    {
        Pressure = (B7 / B4) *2;
    }
    X1 = (Pressure / pow(2, 8)) * (Pressure / pow(2, 8));
    X1 = (X1 * 3038) / pow(2, 16);
    X2 = (-7357 * Pressure) / pow(2, 16);
    //Pressure = Pressure + (X1 + X2 + 3791) / pow(2, 4);
    Pressure= getpress280(); 
    
    UART_UartPutString("\n\r PRESS \n\r ");
    UART_UartPutString(ltoa(Pressure));
    /* Calculate the true altitude based on pressure at sea level as described in sensor datasheet */
    //Altitude = 44330 * (1 - ( pow(Pressure/PressureAtSeaLevel, 1/5.255)));
}



//int gettemp280(){
//     
//    static uint8 I2CReadBuffer[0x04]; //The uncompensated temperature read from the sensor is stored here
//    static uint8 I2CWriteBuffer[0x02] = {PRESSTEMP}; //Command for temperature conversion - Register address and value
//    static uint8 SetReadAddressBuffer[0x02] = {TEMP280,PRESS280}; //Register address from which the data is read
//    
//     /* Do a write operation with the memory address bytes to get temperature data */
//	I2C_1_I2CMasterWriteBuf(DEVICEADDRESSBME, (uint8 *) SetReadAddressBuffer, sizeof(SetReadAddressBuffer), I2C_I2C_MODE_COMPLETE_XFER);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
//    
//  //  CyDelay(5); //Wait for 5ms temperature conversion
// 
//    /* Read temperature from the sensor and store it in a buffer */
//	I2C_1_I2CMasterReadBuf(DEVICEADDRESSBME, (uint8 *) SetReadAddressBuffer, sizeof(SetReadAddressBuffer), I2C_I2C_MODE_COMPLETE_XFER);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
//    
//	UT280 = (I2CReadBuffer[0x00] << 0x08) | I2CReadBuffer[0x01]; //Return uncompensated temperature value
//    temp280=UT280;
//    return temp280;
//}
//
//int getthum280(){
//    
//    static uint8 I2CReadBuffer[0x04]; //The uncompensated temperature read from the sensor is stored here
//    static uint8 I2CWriteBuffer[0x01] = {0x8A}; //Command for temperature conversion - Register address and value
//    static uint8 SetReadAddressBuffer[0x01] = {HUM280}; //Register address from which the data is read
//     
//    
//     /* Do a write operation with the memory address bytes to get temperature data */
//	I2C_1_I2CMasterWriteBuf(DEVICEADDRESSBME, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_I2C_MODE_NO_STOP);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
//    
//    CyDelay(5); //Wait for 5ms temperature conversion
// 
//    /* Read temperature from the sensor and store it in a buffer */
//	I2C_1_I2CMasterReadBuf(DEVICEADDRESSBME, (uint8 *) I2CReadBuffer, sizeof(I2CReadBuffer), I2C_I2C_MODE_NO_STOP);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
//    
//	UH280 = (I2CReadBuffer[0x00] << 0x08) | I2CReadBuffer[0x01]; //Return uncompensated temperature value
//    hum280=UH280;
//    
//    return hum280;
//}
//
//int gettpress280(){
//    static uint8 I2CReadBuffer[0x04]; //The uncompensated temperature read from the sensor is stored here
//    static uint8 I2CWriteBuffer[0x01] = {0x8A}; //Command for temperature conversion - Register address and value
//    static uint8 SetReadAddressBuffer[0x01] = {HUM280}; //Register address from which the data is read
//     
//    
//     /* Do a write operation with the memory address bytes to get temperature data */
//	I2C_1_I2CMasterWriteBuf(DEVICEADDRESSBME, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_I2C_MODE_NO_STOP);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
//    
//    CyDelay(5); //Wait for 5ms temperature conversion
// 
//    /* Read temperature from the sensor and store it in a buffer */
//	I2C_1_I2CMasterReadBuf(DEVICEADDRESSBME, (uint8 *) I2CReadBuffer, sizeof(I2CReadBuffer), I2C_I2C_MODE_NO_STOP);
//    while(!(I2C_1_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
//    I2C_1_I2CMasterClearStatus(); //Clear I2C master status
//    
//	UP280 = (I2CReadBuffer[0x00] << 0x08) | I2CReadBuffer[0x01]; //Return uncompensated temperature value
//    press280=UP280;  
//    
//   return press280;
//}



                // Calibration Proccess defined by Bosch
//
//// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
//// t_fine carries fine temperature as global value
//BME280_S32_t t_fine;
//BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T)
//{
//BME280_S32_t var1, var2, T;
//var1 = ((((adc_T>>3) – ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
//var2 = (((((adc_T>>4) – ((BME280_S32_t)dig_T1)) * ((adc_T>>4) – ((BME280_S32_t)dig_T1))) >> 12) *
//((BME280_S32_t)dig_T3)) >> 14;
//t_fine = var1 + var2;
//T = (t_fine * 5 + 128) >> 8;
//return T;
//}
//// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
//// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
//BME280_U32_t BME280_compensate_P_int64(BME280_S32_t adc_P)
//{
//BME280_S64_t var1, var2, p;
//var1 = ((BME280_S64_t)t_fine) – 128000;
//var2 = var1 * var1 * (BME280_S64_t)dig_P6;
//var2 = var2 + ((var1*(BME280_S64_t)dig_P5)<<17);
//var2 = var2 + (((BME280_S64_t)dig_P4)<<35);
//var1 = ((var1 * var1 * (BME280_S64_t)dig_P3)>>8) + ((var1 * (BME280_S64_t)dig_P2)<<12);
//var1 = (((((BME280_S64_t)1)<<47)+var1))*((BME280_S64_t)dig_P1)>>33;
//if (var1 == 0)
//{
//return 0; // avoid exception caused by division by zero
//}
//p = 1048576-adc_P;
//p = (((p<<31)-var2)*3125)/var1;
//var1 = (((BME280_S64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
//var2 = (((BME280_S64_t)dig_P8) * p) >> 19;
//p = ((p + var1 + var2) >> 8) + (((BME280_S64_t)dig_P7)<<4);
//return (BME280_U32_t)p;
//}
//// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
//// Output value of “47445” represents 47445/1024 = 46.333 %RH
//BME280_U32_t bme280_compensate_H_int32(BME280_S32_t adc_H)
//{
//BME280_S32_t v_x1_u32r;
//v_x1_u32r = (t_fine – ((BME280_S32_t)76800));
//v_x1_u32r = (((((adc_H << 14) – (((BME280_S32_t)dig_H4) << 20) – (((BME280_S32_t)dig_H5) * v_x1_u32r)) +
//((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r * ((BME280_S32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((BME280_S32_t)dig_H3)) >> 11) + ((BME280_S32_t)32768))) >> 10) + ((BME280_S32_t)2097152)) *
//((BME280_S32_t)dig_H2) + 8192) >> 14));
//v_x1_u32r = (v_x1_u32r – (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((BME280_S32_t)dig_H1)) >> 4));
//v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
//v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
//return (BME280_U32_t)(v_x1_u32r>>12);
//}

/* [] END OF FILE */
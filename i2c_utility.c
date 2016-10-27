/*

i2c_utility.c

*/


#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "twi.h"
#include "serial_connector.h"
#include "i2c_utility.h"
#include "Arduino.h"

#define True 1
#define False 0



//Module Variables
static uint8_t  rxBuffer[BUFFER_LENGTH];
static uint8_t  rxBufferIndex            = 0;
static uint8_t  txBuffer[BUFFER_LENGTH];
static uint8_t  txBufferIndex            = 0;
static uint8_t  transmitting             = 0;

//DS1631 Temp Sensor Addresses, Registers, and Commands
static uint8_t  TS_DEVICE_ADDRESS     = 0x90 >> 1; // Address is shifted back in twi
static uint8_t  TS_CONFIG_REG         = 0xAC;
static uint8_t  TS_CONFIG_REG_STARTUP = 0x04; // 0.125 accuracy and continuous mode 
static uint16_t TS_CONFIG_REG_POW_ON  = 0x00AC; // twi is little endian.
                                               // Address needs to be sent first
static uint8_t TS_START     = 0x51;
static uint8_t TS_STOP      = 0x22;
static uint8_t TS_READ_TEMP = 0xAA;



//TCS3490 Light Sensor Config Registers
static uint8_t  LS_DEVICE_ADDRESS    = 0x72 >> 1; //Shifted back in twi
static uint8_t  LS_ENABLE_REG        = 0x80;
static uint8_t  LS_ADC_POW_ON        = 0x03;
static uint16_t LS_ENABLE_REG_POW_ON = 0x0380; //Light sensor power on command. twi sends data little endian
static uint8_t  LS_STATUS_REG        = 0x93;


//Light Sensor Data Low Register Addresses
// Color registers are two bytes
#define LS_CDATAL 0x94 // Low Clear Data
#define LS_RDATAL 0X96 // Low Red   Data
#define LS_GDATAL 0X98 // Low Green Data
#define LS_BDATAL 0X9A // Low Blue  Data

#define TEMP_SCALE_FACTOR 10000   // 
#define TEMP_CONV_FACT    39      // 125 (max degrees C) / 0x7D00(max hex value) = .0039. We take decimal into account with TEMP_SCALE_FACTOR
#define TEMP_ACCURACY     125     // Accurate up to 0.125

//Light Sensor Color Data Container
static uint8_t LS_CDATA_CONTAINER[] = {LS_CDATAL,
                                       LS_RDATAL,
                                       LS_GDATAL,
                                       LS_BDATAL};


uint8_t configTWI(){
  

  rxBufferIndex = 0;
  txBufferIndex = 0;
  transmitting = 1;

  twi_init();

  return 0;

}

void stop_i2c(){
  transmitting = 0;
  twi_disable();
}


uint8_t startUpTempSensor(){
   if(!transmitting){configTWI();}
   uint8_t tempData;

   // Set configuration register
   // 0x00 == 0.5 accuracy and continuous data sampling
   twi_writeTo(TS_DEVICE_ADDRESS, &TS_CONFIG_REG_POW_ON, 2, 1, True); // Target config reg and write config
   twi_writeTo(TS_DEVICE_ADDRESS, &TS_CONFIG_REG, 1, 1, True); //Target config
   twi_readFrom(TS_DEVICE_ADDRESS, &tempData, 1, True); // Read config
   //Config reads 0x90. Is this right?
   // 0x90 == Done, Memory busy

   // Start ADC conversion
   twi_writeTo(TS_DEVICE_ADDRESS, &TS_START, 1, 1, True);

   return 0;

}

void pollTempData(uint8_t* spliced_tempData){
  //cnt_printString("polling");
  uint16_t tempData = 0x0000;
  uint16_t highData = 0x0000;
  uint16_t lowData  = 0x0000;

  twi_writeTo(TS_DEVICE_ADDRESS, &TS_READ_TEMP, 1, 1, False);
  twi_readFrom(TS_DEVICE_ADDRESS, &tempData, 2, True);

  //twi is little endian. So swap high & low
  highData |= tempData & 0x00FF;
  highData  = highData << 8;
  lowData  |= tempData & 0xFF00;
  lowData   = lowData >> 8;
  tempData ^= tempData; // Clear
  tempData |= ( highData | lowData );
  

  spliceTempData(tempData, spliced_tempData);
}

// raw_tempData is a 2byte, big endian 
void spliceTempData(uint16_t raw_tempData, uint8_t* spliced_tempData){

  uint32_t  full_dec_tempData;       // Hold temp data for numeric conversion
  uint8_t   whole_num_tempData;      // Data to the left of the decimal
  uint8_t   decimal_num_tempData;    // Accuracy is 0.125 so...

  full_dec_tempData = (uint32_t)raw_tempData * TEMP_CONV_FACT;
  whole_num_tempData = full_dec_tempData / TEMP_SCALE_FACTOR;
  decimal_num_tempData = full_dec_tempData % TEMP_SCALE_FACTOR;
  
  //Configure decimal accuracy?
  spliced_tempData[0] = whole_num_tempData;
  spliced_tempData[1] = decimal_num_tempData; 

}


uint8_t startupLightSensor(){
  if(!transmitting){configTWI();}
  uint8_t tempData;

  // uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
  // Will return:
  // 0 if sucess
  // 2 if address send, nak recieved
  // 3 if data sent, nak recieved
  // 4 other twi error           
  //                                         
  //cnt_printString("lightsensor");

  //                             |- Must send start address of data
  twi_writeTo(LS_DEVICE_ADDRESS, &LS_ENABLE_REG_POW_ON, 2, 1, False); // Target Enable Register and turn on pow and adc
  //twi_writeTo(LS_DEVICE_ADDRESS, &LS_ENABLE_REG, 1, 1, True);    // Target enable reg
  twi_readFrom(LS_DEVICE_ADDRESS, &tempData, 1, False);          // Read enable reg
  twi_writeTo(LS_DEVICE_ADDRESS, &LS_STATUS_REG, 1, 1, False);   //Target Status Reg
  twi_readFrom(LS_DEVICE_ADDRESS, &tempData, 1, True);           // Read status reg

  return 0;
}


//TODO: Pass in data_container
//Fill an array of CLEAR, RED, GREEN, and BLUE data
void pollLightData(uint16_t* lightData){

  uint8_t low_byte_address;
  uint8_t high_byte_address;
  uint16_t data_container[LS_CDATA_CONTAINER_LEN];
  
  //cnt_printString("lightsensor");



  for(int i = 0; i < LS_CDATA_CONTAINER_LEN; i++){
    uint16_t sensorData = 0x00000000;
    uint8_t  tempData =  0x0000;


    // Low and High Byte Addresses
    low_byte_address = LS_CDATA_CONTAINER[i]; // Get Low Address
    high_byte_address = low_byte_address + 1;

    // Get Low Byte
    twi_writeTo(LS_DEVICE_ADDRESS, &low_byte_address, 1, 1, False); // Target low address
    twi_readFrom(LS_DEVICE_ADDRESS, &tempData, 1, False);           // Read data into tempData
    sensorData |= tempData; // Low byte data so no shift
    tempData ^= tempData;  //Clear tempData
    
    //Get High Byte
    twi_writeTo(LS_DEVICE_ADDRESS, &high_byte_address, 1, 1, False); //Target high address
    twi_readFrom(LS_DEVICE_ADDRESS, &tempData, 1, False);
    sensorData |= (uint16_t)tempData<<8; // High byte so shift

    lightData[i] = sensorData;

  }
  //Stop Transmission
  twi_releaseBus();



}

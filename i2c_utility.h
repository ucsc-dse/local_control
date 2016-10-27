/*
i2c_utility.c
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#define BUFFER_LENGTH 2 		 // 2 Byte buffer size
#define LS_CDATA_CONTAINER_LEN 4 // Light Sensor Data Array Length

uint8_t    configTWI();          // Turn on TWI engine
uint8_t    startupLightSensor(); // start and configure sensors
uint8_t    startUpTempSensor();
void       pollLightData(uint16_t* lightData);      // Read 8 bytes of RED, BLUE, GREEN, CLEAR data
void   	   pollTempData(uint8_t* spliced_tempData);		 // Read 2 bytes of temperature data
void	   spliceTempData(uint16_t raw_tempData, uint8_t* spliced_tempData);
void 	   stop_i2c();


#ifdef __cplusplus
}
#endif

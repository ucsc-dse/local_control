#include "DataHandler.h"
#include "i2c_utility.h"
#include "Arduino.h"

using namespace std;


//Get Light Data
//lightData[0] : CLEAR
//lightData[1] : RED
//lightData[2] : GREEN
//lightData[3] : BLUE
void DataHandler::setLightData(){
	Serial.println("Polling Light Data");
	Serial.flush();

	pollLightData(_lightData);
}

void DataHandler::setTempData(){
	Serial.println("Polling Temp Data");
	Serial.flush();

	pollTempData(_tempData); // 2 byte temperature Data. Implied decimal point between elements
}

void DataHandler::setVoltageData(uint16_t* vData, int len){
	voltageData_len = len;
	_voltageData = vData;
}

void DataHandler::setCurrentData(uint16_t* cData, int len){
	currentData_len = len;
	_currentData = cData;
}


/*

void saveData()

Save sensor and power data to dataCache

*/
void DataHandler::saveData(){

	// Add Header
	uint16_t head = getDataHeader();
	dataCache[data_idx++] = head;

	// Add light data
	uint8_t numBytes = lightData_len * 2; // Each LightData is 2 bytes
	memcpy(&dataCache[data_idx], _lightData, numBytes); 
	data_idx += lightData_len;

	//Add temp data
	numBytes = tempData_len; // Each temp data is 1 bytes
	memcpy(&dataCache[data_idx], _tempData, numBytes);
	data_idx += tempData_len;	

	for(int i = 0; i < 255; i++){
		Serial.print(dataCache[i],HEX);
		Serial.flush();
	}

}

uint16_t DataHandler::getDataHeader(){
	return 0xFFFF;
}



uint16_t* DataHandler::getLightData(){
	return _lightData;
}

uint8_t* DataHandler::getTempData(){
	return _tempData;
}

uint16_t* DataHandler::getVoltageData(){
	return _voltageData;
}

uint16_t* DataHandler::getCurrentData(){
	return _currentData;
}


void DataHandler::serial_printTempData(){
	Serial.println("");
	Serial.println("TEMP DATA");
	Serial.print(_tempData[0]);
	Serial.print(".");
	Serial.print(_tempData[1]);
	Serial.flush();
}

void DataHandler::serial_printLightData(){
	Serial.println("");
	Serial.println("LIGHT DATA");
	Serial.print(_lightData[0]);
	Serial.print(_lightData[1]);
	Serial.print(_lightData[2]);
	Serial.print(_lightData[3]);
	Serial.flush();
}
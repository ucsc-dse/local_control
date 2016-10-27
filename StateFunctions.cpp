#include "StateClasses.h"
#include "DeviceOperation.h"
#include <string.h>

using namespace std;
/*
	Base State
*/
char* BaseState::getName(){
	return "----- Base State -----";
}
void BaseState::start(DeviceOperation * device_operator){

	//Stop State
	this->stop(device_operator);

}

void BaseState::stop(DeviceOperation * device_operator){
	
}


/*
	Data Collect State
*/
char* DataCollectState::getName(){
	return "----- Data Collect State -----";
}
void DataCollectState::start(DeviceOperation * device_operator){
	stop_i2c(); //Make sure twi off

  	// Startup Sensors
  	startupLightSensor();
	Serial.println("Light Sensor Started");
	Serial.flush();

	startUpTempSensor();
	Serial.println("Temp  Sensor Started");
	Serial.flush();
	

	//Set data in dataHandler
	device_operator->_dataHandler.setLightData();
 	device_operator->_dataHandler.setTempData();

 	//Stop State
 	this->stop(device_operator);
}

void DataCollectState::stop(DeviceOperation* device_operator){
	device_operator->_dataHandler.serial_printLightData();
 	device_operator->_dataHandler.serial_printTempData();
 	stop_i2c();
}



/*
	
	PowerMonitor State

*/

char* PowerMonitorState::getName(){
	return "----- PowerMonitor State -----";
}

void PowerMonitorState::start(DeviceOperation* device_operator){

	device_operator->_powerMonitor.setPowerData(); // Collect data from ADC's


	// Get current and voltage data
	uint8_t num_voltages = device_operator->_powerMonitor.num_voltages;
	uint8_t num_currents = device_operator->_powerMonitor.num_currents;

	uint16_t int_voltage_data[num_voltages]; // Integer Voltage Data
	uint16_t int_current_data[num_currents]; // Integer Current Data

	device_operator->_powerMonitor.getVoltage(int_voltage_data, num_voltages);
	device_operator->_powerMonitor.getCurrent(int_current_data, num_currents);

	// Pass data to dataHandler
	device_operator->_dataHandler.setVoltageData(int_voltage_data, num_voltages);
	device_operator->_dataHandler.setCurrentData(int_current_data, num_currents);

	this->stop(device_operator);
}

void PowerMonitorState::stop(DeviceOperation* device_operator){
	device_operator->_powerMonitor.serial_printPowerData();
}


/*
	
	Package State
	
	Move collected data to dataCache
*/

char* DataPackageState::getName(){
	return "----- Data Package State -----";
}
void DataPackageState::start(DeviceOperation* device_operator){

	// Save sensor data to dataHandler
	device_operator->_dataHandler.saveData();


}
void DataPackageState::stop(DeviceOperation* device_operator){

}


/*
	Operate SIM900 State

	Command Sequences are stored in SIM900Interface and
	sent in SIM900Operation. 
*/

char* OperateSIM900State::getName(){
	return "----- Operate SIM900 State -----";
}

void OperateSIM900State::start(DeviceOperation* device_operator){

	//Turn on Seeed GPRS Module
	digitalWrite(9,HIGH);
	delay(2000);
	digitalWrite(9,LOW);

	sim900_interface.initalize();
	//sim900_interface.initGPRS();

	sim900_interface.initSMSTransmission();
	sim900_interface.sendSMS(&device_operator->_dataHandler);
	//sim900_interface.readMessages();
	//delay(1000);

	this->stop(device_operator);
}

void OperateSIM900State::stop(DeviceOperation* device_operator){
	Serial.println("Shutting down GSM modem");
	Serial.flush();
	
	//Turn off Seeed GPRS Module
	digitalWrite(9,HIGH);
	delay(1000);
	digitalWrite(9,LOW);

}









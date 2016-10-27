#include "i2c_utility.h"
#include "serial_utility.h"
#include "DeviceOperation.h"
#include "SIM900Interface.h"
#include "StateClasses.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>


#define TEN_SECONDS  1000 * 1
#define FIVE_MINUTES 1000 * 60 * 5 //1s * 60s/min * 5 min 
#define RESET_PIN 3

using namespace std;

int main(){
	init();

	//Setup Reset Circuit
	digitalWrite(RESET_PIN, HIGH);
	pinMode(RESET_PIN, OUTPUT);

	//Setup Serial Port
	Serial.begin(9600);	
	Serial.println("\nSTARTUP");
	Serial.flush();

	//States
	BaseState          base_state;
	DataCollectState   data_state;
	PowerMonitorState  power_state;  
	DataPackageState   data_package_state;
	OperateSIM900State sim900_state;

	//Create Operation State Machine
	DeviceOperation device_operator;


	//Control Loop
	while(true){	

		/* Base State */
			device_operator.changeState(&base_state);
			device_operator.start();

			unsigned long start_time = millis();
			/* Standby */
			while( (millis() - start_time) < TEN_SECONDS){
				// Do Nothing
			}

		/* Data Handling */

			// Get sensor data
			device_operator.changeState(&data_state);
			device_operator.start();

			// Get power data
			device_operator.changeState(&power_state);
			device_operator.start();

			// Package data into dataCache
			device_operator.changeState(&data_package_state);
			device_operator.start();

			// Increment number of samples
			device_operator.num_samples += 1;


			// Reset control loop if we are not ready to send
			if(device_operator.num_samples < 5)
				continue;

		/* Data Transmission */			
			device_operator.changeState(&sim900_state);
			device_operator.start();


		//Reset device ;)
		digitalWrite(RESET_PIN, LOW);

		
	}



}








#ifndef   DEVICE_OPERATION_H
#define   DEVICE_OPERATION_H

#include "DeviceState.h"
#include "DataHandler.h"
#include "StateClasses.h"
#include "SoftwareSerial.h"
#include "PowerMonitor.h"

#include "SIM900Interface.h"
#include "StateClasses.h"
#include "Arduino.h"

/* Forward Declare SIM900Interface */
class SIM900Interface;


class DeviceOperation{

friend class DeviceState; 

public:
	DeviceOperation();
	void start();
	void stop();
	void changeState(DeviceState*);
	
	DataHandler  _dataHandler;
	PowerMonitor _powerMonitor;

	uint8_t num_samples = 0; // Monitor # of data samples taken

private:
	DeviceState * _currentState;
};


/*
	  SIM900Operation
*/
class SIM900Operation:DeviceOperation{

friend class SIM900Interface;

public:
	SIM900Operation();
	void sendAndRecieve(char const * cmdText, SoftwareSerial*);
	void rx(SIM900Operation* device_operator, SoftwareSerial*);
	void tx(SIM900Operation* device_operator,char const* cmdText, SoftwareSerial*);
	
private:
	SoftwareSerial* gsmInterface;	// Serial port for SIM900 module communication

	/* Flags */
	uint8_t   sms_read_flag = 0;
	int 	  tx_buffer_len = 0;
	char*     tx_buffer[255] = {0};
	uintptr_t tx_buffer_address = 0;

	char*     rx_buffer;
	int       rx_buffer_len     = -1;
	//int* data_ptr = (int*)&rxBuffer;


};


#endif
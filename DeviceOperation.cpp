#include "DeviceOperation.h"
#include "StateClasses.h"


DeviceOperation::DeviceOperation(){

}

void DeviceOperation::start(){
	Serial.flush();
	Serial.println("");
	Serial.println(_currentState->getName());
	Serial.flush();

	_currentState->start(this);
}

void DeviceOperation::stop(){
//	Serial.println("");
//	Serial.print("STOPPING: "); Serial.print(_currentState->getName());
//	Serial.flush();

	_currentState->stop(this);
}

void DeviceOperation::changeState(DeviceState* s){
	_currentState = s;
}




void SIM900Operation::sendAndRecieve(char const * cmdText, SoftwareSerial* SIM900_SerialPort){
	Serial.println("sendAndRecieve");

	//Serial.println(cmdText);
	//Serial.flush();

	//Load Tx Buffer
	memcpy(tx_buffer,cmdText,strlen(cmdText));
	tx_buffer_len = strlen(cmdText);

	for(uint8_t i = 0; i < tx_buffer_len; i++){
		Serial.print(tx_buffer[i]);
	}
	Serial.flush();
	//Serial.println(tx_buffer);

	memcpy(tx_buffer, cmdText, strlen(cmdText));
	tx_buffer_len = strlen(cmdText);


	tx(this, cmdText, SIM900_SerialPort);
	delay(1000);
	//rx(this, SIM900_SerialPort);

	//rx_state.start(this);
	
}


void SIM900Operation::tx(SIM900Operation* device_operator, char const* cmdText , SoftwareSerial* SIM900_SerialPort){
	Serial.println("\nTxState Start");

	//Send Tx Buffer 
	//for(int i = 0; i < tx_buffer_len; i++){
		SIM900_SerialPort->print(cmdText);
//	}
	SIM900_SerialPort->println();
	SIM900_SerialPort->flush();


	//Wait for Response
	//delay(2000);

}
void SIM900Operation::rx(SIM900Operation* device_operator, SoftwareSerial* SIM900_SerialPort){
	Serial.println("RxState Stop");


	delay(1000);
	int timeout = 2000; //2 second timeout
	int start_time = millis();

	while(!SIM900_SerialPort->available()){
		if( (millis() - start_time) >= timeout){Serial.println("timeout");break;}
		delay(5); //Let rx buffer fill
	}

	uint8_t i = 0;
	uint8_t numBytes = SIM900_SerialPort->available();
	char response[numBytes];
	while(numBytes--){ //Decrement to zero
		
		char c = SIM900_SerialPort->read(); // Read byte
		if(c == '\n' || c == '\r' || c == 255){ c=' '; } // Skip newline, carriage, and blank value
		response[i++] = c;


	} 
	Serial.println("RESPONSE");
	Serial.println(response);
	Serial.flush();

	/*

	// Get valid data
	// Valid data is between AT and OK
	int start_idx = 0;
	int   end_idx = 0;
	for(int j = 0; j < i; j++){
		if(response[j] == 'A' && response[j+1] == 'T')
			start_idx = j;
		if(response[j] == 'O' && response[j+1] == 'K')
			end_idx = j;
	}

	//Transfer valid data to array and to SIM900Operator
	int  valid_len   = end_idx - start_idx;
	char valid_data[valid_len]; 
	for(int j = 0; j < valid_len; j++){
		device_operator->rx_buffer[j] = response[start_idx+j];
		valid_data[j] 				  = response[start_idx+j];
	}

	// Only print if we got an OK
	if(end_idx != 0){ 
		device_operator->rx_buffer_len = valid_len;
		
		Serial.println(valid_data); 
		Serial.flush(); 
	}else{ // Disregard the data
		device_operator->rx_buffer_len =  -1;  // Set as invalid data
		device_operator->rx_buffer[0]  = '\0';
		Serial.println("-");

	}
	
	*/

}

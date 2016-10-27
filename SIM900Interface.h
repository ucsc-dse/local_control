#ifndef   SIM_INTERFACE_H
#define   SIM_INTERFACE_H

#include "SoftwareSerial.h"
//#include "DeviceOperation.h"
	
class DataHandler;
class SIM900Operation;

class SIM900Interface{
	friend class SIM900Operation;
	public:	

	SIM900Interface();
	void initalize();
	void readMessages();
	void initGPRS();
	void initSMSTransmission();	
	void sendSMS(DataHandler* dataHandler);

	private:
	SIM900Operation* sim900_operator;

	int  AT_Tokenizer(char* cmdSequence, char** parsedSequence);
	void AT_sendSequence(char* cmdSequence, char const * desc);


};

#endif
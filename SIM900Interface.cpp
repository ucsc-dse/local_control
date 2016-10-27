#include "SIM900Interface.h"
#include "DeviceOperation.h"
#include "serial_utility.h"



SIM900Interface::SIM900Interface(){

}

void SIM900Interface::initalize(){
	char* AT_initGSMModule = {"|AT+CFUN?|ATE0|AT+CGATT?|AT+CMEE=1|AT+CSQ|AT+CREG?|AT+CGREG?|AT+CPIN?"};	
	AT_sendSequence(AT_initGSMModule,"Initalizing GSM Module");


}

void SIM900Interface::readMessages(){

	// Set flag for operator
	sim900_operator->sms_read_flag = 1;

	char* AT_readText = {"AT+CMGL=\"ALL\""};
	AT_sendSequence(AT_readText, "Read Texts"); 	// Currently restricted from GPRS access 


	//Reset flag
	sim900_operator->sms_read_flag = 0;

}

void SIM900Interface::initGPRS(){
	char* AT_initGPRS_1	      = {"AT+CIPSTATUS|AT+CIPSHUT|AT+CIPMUX=0|AT+CIPMODE=1|AT+CGDCONT=1,\"IP\",\"wholesale\"|AT+CSTT=\"wholesale\"|AT+CIICR|AT+CGATT=1|AT+CGDATA=1|"};//|AT+CIFSR"};
	char* AT_initGPRS_2		  = {"AT+CEER|AT+CGACT=1,1|AT+CIICR|AT+CIPSTATUS|AT+CIFSR|AT+CIPSTART=\"TCP\",23.253.157.17,80|AT+CIPSTATUS|"};
		
	AT_sendSequence(AT_initGPRS_1, "Init GPRS_1"); 	// Currently restricted from GPRS access 
	AT_sendSequence(AT_initGPRS_2, "Init GPRS_2");	// Because MS is an "Illegal Device"


}

//18052703481 GMAIL
void SIM900Interface::initSMSTransmission(){
	char* AT_initDataTransmit = {"|AT+CMGF=1|AT+CSMP=17,167,0,16|AT+CMGS=\"+18052703481\"|"};
	AT_sendSequence(AT_initDataTransmit, "Initalize GSM Transmission");


		Serial.println("sendSMS");
		Serial.flush();


}

void SIM900Interface::sendSMS(DataHandler* dataHandler){
	SoftwareSerial SIM900_SerialPort(7,8);  // Create Virtual Serial Port
	SIM900_SerialPort.begin(19200);

	//Serial.println("sendSMS_START");

	//Send dataCache
	for(uint8_t i = 0; i < dataHandler->data_idx; i++){
		Serial.print(dataHandler->dataCache[i],HEX);
		Serial.flush();
		SIM900_SerialPort.print(dataHandler->dataCache[i],HEX);
	}

	// Trigger sms send
	SIM900_SerialPort.write(0x1a);
	SIM900_SerialPort.flush();


	SIM900_SerialPort.end();
	delay(5000);

}


// Returns the length of tokenized sequence
int SIM900Interface::AT_Tokenizer(char* cmdSequence, char** parsedSequence){
	//Serial.println("Tokenizer");
	char* savePrt;
	int i = 0;

	//Copy cmdSequence so it will not get modified by strtok_r
	//char cmdSequence_cp[255];
	//strcpy(cmdSequence_cp, cmdSequence);
	//Serial.println(cmdSequence_cp);
	Serial.println(cmdSequence);
	Serial.flush();

	char* token = strtok_r(cmdSequence, "|", &savePrt);
	while(token != NULL){
		parsedSequence[i++] = token;
		token = strtok_r(NULL, "|", &savePrt);
	}

	return i;

}



void SIM900Interface::AT_sendSequence(char* cmdSequence, char const * desc){
	delay(1000);
	//clearTxRxBuffers(sPort);
	Serial.println(desc);
	Serial.flush();

	SoftwareSerial SIM900_SerialPort(7,8);  // Create Virtual Serial Port
	SIM900_SerialPort.begin(19200);

	char* parsedSequence[20]; //20 commands max

	Serial.println("beforeTok");

	int len = AT_Tokenizer(cmdSequence, parsedSequence);

	Serial.println(len);	
	for(int i = 0; i < len; i++){
		sim900_operator->sendAndRecieve(parsedSequence[i], &SIM900_SerialPort);

		// Delay for gprs connection?
		//if(desc == "Init GPRS_1" || desc == "Init GPRS_2")
		//	delay(2500);
	}

	SIM900_SerialPort.end();

	Serial.println("_____________________________");
	Serial.println("");
	Serial.flush();

}






	


#include "Arduino.h"
#include "serial_utility.h"
#include <SoftwareSerial.h>
#include <string.h>


using namespace std;

int CONTINUE_RX = 0;

void clearTxRxBuffers(SoftwareSerial * sPort){
	while(sPort->available()){sPort->read();}; 		   // Clear rx
	while(Serial.available()){Serial.read();}
	sPort->print(""); sPort->flush();				   // Clear tx
	Serial.print(""); Serial.flush();
}


void sendAndRecieve(SoftwareSerial * sPort, char const * cmdText){
	char dataBytes[64] = {0}; //Size of serial buffer is 64 bytes

	if(!CONTINUE_RX){
		Serial.print("-> "); Serial.println(cmdText);
		Serial.flush();
		sPort->println(cmdText);//Send
		delay(200);
	}

	int status = recieve(sPort, dataBytes);
	if      (status == 1)   				{				  Serial.println("__Serial Timeout__");}
	else if (status == 0)   				{CONTINUE_RX = 0; Serial.print("<- ");Serial.println(dataBytes);}						
	else if (status == 2)   				{CONTINUE_RX = 1; sendAndRecieve(sPort, "");} // Print more data					
	else if (status == 0 && CONTINUE_RX)	{Serial.print(dataBytes);}

	//Check for error if ERROR is found
	//if(-1 != find_text(String(dataBytes), "ERROR")){
		//Serial.println(dataBytes);
		//sendAndRecieve(sPort, "AT+CEER");
	//}

	delay(500);
	//Serial.flush();


}

int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; (i < haystack.length() - needle.length()); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

int recieve(SoftwareSerial * sPort, char * outgoingData){
	int i = 0;
	int timeout = 2000; //2 second timeout
	int start_time = millis();
	char c;

	//sPort gets junk. Need to filter
	while(!sPort->available()){
		if( (millis() - start_time) >= timeout){return 1;}
		delay(50); //Let rx buffer fill
	}

	byte numBytes = sPort->available();
	while(numBytes--){ //Decrement to zero
		
		c = sPort->read(); //Read byte
		if(c == '\n' || c == '\r'){c=' ';} //Skip newline and carriage
		outgoingData[i++] = c;

	} delay(5); //Allow rx buffer to fill is there is more data coming


	//if(sPort->available()){return 2;}; // There is still data to print

	return 0;
}


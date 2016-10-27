#include "Arduino.h"        // Some Arduino libraries are handy
#include <SoftwareSerial.h> // Used to establish connection with GSM Modem

void clearTxRxBuffers(SoftwareSerial * sPort);                     // Clear the port passed to it
void sendAndRecieve(SoftwareSerial * sPort, char const * cmdText); // Send AT command and call recieve()
int recieve(SoftwareSerial * sPort, char * outgoingData);		   // Capture serial response
int find_text(String needle, String haystack);                     // Parse the responses


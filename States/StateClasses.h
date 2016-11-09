#ifndef   STATECLASSES_H
#define   STATECLASSES_H

#include "DeviceState.h"

#include "i2c_utility.h"
#include "Arduino.h"
#include "serial_connector.h"
#include "SIM900Interface.h"
#include "SoftwareSerial.h"


/*  Forward Declare */
class DeviceOperation;
class SIM900Operation;
class SIM900Interface; 

using namespace std;


class BaseState: public DeviceState{
	public:
	char* getName();
	void start(DeviceOperation *);
    void stop (DeviceOperation *);
};


class DataCollectState: public DeviceState{
	public:
	char* getName();
	void start(DeviceOperation *);
    void stop (DeviceOperation *); 

};

class PowerMonitorState: public DeviceState{
	public:
	char* getName();
	void start(DeviceOperation *);
    void stop (DeviceOperation *); 

};

class DataPackageState: public DeviceState{
	public:
	char* getName();
	void start(DeviceOperation *);
    void stop (DeviceOperation *); 

};
class OperateSIM900State: public DeviceState{
	public:
	char* getName();
	void start(DeviceOperation *);
    void stop (DeviceOperation *); 

    SIM900Interface sim900_interface;

};


#endif


#ifndef   DEVICE_STATE_H
#define   DEVICE_STATE_H

class DeviceOperation;

class DeviceState{

public:
	virtual char* getName() = 0;
	virtual void start(DeviceOperation*)  = 0;
	virtual void stop (DeviceOperation*)  = 0;

};

#endif
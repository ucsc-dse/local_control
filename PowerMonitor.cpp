#include "PowerMonitor.h"


#define VIN_2_UNIT			     0       // Arduino Pin 2. Unit    Voltage
#define VIN_1_BATT     	         1       // Arduino Pin 1. Battery Voltage
#define VIN_0_SOLAR		         2 	     // Arduino Pin 0. Solar   Voltage
#define SOLAR_MONITOR_RESISTOR   10.0
#define UNIT_MONITOR_RESISTOR    1.0     // Measure drop over 1 Ohm to find current
#define VOLTAGE_CONV_FACT        2.0     // Input voltage is passed through voltage divider. Used to find supply voltage.
#define ADC_CONV_FACT            0.0049  // 5V/1024  Arduino ADC has 10 bit resolution


/*
	Scan ADC, get voltage over resistor, calculate current
*/
void PowerMonitor::setPowerData(){
	//Get Voltages
	unit_voltage    = (float)analogRead(VIN_2_UNIT)  * ADC_CONV_FACT * VOLTAGE_CONV_FACT;
	battery_voltage = (float)analogRead(VIN_1_BATT)  * ADC_CONV_FACT * VOLTAGE_CONV_FACT;
	solar_voltage   = (float)analogRead(VIN_0_SOLAR) * ADC_CONV_FACT * VOLTAGE_CONV_FACT;

	//Get Currents
	solar_current = (solar_voltage - battery_voltage) / SOLAR_MONITOR_RESISTOR;
	unit_current  = (battery_voltage - unit_voltage)  / UNIT_MONITOR_RESISTOR;

	//Package
	voltage_arr[v_unit]    = unit_voltage; 
	voltage_arr[v_solar]   = solar_voltage;
	voltage_arr[v_battery] = battery_voltage;

	current_arr[c_solar]   = solar_current;
	current_arr[c_unit]    = unit_current; 

}

void PowerMonitor::convertToInt(float* float_arr, int len, uint16_t* int_arr){
	
	for(int i = 0; i < len; i++){
		int_arr[i] = (uint16_t)(float_arr[i] * 1000);
	}
}

void PowerMonitor::getVoltage(uint16_t* fillArr, int arrLen){
	convertToInt(voltage_arr, arrLen, fillArr);
}

void PowerMonitor::getCurrent(uint16_t* fillArr, int arrLen){
	convertToInt(current_arr, arrLen, fillArr);
}

void PowerMonitor::serial_printPowerData(){
	Serial.println("");
	Serial.print("Unit    Voltage(V) : ");Serial.println(unit_voltage);
	Serial.print("Battery Voltage(V) : ");Serial.println(battery_voltage);
	Serial.print("Solar   Voltage(V) : ");Serial.println(solar_voltage);

	Serial.print("Solar Current(mA): ");Serial.println(solar_current*1000);
	Serial.print("Unit Current(mA): ");Serial.println(unit_current*1000);

	Serial.flush();
}
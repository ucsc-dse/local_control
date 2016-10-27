#include "Arduino.h"

class PowerMonitor{
	public:

	uint8_t       num_voltages = 3;
	uint8_t       num_currents = 2;

	void      setPowerData();      // Read power data from ADC's
	void convertToInt(float* float_arr, int len, uint16_t* int_arr);
	void      getVoltage(uint16_t* fillArr, int arrLen); //Fill array with voltage values
	void      getCurrent(uint16_t* fillArr, int arrLen);
	void      serial_printPowerData();

	private:
	float solar_voltage;
	float battery_voltage;
	float unit_voltage;
	float voltage_arr[3];

	float solar_current;
	float unit_current;
	float current_arr[2];
};

enum voltage_idx {v_solar = 0, v_battery = 1, v_unit = 2};
enum current_idx {c_solar = 0, c_unit = 1};
#include <inttypes.h>

using namespace std;

class DataHandler{
public:
	// Give DataHandler the sensor data
	void      setLightData(); // Communicate with light sensor and set lightData
	void 	  setTempData();  // Communicate with temp sensor and set tempData

	// Give DataHandler the power data
	void 	  setVoltageData(uint16_t* vData, int len); // Called in PowerMonitor State
	void      setCurrentData(uint16_t* cData, int len); // To give DataHandler power data

	// Return sensor data
	uint16_t* getLightData(); // Array of [CLEAR, RED, GREEN, BLUE] data
	uint8_t*  getTempData();  // Implied decimal point between elements 

	// Return Power Data
	uint16_t* getVoltageData(); 
	uint16_t* getCurrentData();

	// Data Handling
	void saveData();
	uint16_t getDataHeader();

	//Print methods
	void      serial_printLightData();
	void      serial_printTempData();
	
	uint8_t lightData_len = 4;
	uint8_t tempData_len  = 2;
	uint8_t voltageData_len = 0; // Set in setVoltageData
	uint8_t currentData_len = 0; // Set in setCurrentData

	uint16_t*  _lightData;
	uint8_t    _tempData[2];
	uint16_t*  _voltageData;
	uint16_t*  _currentData;

	uint8_t  data_idx = 0;
	uint16_t dataCache[255] = {0};
};

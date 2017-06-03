#ifndef SI2070_H
#define SI2070_H

#include <cstdint>

// The validity is how long since the last update. The default is to
// take readings only once every 15 minutes.
#define SI2070_VALIDITY		900

class SI2070 {
public:
	// Update returns false if an error occurred; check ErrorCode in
	// that case.
	bool Update(void);
	double Temperature(void); // degrees C
	double RelativeHumidity(void);

	// ErrorCode returns the last I²C error code.
	uint8_t	ErrorCode() { return errorCode; }
private:
	uint8_t addr = 0x40;
	uint8_t regTemp = 0xE3;
	uint8_t regHumid = 0xE5;
	uint8_t errorCode;

	uint16_t temp;
	uint16_t humid;

	bool takeMeasurement(uint8_t reg, uint16_t &target);
};

#endif // SI2070_H

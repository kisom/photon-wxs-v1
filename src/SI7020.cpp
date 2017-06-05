#include "SI7020.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"

// How many bytes should be requested using requestFrom?
static const uint8_t	readBytes = 2;

bool
SI2070::takeMeasurement(uint8_t reg, uint16_t &target)
{
	// NB: Update should clear the error code before beginning.
	// NB: callers should check Ready before continuing.

	// From the docs, begin() should be called only once. We'll only
	// enable it if the I²C bus hasn't been intialised already.
	if (!Wire.isEnabled()) {
		Wire.begin();
	}

	Wire.beginTransmission(addr);
	Wire.write(regTemp);
	errorCode = Wire.endTransmission();
	if (errorCode != 0) {
		return false;
	}

	uint16_t data[2]; // Use 16-bit to make sure the shift works properly.
	Wire.requestFrom(addr, readBytes); // Readings are 16-bit.
	data[0] = Wire.read() & 0x00FF;
	data[1] = Wire.read() & 0x00FF;
	target = (data[0] << 8) + data[1];
	return true;
}

bool
SI2070::Update()
{
	errorCode = 0;
	if (!takeMeasurement(regTemp, temp)) {
		return false;
	}

	if (!takeMeasurement(regHumid, humid)) {
		return false;
	}

	return true;
}

double
SI2070::Temperature()
{
	return (175.72 * static_cast<double>(temp) / 65536) - 46.85; // datasheet p23
}

double
SI2070::RelativeHumidity()
{
	return (125 * static_cast<double>(humid) / 65536) - 6; // datasheet p22
}

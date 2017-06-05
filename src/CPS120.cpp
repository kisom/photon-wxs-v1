#include "CPS120.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"

// How many bytes should be requested using requestFrom? 3: one junk byte,
// and the 16-bit reading.
static const uint8_t readBytes = 3;

bool
CPS120::Update()
{
	// From the docs, begin() should be called only once. We'll only
	// enable it if the I²C bus hasn't been intialised already.
	if (!Wire.isEnabled()) {
		Wire.begin();
	}

	Wire.beginTransmission(addr);
	Wire.write(0);
	errorCode = Wire.endTransmission();
	if (errorCode != 0) {
		return false;
	}

	// first byte is junk, so ignore it.
	uint16_t data[2];
	Wire.requestFrom(addr, readBytes); // Readings are 14-bit.
	Wire.read();
	data[0] = Wire.read() & 0x00FF;
	data[1] = Wire.read() & 0x00FF;
	pressure = (data[0] << 8) + data[1];
	pressure &= 0x4FFF; // Mask out the 14 LSBs for the reading.
	return true;
}

double
CPS120::Pressure()
{
	return (static_cast<double>(pressure) / 16384 * 60); // datasheet p16
}

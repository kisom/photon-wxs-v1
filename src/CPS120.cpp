#include "CPS120.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"

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
	Wire.requestFrom(addr, 3); // Readings are 16-bit.
	Wire.read();
	data[0] = Wire.read() & 0x00FF;
	data[1] = Wire.read() & 0x00FF;
	pressure = (data[0] << 8) + data[1];
	pressure &= 0x4FFF;
	return true;
}

double
CPS120::Pressure()
{
	return (static_cast<double>(pressure) / 16384 * 60); // datasheet p16
}

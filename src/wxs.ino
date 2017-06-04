/*
 * Project wxs
 * Description: basic weather station
 * Author: K. Isom
 * Date: 2017-06-03
 */

#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include "wxs.h"
#include "ap.h"
#include "SI7020.h"
#include "CPS120.h"

#define TOPIC	"kisom/wxs/v1/messages"
#define MSG(m)	Particle.publish(TOPIC, (m), 3600, PRIVATE)

Reading	weather;
SI2070 tempSensor;
CPS120 barometer;

// This is janky, but done for reasons of low power, and because I don't
// want to deal with OAuth.

// The encoded buffers will need to contain our three (double) measurements,
// the (int) timestamp, and the (int) last error code.
constexpr size_t	encodedSize = 2 * sizeof(weather);

void
hexDigit(uint8_t c, uint8_t *out)
{
        uint8_t tmp = (c >> 4) & 0xF;
        if (tmp < 10) {
                tmp += 0x30;
        }
        else {
		// += 0x37 is equivalent to:
		// tmp -= 10; tmp += 0x41;
                tmp += 0x37;
        }
        *out = tmp;

        tmp = c & 0x0F;
        if (tmp < 10) {
                tmp += 0x30;
        }
        else {
                tmp += 0x37;
        }
        *(out + 1) = tmp;
}

void
publishMeasurements(void)
{
	// Warn if our sensor readings get too big fer their britches --- or
	// to be published as a Particle message.
	if (encodedSize > 255) {
		Particle.publish("kisom/wxs/v1/warnings", "encodedSize > 255 (c.f. publishMeasurements)",
			3600, PRIVATE);
		return;
	}
	uint8_t	 encoded[encodedSize + 1] = {0};
	uint8_t *wxBytes = reinterpret_cast<uint8_t *>(&weather);

	for (auto i = 0; i < sizeof(weather); i++) {
		hexDigit(*(wxBytes+i), encoded + (i*2));
	}

	Particle.publish("kisom/wxs/v1/measurements",
		reinterpret_cast<const char *>(encoded), 900, PUBLIC);
}

int
takeMeasurements(String unused)
{
	weather.Updated = Time.now();
	if (!tempSensor.Update()) {
		MSG("failed to update temperature");
		weather.Error = tempSensor.ErrorCode();
		return -1;
	}
	weather.Temperature = tempSensor.Temperature();
	weather.Humidity = tempSensor.RelativeHumidity();

	if (!barometer.Update()) {
		MSG("failed to update pressure");
		weather.Error = barometer.ErrorCode();
		return -1;
	}
	weather.AirPressure = barometer.Pressure();

	publishMeasurements();
	return 0;
}

// setup() runs once, when the device is first turned on.
void
setup()
{
	Particle.variable("temperature", weather.Temperature);
	Particle.variable("humidity", weather.Humidity);
	Particle.variable("airPressure", weather.AirPressure);
	Particle.variable("updatedAt", weather.Updated);
	Particle.variable("sensorError", weather.Error);

	// Allow manual updates.
	Particle.function("update", takeMeasurements);

	while (!WiFi.ready()) {
		delay(250);
	}
}

// loop() runs over and over again, as quickly as it can execute.
void
loop()
{
	takeMeasurements("");
	Particle.publish("kisom/wxs/v1/wakeup", NULL, 30, PUBLIC);

	// Stay awake for one minute afterwards to give the system time to
	// flush pending messages. This also gives us time to kick off a
	// firmware update.
	delay(50000);
	Particle.publish("kisom/wxs/v1/sleep", NULL, 10000);

	// Go into deep sleep for 14 minutes --- this gives us roughly 15
	// minutes between measurements.
	System.sleep(SLEEP_MODE_DEEP, 840);
}

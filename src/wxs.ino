/*
 * Project wxs
 * Description: basic weather station
 * Author: K. Isom
 * Date: 2017-06-03
 */

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "wxs.h"
#include "SI7020.h"
#include "CPS120.h"

#define TOPIC	"kisom/wxs/v1/messages"
#define MSG(m)	Particle.publish(TOPIC, (m), 3600, PRIVATE)

Reading	weather;
SI2070 tempSensor;
CPS120 barometer;

// This is janky, but done for reasons of low power, and because I don't
// want to deal with OAuth: instead, I'll just do this via a webhook.

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

	for (size_t i = 0; i < sizeof(weather); i++) {
		hexDigit(*(wxBytes+i), encoded + (i*2));
	}

	Particle.publish("kisom/wxs/v1/measurements",
		reinterpret_cast<const char *>(encoded), 900, PUBLIC);
}

int
takeMeasurements(String unused)
{
	char err[32];
	weather.Updated = Time.now();
	if (!tempSensor.Update()) {
		sprintf(err, "failed to update temperature: %d",
			tempSensor.ErrorCode());
		MSG(err);
		weather.Error = tempSensor.ErrorCode();
		return -1;
	}
	weather.Temperature = tempSensor.Temperature();
	weather.Humidity = tempSensor.RelativeHumidity();

	if (!barometer.Update()) {
		sprintf(err, "failed to update pressure: %d",
			barometer.ErrorCode());
		MSG(err);
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
	// There's not much to do in the setup loop. The first
	// iteration had cloud variables and functions, but
	// those turned out to be not very useful. The variables
	// were only available when the device is online, and for
	// reasons of power consumption, I've tried to minimise the
	// time the device is awake. Now, the idea is webhook +
	// published message. The future might be MQTT.
}

// loop() runs over and over again, as quickly as it can execute.
void
loop()
{
	Particle.publish("kisom/wxs/v1/wakeup", NULL, 30, PUBLIC);
	takeMeasurements("");

	// Stay awake for one minute afterwards to give the system time to
	// flush pending messages. This also gives us time to kick off a
	// firmware update.
	delay(50000);
	Particle.publish("kisom/wxs/v1/sleep", NULL, 10000);

	// Go into deep sleep for 14 minutes --- this gives us roughly 15
	// minutes between measurements.
	System.sleep(SLEEP_MODE_DEEP, 840);
}

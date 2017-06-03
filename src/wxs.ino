/*
 * Project wxs
 * Description: basic weather station
 * Author: K. Isom
 * Date: 2017-06-03
 */

#include <cstdio>
#include <cstdint>

#include "wxs.h"
#include "SI7020.h"
#include "CPS120.h"

#define TOPIC	"kisom/wxs/v1/messages"
#define MSG(m)	Particle.publish(TOPIC, (m), 3600, PRIVATE)

Reading	weather;
SI2070 tempSensor;
CPS120 barometer;

int
takeMeasurements(String unused)
{
	MSG("taking sensor measurements");
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

	MSG("sensor update complete");
	return 0;
}

// setup() runs once, when the device is first turned on.
void setup() {
	Particle.variable("temperature", weather.Temperature);
	Particle.variable("humidity", weather.Humidity);
	Particle.variable("airPressure", weather.AirPressure);
	Particle.variable("updatedAt", weather.Updated);
	Particle.variable("sensorError", weather.Error);

	// Allow manual updates.
	Particle.function("update", takeMeasurements);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
	takeMeasurements("");
	delay(900000);
}

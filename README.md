# wxs

This is a basic weather station built around a Particle Photon. It uses
the following sensors connected to an I²C module:

+ Silicon Labs Si7020-A20 temperature and humidity sensor
+ Consensic CPS120 barometer
+ Freescale Xtrinsic MAG310 3-axis magnetometer and digital compass
+ Silicon Labs Si1132 ambient light sensor

Currently, only the Si7020 and CPS120 are active.

Power is provided by a Sparkfun Sunny Buddy that uses a 2.5W solar panel to
charge a 3.7V 2Ah LiPo battery.

## References

+ [Si7020](https://shop.controleverything.com/products/humidity-and-temperature-sensor-4-rh-0-4-c?variant=25687601163)
+ [CPS120](https://shop.controleverything.com/products/digital-barometer-altimeter-50-to-1200-mbar?variant=25687620043)
+ [MAG310](https://shop.controleverything.com/products/3-axis-digital-magnetometer-electronic-compass?variant=25687552331)
+ [Si1132](https://shop.controleverything.com/products/uv-index-ambient-light-sensor?variant=25687570507)
+ [I²C shield](https://shop.controleverything.com/products/i2c-breakout-for-particle-photon?variant=25687320587)
+ [Sunny Buddy](https://www.sparkfun.com/products/12885)
+ [battery](https://www.sparkfun.com/products/8483)
+ [solar cell](https://www.sparkfun.com/products/retired/7840)

## Future

+ Upgrade to the following:

  + [3.5W solar panel](https://www.sparkfun.com/products/13782)
  + [SparkFun weather shield](https://www.sparkfun.com/products/13674)
  + [Weather meters](https://www.sparkfun.com/products/8942)

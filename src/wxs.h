#ifndef WXS_H
#define WXS_H

// Reading contains the last set of sensor measurements.
typedef struct {
	double 	Temperature;	// eight bytes
	double	Humidity;	// eight bytes
	double	AirPressure;	// eight bytes
	int	Updated;	// four bytes
	int	Error;		// four bytes
	float	Voltage;	// four bytes
	float	Charge;		// four bytes
} Reading; 			// 40 bytes total; 80 bytes hex-encoded.

#endif // WXS_H

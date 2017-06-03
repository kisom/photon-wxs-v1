#ifndef WXS_H
#define WXS_H

typedef struct {
	double 	Temperature;
	double	Humidity;
	double	AirPressure;
	int	Updated;
	int	Error;
} Reading;

#endif // WXS_H

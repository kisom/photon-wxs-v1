#ifndef CPS120_H
#define CPS120_H

#include <cstdint>

class CPS120 {
public:
	bool	Update(void);
	uint8_t	ErrorCode(void) { return errorCode; }
	double	Pressure(void);
private:
	uint8_t	addr = 0x28;
	uint16_t pressure;
	uint8_t errorCode;
};

#endif

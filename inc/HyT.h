#ifndef _HyT_H_
#define _HyT_H_

#include <stdint.h>

extern uint32_t HyT_lastTime;

extern float Humidity;
extern float Temperature;

void poll_HyT(void);

uint8_t HyT_checksum(void);

char* HyT_getTemp(void);
char* HyT_getHum(void);
#endif


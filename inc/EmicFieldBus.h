#ifndef _EMICfieldBus_H_
#define _EMICfieldBus_H_

/********************************[macros]****************************************/
#define		FrameLf		'\n'

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "streamIn.h"
#include "streamOut.h"

void emicFBp(char* format, ...);

void poll_EMICfb (void);

char		getBusStreamIn(void);
uint16_t	countBusStreamIn(void);
void		setBusStreamOut(char);
uint16_t	countBusStreamOut(void);

extern const streamIn_t		streamIn_fieldBus;
extern const streamOut_t	streamOut_fieldBus;

#endif


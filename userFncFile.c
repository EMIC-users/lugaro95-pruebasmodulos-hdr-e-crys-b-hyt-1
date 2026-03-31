#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/userFncFile.h"
#include "inc/systemTimer.h"
#include "inc/led_Led1.h"
#include "inc/led_Led2.h"
#include "inc/led_Led3.h"
#include "inc/timer_api1.h"
#include "inc/Transport.h"
#include "inc/EmicFieldBus.h"
#include "inc/dht_comm.h"
#include "inc/HyT.h"

void SystemConfig()
{
    My_ID = 2;
}


void onReset()
{
    LEDs_Led1_blink(125, 250, 3);
    LEDs_Led2_blink(125, 250, 3);
    LEDs_Led3_blink(125, 250, 3);
    setTime1(2000, 'A');
}


void etOut1()
{
    emicFBp("Temp:\t$s", HyT_getTemp());
    emicFBp("Hum:\t$s", HyT_getHum());
    LEDs_Led1_blink(125, 250, 1);
}


void emicFBe(char* TAG, const streamIn_t* const Message)
{
    emicFBp("HyT:\tOK");
    LEDs_Led3_blink(125, 250, 1);
}




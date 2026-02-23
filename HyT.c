#include "inc/HyT.h"
#include "inc/dht_comm.h"
#include "inc/systemTimer.h"
#include <stdio.h>

uint32_t HyT_lastTime = 0;
float Humidity = 0;
float Temperature = 0;

void poll_HyT(void)
{
    if(getSystemMilis() - HyT_lastTime > 2000)     //Sensor measure takes at least 2 seconds
    {
        dht_start(18);   //dht22 needs 18ms of initial low time
        HyT_lastTime = getSystemMilis();
    }
    if(dht_dataReceived)
    {
        dht_dataReceived = 0;
        if(HyT_checksum())
        {
           Temperature = ((uint16_t)(dht_shift.bytes.byte3) << 8 | dht_shift.bytes.byte2)*0.10;
           Humidity = ((uint16_t)dht_shift.bytes.byte5 << 8 | dht_shift.bytes.byte4)*0.10;
        }
        else
        {
            dht_shift.entero = 0;
        }
    }
}
uint8_t HyT_checksum(void)
{
    uint32_t aux = dht_shift.bytes.byte2 + dht_shift.bytes.byte3 + dht_shift.bytes.byte4 + dht_shift.bytes.byte5;
    if(dht_shift.bytes.byte1 == (uint8_t)(aux & 0xFF))
    {
        return 1;
    }
    return 0;
}

char* HyT_getTemp(void)
{
        static char strTemp[6];
        sprintf(strTemp, "%1.1f",(double)Temperature);
        return strTemp;
}

char* HyT_getHum(void)
{
    static char strHum[6];
    sprintf(strHum, "%1.1f",(double)Humidity);
    return strHum;
}


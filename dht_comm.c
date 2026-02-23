#include "inc/dht_comm.h"
#include "inc/gpio.h"
#include "inc/pins.h"
#include "inc/system.h"
#include "inc/systemTimer.h"

union W64Bits dht_shift;

int32_t dht_bitStart = 0;
int32_t dht_bitStop = 0;
dht_state_t Curr_State;
uint8_t dht_dataReceived = 0;
int8_t dht_receivedBit;
uint8_t dht_firstInit = 0;
uint16_t lowPulseDuration;
uint32_t dht_firstInitTime = 0;
uint32_t dht_initTime = 0;
uint32_t dht_receivingInitTime = 0;

void dht_init(void)
{
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPINR0bits.INT1R = RPIN_AM2302_MISO;
    IFS1bits.INT1IF = 0;
    IPC5bits.INT1IP = 6;
    INTCON2bits.INT1EP = 1;             //0 positive edge, 1 negative edge
    IEC1bits.INT1IE = 0;
    __builtin_enable_interrupts();
    __builtin_write_OSCCONL(OSCCON | 0x40);

    T2CONbits.TON = 1;
    T2CONbits.T32 = 1;
}

void dht_start(uint32_t low_time)
{
    if(dht_firstInit)
    {
        dht_firstInitTime = getSystemMilis();
        setPullUpAM2302_MISO;
        dht_firstInit = 0;
    }
    if (getSystemMilis() - dht_firstInitTime < 1000)
    {
        return;
    }
    lowPulseDuration = low_time;
    Curr_State = DHT_STATE_INIT;

}

void dht_poll(void)
{
    switch (Curr_State)
    {
         case DHT_STATE_IDLE:
        {
            break;
        }

        case DHT_STATE_INIT:
        {
            HAL_GPIO_PinCfg(AM2302_MISO,GPIO_OUTPUT);
            HAL_GPIO_PinSet(AM2302_MISO,GPIO_LOW);
            dht_initTime = getSystemMilis();
            Curr_State = DHT_STATE_SENDING;
            break;
        }

        case DHT_STATE_SENDING:
        {
            if(getSystemMilis() - dht_initTime >= lowPulseDuration)           //if start pulse ended
            {
                Curr_State = DHT_STATE_IDLE;
                HAL_GPIO_PinCfg(AM2302_MISO,GPIO_INPUT);
                IFS1bits.INT1IF = 0;
		        IEC1bits.INT1IE = 1;
                dht_receivedBit = -2;
            }
            break;
        }

        case DHT_STATE_RECEIVING:
        {
            if(getSystemMilis() - dht_receivingInitTime > DHT_MAX_TIME)     //if receiving state last longer than 5ms an error occurred
            {
                Curr_State = DHT_STATE_ERROR;
            }

            if(dht_receivedBit == DHT_BIT_QTY)  // if received all data
            {
                dht_dataReceived = 1;
                Curr_State = DHT_STATE_IDLE;
                IEC1bits.INT1IE = 0;
            }
            break;
        }

        case DHT_STATE_ERROR:
        {
            IEC1bits.INT1IE = 0;
            dht_shift.entero = 0;
            break;
        }

    }

}

void __attribute__((interrupt(auto_psv))) _INT1Interrupt (void)
{
    dht_bitStop = dht_clock;
	IFS1bits.INT1IF = 0;
    if(dht_receivedBit == -2)
    {
        Curr_State = DHT_STATE_RECEIVING;
        dht_receivingInitTime = getSystemMilis();
        dht_receivedBit ++;
        return;
    }
    if(dht_receivedBit == -1)
    {
        dht_bitStart = dht_bitStop;
        dht_receivedBit ++;
        return;
    }
    if(dht_diffTime(dht_bitStart,dht_bitStop) > 100)
    {
        dht_shift.entero = (dht_shift.entero << 1) | 1;
    }
    else
    {
        dht_shift.entero = (dht_shift.entero << 1) | 0;
    }
    dht_receivedBit ++;
    dht_bitStart = dht_bitStop;
}

int32_t dht_diffTime(int32_t lastTime, int32_t currTime)
{
    if(currTime > lastTime)
	{
		int32_t rv =(currTime - lastTime)/(FCY/1000000);
		return rv;
	}
	else
	{
		int32_t rv = (lastTime + (0xffffffff - currTime))/(FCY/1000000);
		return rv;
	}
}


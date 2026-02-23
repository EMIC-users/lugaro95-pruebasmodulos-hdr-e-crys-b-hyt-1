#ifndef _DHT_COMM_H
#define _DHT_COMM_H

#include <stdint.h>

#define DHT_MAX_TIME 5  //max message time in ms
#define DHT_BIT_QTY 40 // message bit quantity
#define dht_clock (uint32_t)TMR3 << 16 | TMR2  //timer2 32bit clock 1:1

typedef enum
{
	DHT_STATE_IDLE,
    DHT_STATE_INIT,
    DHT_STATE_SENDING,
    DHT_STATE_RECEIVING,
    DHT_STATE_ERROR,
}dht_state_t;

union W64Bits {
	uint64_t entero;
	struct {
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t byte4;
        uint8_t byte5;
		uint8_t byte6;
		uint8_t byte7;
		uint8_t byte8;
	} bytes;
};

extern union W64Bits dht_shift;

extern int32_t dht_bitStart;
extern int32_t dht_bitStop;
extern dht_state_t Curr_State;
extern uint8_t dht_dataReceived;
extern int8_t dht_receivedBit;
extern uint8_t dht_firstInit;
extern uint16_t lowPulseDuration;
extern uint32_t dht_firstInitTime;
extern uint32_t dht_initTime;
extern uint32_t dht_receivingInitTime;

int32_t dht_diffTime(int32_t lastTime, int32_t currTime);

void dht_start(uint32_t low_time);

void dht_init(void);

void dht_poll(void);

#endif


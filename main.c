#include <xc.h>
#include "inc/systemConfig.h"

#include "inc/systemTimer.h"
#include "inc/led_Led1.h"
#include "inc/led_Led2.h"
#include "inc/led_Led3.h"
#include "inc/timer_api1.h"
#include "inc/Transport.h"
#include "inc/EmicFieldBus.h"
#include "inc/dht_comm.h"
#include "inc/HyT.h"
#include "inc/system.h"
#include "inc/userFncFile.h"

#include "system.c"

int main(void)
{
	initSystem();
	systemTimeInit();
	LEDs_Led1_init();
	LEDs_Led2_init();
	LEDs_Led3_init();
	init_EMICfb();
	dht_init();
	onReset();
	do
	{
		LEDs_Led1_poll();
		LEDs_Led2_poll();
		LEDs_Led3_poll();
		timer1_Poll();
		poll_fieldBusTransport();
		poll_EMICfb();
		dht_poll();
		poll_HyT();
	}
	while(1);
}


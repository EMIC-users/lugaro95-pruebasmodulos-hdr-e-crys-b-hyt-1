#include "inc/EmicFieldBus.h"
#include "inc/stream.h"
#include "inc/Transport.h"
#include "inc/userFncFile.h"

const streamIn_t	streamIn_fieldBus = {getBusStreamIn,countBusStreamIn};
const streamOut_t	streamOut_fieldBus = {setBusStreamOut,countBusStreamOut};

void emicFBp(char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	sendDataToStream(&streamOut_fieldBus,format,arg);

	streamOpenWriteFrame(&fieldBusOutStream);
	va_end(arg);
}


void poll_EMICfb(){
	if (fieldBusInStream.frame_count > 0)
	{
		streamOpenReadFrame(&fieldBusInStream);
		char tag[20];
		char d;
		uint8_t i = 0;

		while (fieldBusInStream.data_count_sal != 0)
		{
			d = streamPop(&fieldBusInStream);
			if (d == '\t'|| i >= 20)
			{
				break;
			}
			tag[i++] = d;
		}
		tag[i] = 0;

		emicFBe(tag, &streamIn_fieldBus);
		while (fieldBusInStream.data_count_sal != 0)
		{
			d = streamPop(&fieldBusInStream);
		}
	}
}

char	getBusStreamIn(void)
{
	return streamPop(&fieldBusInStream);
}
uint16_t countBusStreamIn(void)
{
	return fieldBusInStream.data_count_sal;
}

void	setBusStreamOut(char data)
{
	streamPush(&fieldBusOutStream,data);
}
uint16_t countBusStreamOut(void)
{
	return fieldBusOutStream.data_count_sal;
}


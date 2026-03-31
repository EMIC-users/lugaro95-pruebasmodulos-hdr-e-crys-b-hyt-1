#include "inc/Transport.h"
#include "inc/gpio.h"
#include "inc/UART1.h"
#include "inc/systemTimer.h"

unsigned char fieldBusInDataBuffer[BufferSize];
unsigned char fieldBusOutDataBuffer[BufferSize];

uint16_t fieldBusInFrameBuffer[FrameQty];
uint16_t fieldBusOutFrameBuffer[FrameQty];


stream_t fieldBusInStream =
{
	.data_idx_entr = 0,
	.data_idx_sal = 0,
	.data_idx_sal_aux = 0,
	.data_count_entr = 0,
	.data_count_sal = 0,
	.data_count_sal_aux = 0,
	.frame_idx_entr = 0,
	.frame_idx_sal = 0,
	.frame_count = 0,
	.frame_fifo 	= fieldBusInFrameBuffer,
	.data_fifo 	= fieldBusInDataBuffer,
	.frame_indentifier = FrameLf,
	.data_idx_mask = BufferSize-1,
	.frame_idx_mask = FrameQty-1
};


stream_t fieldBusOutStream =
{
	.data_idx_entr = 0,
	.data_idx_sal = 0,
	.data_idx_sal_aux = 0,
	.data_count_entr = 0,
	.data_count_sal = 0,
	.data_count_sal_aux = 0,
	.frame_idx_entr = 0,
	.frame_idx_sal = 0,
	.frame_count = 0,
	.frame_fifo 	= fieldBusOutFrameBuffer,
	.data_fifo 	= fieldBusOutDataBuffer,
	.frame_indentifier = FrameLf,
	.data_idx_mask = BufferSize-1,
	.frame_idx_mask = FrameQty-1
};

uint16_t Transport_frame_count = 0;

uint8_t received_ID = 0;

uint8_t My_ID;

// UART RX callback: pushes received bytes to FIFO and detects frames
void ISR_UART1_CALLBACK(char d)
{
    UART1_push(&UART1_IN_fifo, d);

    if (d == FrameLf)
    {
        Transport_frame_count++;
    }
}

void discard_current_frame(void)
{
    uint8_t d;

    while(UART1_IN_count())
    {
        d = UART1_IN_pop();

        if (d == FrameLf)
            break;
    }
}

void init_EMICfb ()
{
	HAL_GPIO_PinCfg(RS485_RX, GPIO_INPUT);
	HAL_GPIO_PinCfg(RS485_TX, GPIO_OUTPUT);
	UART1_init();
	// RS485 simplex mode: RTS follows TX activity
	U1MODEbits.RTSMD = 1;
	U1MODEbits.UEN1 = 1;
	U1MODEbits.UEN0 = 0;
}

void poll_fieldBusTransport()
{
    if (Transport_frame_count > 0)
    {
        Transport_frame_count--;

        if(!UART1_IN_count()) return;

        uint8_t data = UART1_IN_pop();

        if(data == 0)
        {
            if(UART1_IN_count())
            {
                data = UART1_IN_pop();

                if (data == My_ID)
                {
                	
                    if (fieldBusOutStream.frame_count > 0)
                    {
                        send_fbTransport();
                    }
                    else
                    {
                        UART1_OUT_push(My_ID);
                        UART1_OUT_push(FrameLf);
                    }
                }
				discard_current_frame();   // <-- CAMBIO
            }
            return;
        }

        received_ID = data;

        if (received_ID == My_ID)
        {
            discard_current_frame();       // <-- CAMBIO
            return;
        }

        // Leer SOLO hasta LF
        while(UART1_IN_count())
        {
            data = UART1_IN_pop();

            if (data == FrameLf)
                break;

            streamPush(&fieldBusInStream, data);
        }

        if (fieldBusInStream.data_count_entr)
        {
            streamOpenWriteFrame(&fieldBusInStream);
        }
    }
}

void send_fbTransport(void)
{
    uint8_t data;

    UART1_OUT_push(My_ID);

    streamOpenReadFrame(&fieldBusOutStream);

    while (fieldBusOutStream.data_count_sal != 0)
    {
        data = streamPop(&fieldBusOutStream);
        UART1_OUT_push(data);
    }

    UART1_OUT_push(FrameLf);
}

void empty_buffer(void)   //empty UART buffer
{
	uint8_t d;
	while(UART1_IN_count())
	{
		d = UART1_IN_pop();
	}
	return;
}



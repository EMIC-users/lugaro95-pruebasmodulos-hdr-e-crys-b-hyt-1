// Define microcontroller type (assuming PIC24 family based on the configuration)

// I2C configuration (using I2C1 - no S_I2C2 defined)

// LEDs
#define TRIS_Led1	                _TRISA0
#define PORT_Led1 	                _A0
#define LAT_Led1 	                _LATA0
#define ODC_Led1	                _ODA0
#define PIN_Led1 	                _RA0
#define CN_Led1		                2
#define ADC_value_Led1              Buffer_entradas[0] 
#define HAL_SetAnalog_Led1()        {_PCFG0=0;\
                                        adc_addAnalogChannel(0);}


#define TRIS_Led2	_TRISA3
#define LAT_Led2 	_LATA3
#define ODC_Led2	_ODA3
#define PIN_Led2 	_RA3
#define CN_Led2		29
#define TRIS_Led3	_TRISA2
#define LAT_Led3 	_LATA2
#define ODC_Led3	_ODA2
#define PIN_Led3 	_RA2
#define CN_Led3		30

// RS485 communication
#define TRIS_RS485_TX	_TRISB4
#define LAT_RS485_TX 	_LATB4
#define ODC_RS485_TX	_ODB4
#define PIN_RS485_TX 	_RB4
#define RPOUT_RS485_TX	RPOR2bits.RP4R
#define RPIN_RS485_TX	4
#define CN_RS485_TX		1
#define TRIS_RS485_RX	                _TRISB5
#define PIN_RS485_RX 	                _RB5
#define Lat_RS485_RX 	                _LATB5
#define ODC_RS485_RX	                _ODB5
#define RPOUT_RS485_RX		            RPOR2bits.RP5R
#define RPIN_RS485_RX		            5
#define CN_RS485_RX		                27

// Click board interface pin definitions

// Crystal slave mode definition

// Board name definition
#define TRIS_AM2302_MISO	                _TRISB0
#define PIN_AM2302_MISO 	                _RB0
#define LAT_AM2302_MISO 	                _LATB0
#define ODC_AM2302_MISO	                _ODB0
#define RPOUT_AM2302_MISO		            RPOR0bits.RP0R
#define RPIN_AM2302_MISO		            0
#define CN_AM2302_MISO		                4
#define ADC_value_AM2302_MISO              Buffer_entradas[2]
#define HAL_SetAnalog_AM2302_MISO()        {_PCFG2=0;\
                                        adc_addAnalogChannel(2);}

#define setPullUpAM2302_MISO CNPU1bits.CN4PUE = 1

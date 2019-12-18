#define CAN_100K	100
#define CAN_125K	125
#define CAN_250K	250
#define CAN_500K	500
#define CAN_1000K	1000

#define CAN_SAMPLE_3	3


#define CAN11BITS				0xfffff800
#define CAN29BITS				0x80000000
#define SIMPLE_CAN				0x100
#define DOUBLE_CAN				0x101


#define OK						0
#define NOT_OK					-1
#define ERROR_MEMORY			-2


// -----------------------------------------------------------------------------------
// defines using the function 'instruction'

#define READ_EEPROM                 0x51
#define COUNTERRESET                0x80
#define READCTR                     0x81
#define SET_EFRREC_FREQ             0xB3
#define GET_CANPEGEL                0xB4
#define RESTART                     0xFE
#define STATUS_READ                 0xC0
#define GET_BUSLOAD                 0xC1
#define DEBUGMODE                   0xB5
#define ST_MIN_TIME                 0xB6
#define RTR_FRAME_CONF              0x60  // activate Remote Transmit-Frame
#define RTR_FRAME_CONF_CLEAR        0x61  // deactivate Remote Transmit-Frame
#define CYCLIC_TRANSMIT_CONF        0x62  // activate cyclic Transmit Frame
#define CYCLIC_TRANSMIT_CONF_CLEAR  0x63  // deactivate cyclic Transmit Frame
#define CONFIG_RECEIVE_BUFFER       0x66
#define READ_RECEIVE_BUFFER         0x67

// -----------------------------------------------------------------------------------

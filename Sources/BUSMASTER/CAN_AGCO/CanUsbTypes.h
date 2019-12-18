	#define	PORT_USB_CAN1				1
	#define	PORT_USB_CAN2				2
	#define	PORT_USB_CAN3				54
	#define	PORT_USB_CAN4				55

	//------------------ Gestion du type des messages CAN -------------

	#define CAN11BITS				(unsigned long)0x00
	#define CAN29BITS				(unsigned long)0x80000000

	#define CAN_REMOTE				0x01
	#define CAN_NOREMOTE			0x00

	#define CAN_100					100000
	#define CAN_125					125000	
	#define CAN_250					250000
	#define CAN_500					500000
	#define CAN_1000				1000000
	
		
	#define CAN_RAPPORT_50			50
	#define	CAN_SAMPLE_1			1
	#define	CAN_SAMPLE_3			3
	#define FACTEUR					0.00978
	#define ID_BusOff				0xFFFFFFFF
	#define ID_TxError				0xFFFFFFFE	
	
	

	//------------------------ Gestion des erreurs --------------------
	#ifndef NO_ERROR
	#define	NO_ERROR				0
	#endif

	#define NOT_OK					-1
	#define OK						0

	//---------------- N° de commande des fonctions supplémentaires ------------
	// ( A utiliser avec Lire_Donnee(int numero, int *error) )
	//
	//
	//								 | numero |									
	//								 |		  |
	//								 |		  |
	#define	ERROR_FRAMES_COUNTER		0x01
	#define	BUS_OFF_COUNTER				0x02
	#define BUS_OFF_COUNTER_CAN2		0x03
	#define RX_COUNTER					0x04
	#define TX_COUNTER					0x05
	#define BUS_LOAD_PERCENT			0x06	// BusLoad = Valeur / 100
	#define TX_ERROR_COUNTER			0x07
	#define TX_ERROR_COUNTER_CAN2		0x08
	
	#define DOMINANT_HIGH_LEVEL			0x09	//	Ces commandes concernent l'interface USB 
	#define DOMINANT_LOW_LEVEL			0x0A	//	|
	#define RECESSIF_HIGH_LEVEL			0x0B	//	|
	#define RECESSIF_LOW_LEVEL			0x0C	//	|
	#define	DOMINANT_ERROR_HIGH			0x0D	//	|
	#define	DOMINANT_ERROR_LOW			0x0E	//	|
	#define	RECESSIF_ERROR_HIGH			0x0F	//	|
	#define	RECESSIF_ERROR_LOW			0x10	//	|	
	#define READ_CANUSB_EEPROM			0x11	//	|__


	//----------------- Erreurs liées aux commandes supplémentaires -------------
	//
	//									  | error |
	//									  |		  |
	#define VALUE_OK						0x00	// La valeur renvoyée peut être prise en compte	
	#define UNKNOW_COMMAND					0x01	// N° de commande inconnue
	#define UNUSABLE_VALUE					0x02	// La valeur renvoyée n'est pas valide
	#define NOT_AVAILABLE					0x03	// N° de commande incompatible avec l'interface utilisée


	//---------------- N° de commande pour la fonction Ercire_Donnée -------------------
	//	
	//							 | numero |
	//						     |		  |
	#define CAN_LEVEL				0x01	// Demande la mesure des niveaux de tension des lignes Can
	#define FILTER_STATE			0x02	// Permet d'activer le filtre en envoyant 1 et de le désactiver avec 0
	#define WRITE_CANUSB_EEPROM		0x03


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

	#define ch2flag	0x01
	#define extflag	0x02
	#define rtrflag 0x04
	#define dirflag 0x08
	#define	errflag 0x10
	#define bulflag 0x20
	#define ctrflag 0x40
	#define datflag 0x80

	

//#endif
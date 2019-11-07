/* This file is generated by BUSMASTER */
/* VERSION [1.2] */
/* BUSMASTER VERSION [2.6.4] */
/* PROTOCOL [LIN] */

/* Start LIN include header */
#include <Windows.h>
#include <LINIncludes.h>

/* End LIN include header */


/* Start LIN global variable */

/* End LIN global variable */


/* Start LIN Function Prototype  */
GCC_EXTERN void GCC_EXPORT OnBus_Pre_Connect();
/* End LIN Function Prototype  */

/* Start LIN Function Wrapper Prototype  */
/* End LIN Function Wrapper Prototype  */


/* Start LIN generated function - OnBus_Pre_Connect */
void OnBus_Pre_Connect()
{
/* TODO */
STLIN_MSG sMsg3;

  sMsg3.messagetype = 1;                 // Slave Response
  sMsg3.checksumtype = 0;              // Classic
  sMsg3.dlc = 8;
  sMsg3.id = 0x3;                // Message Identifier
  sMsg3.data[1] = 10;  // Lower 4 Bytes
  sMsg3.data[7] = 20;  // Upper 4 Bytes
  sMsg3.cluster = 1;

  // Send the message
  SendMsg(sMsg3);

/* TODO */
STLIN_MSG sMsg4;

  sMsg4.messagetype = 1;                 // Slave Response
  sMsg4.checksumtype = 0;              // Classic
  sMsg4.dlc= 8;
  sMsg4.id = 0x4;                // Message Identifier
  sMsg4.data[1] = 10;  // Lower 4 Bytes
  sMsg4.data[7] = 20;  // Upper 4 Bytes
  sMsg4.cluster = 1;

  // Send the message
  SendMsg(sMsg4);
/* TODO */
STLIN_MSG sMsg8;

  sMsg8.messagetype = 1;                 // Slave Response
  sMsg8.checksumtype = 0;              // Classic
  sMsg8.dlc = 8;
  sMsg8.id = 0x8;                // Message Identifier
  sMsg8.data[1] = 10;  // Lower 4 Bytes
  sMsg8.data[7] = 20;  // Upper 4 Bytes
  sMsg8.cluster = 1;

  // Send the message
  SendMsg(sMsg8);

/* TODO */
STLIN_MSG sMsg10;

  sMsg10.messagetype = 1;                 // Slave Response
  sMsg10.checksumtype = 0;              // Classic
  sMsg10.dlc = 8;
  sMsg10.id = 0x10;                // Message Identifier
  sMsg10.data[1] = 10;  // Lower 4 Bytes
  sMsg10.data[7] = 20;  // Upper 4 Bytes
  sMsg10.cluster = 1;

  // Send the message
  SendMsg(sMsg10);
/* TODO */
STLIN_MSG sMsg11;

  sMsg11.messagetype = 1;                 // Slave Response
  sMsg11.checksumtype = 0;              // Classic
  sMsg11.dlc = 8;
  sMsg11.id = 0x11;                // Message Identifier
  sMsg11.data[1] = 10;  // Lower 4 Bytes
  sMsg11.data[7] = 20;  // Upper 4 Bytes
  sMsg11.cluster = 1;

  // Send the message
  SendMsg(sMsg11);

/* TODO */
STLIN_MSG sMsg9;

  sMsg9.messagetype= 1;                 // Slave Response
  sMsg9.checksumtype = 0;              // Classic
  sMsg9.dlc = 8;
  sMsg9.id = 0x9;                // Message Identifier
  sMsg9.data[1] = 10;  // Lower 4 Bytes
  sMsg9.data[7] = 20;  // Upper 4 Bytes
  sMsg9.cluster = 1;

  // Send the message
  SendMsg(sMsg9);

/* TODO */
STLIN_MSG sMsg12;

  sMsg12.messagetype= 1;                 // Slave Response
  sMsg12.checksumtype = 0;              // Classic
  sMsg12.dlc = 8;
  sMsg12.id = 0x12;                // Message Identifier
  sMsg12.data[1] = 10;  // Lower 4 Bytes
  sMsg12.data[7] = 20;  // Upper 4 Bytes
  sMsg12.cluster = 1;

  // Send the message
  SendMsg(sMsg12);

/* TODO */
}/* End LIN generated function - OnBus_Pre_Connect */

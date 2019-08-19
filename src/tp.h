#ifndef J1939_TP_SERVICES_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
int TxData(int socket, int nSize, unsigned char* data);
int RxData(int socket, unsigned char* buffer);
int TPInit();

#define MESSAGE_MAX_LENGTH   255
#define CAN_DATAFRAME_LENGTH   8
#define CAN_FLOW_CONTROL_MIN_LENGTH 3

#define SINGLE_FRAME        0x00
#define FIRST_FRAME         0x10
#define CONSECUTIVE_FRAME   0x20
#define FLOW_CONTROL        0x30

typedef enum _TP_STATES
{
	READY,
	FF_RX,
	FC_SEND,
	WAITING_FOR_RESP	
}TP_STATE;
#endif //J1939_TP_SERVICES_H
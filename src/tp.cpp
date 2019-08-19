#include <time.h>
#include "tp.h"


int g_socketID = 0;
int start, end;

int TxData(int socket, int nSize, unsigned char* data)
{
	if (nSize > 0 && nSize < 8)
	{
		struct can_frame frame;
		int iIndex = 2;
		frame.can_id = frame.can_id << 8 | data[0]; //Append destination address
		frame.can_id = frame.can_id << 8 | data[1]; //Append source address
		frame.can_id = frame.can_id | CAN_EFF_FLAG;
		printf("TxData socket %d nSize: %d %X\n", socket, nSize, frame.can_id);
		frame.can_dlc = 3;
		for (; iIndex < nSize; iIndex++)
		{
			frame.data[iIndex - 2] = data[iIndex];
			printf("TxData socket frame.data[%d] = %x\n", iIndex - 2, data[iIndex]);
		}
		return write(socket, &frame, sizeof(struct can_frame));
	}
	else
	{
		return 0;
	}

}

int RxData(int socket, unsigned char* buffer)
{
	struct can_frame frame;
	int nbytes = read(socket, &frame, sizeof(struct can_frame));
	if (nbytes != sizeof(frame))
	{
		perror("read socket");
		return 1;
	}
	else
	{
		if (frame.can_id & CAN_EFF_FLAG)
		{
			//0x18DAF955
			int CanID = frame.can_id & CAN_EFF_MASK;
			printf("%08X%d\n", CanID, frame.can_dlc);
			int nSrc = CanID & 0xFF;
			int nDest = CanID >> 8 & 0xFF;
			int PGN = CanID >> 16 & 0xFF;
			printf("TxData PGN = %x SA: %x DA: %x \n", PGN, frame.can_id, nSrc, nDest);
		}
	}
}

int TPInit()
{
	int nbytes;
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;

	const char *ifname = "vcan0";

	if ((g_socketID = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("Error while opening socket");
		return -1;
	}

	strcpy(ifr.ifr_name, ifname);
	ioctl(g_socketID, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	if (bind(g_socketID, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}
	return 0;
}

int HandleRespBlock(int SA, int DA)
{

}

int TPExecuteRequest(int nMsgSize, unsigned char* pMsgData, int &RespSize, unsigned char** pRespData)
{
	start = clock() * 1000 / CLOCKS_PER_SEC;
	end = start + 5000; //Set 5 second time out
	TP_STATE state = READY;
	bool bFullRespRx = false;
	TxData(g_socketID, nMsgSize, pMsgData);
	state = WAITING_FOR_RESP;
	do
	{
		switch (state)
		{

		case FF_RX:
		{

		}
		break;
		case FC_SEND:
		{

		}
		break;
		case WAITING_FOR_RESP:
		{

		}
		break;
		default:
			break;
		}
	} while (start <= end && bFullRespRx != true);
}
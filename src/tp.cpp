#include <time.h>
#include <fcntl.h>
#include "tp.h"

int g_socketID = 0;
int g_nToolAddress = 0xF9;
int g_nECUAddress = 0x33;
int g_nPGN = 0xDA; //PGN for sending UDS request over J1939, Phy request

int TxData(int nSize, unsigned char* data)
{
	if (nSize > 1 && nSize <= 8)
	{
		struct can_frame frame;
		
		frame.can_id = 0x18DA;
		frame.can_id = frame.can_id << 8 | g_nECUAddress; //Append destination address
		frame.can_id = frame.can_id << 8 | g_nToolAddress; //Append source address
		frame.can_id = frame.can_id | CAN_EFF_FLAG;
		
		printf("TxData socket %d nSize: %d %X\n", socket, nSize, frame.can_id);
		frame.can_dlc = nSize;
		for (int iIndex = 0; iIndex <= nSize; iIndex++)
		{
			frame.data[iIndex] = data[iIndex];
			printf("TxData socket frame.data[%d] = %x\n", iIndex, data[iIndex]);
		}
		return write(g_socketID, &frame, sizeof(struct can_frame));
	}
	else
	{
		return 0;
	}

}

int RxData(unsigned char* buffer)
{
	struct can_frame frame;
	int nbytes = read(g_socketID, &frame, sizeof(struct can_frame));
	if (nbytes != sizeof(frame))
	{
		perror("read socket");
		return 0;
	}
	else
	{
		if (frame.can_id & CAN_EFF_FLAG)
		{
			//0x18DAF955
			int CanID = frame.can_id & CAN_EFF_MASK;
			int nSrc = CanID & 0xFF;
			int nDest = CanID >> 8 & 0xFF;
			int PGN = CanID >> 16 & 0xFF;
			if (nSrc == g_nECUAddress && nDest == g_nToolAddress && PGN == g_nPGN)
			{
				for (int i = 0; i < frame.can_dlc; i++)
				{
					buffer[i] = frame.data[i];
				}
				return frame.can_dlc;
			}
			else
			{
				return 0;
			}

			//printf("TxData PGN = %x SA: %x DA: %x \n", PGN, frame.can_id, nSrc, nDest);
		}
	}
}

int TPInit()
{
	int nbytes;
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;

	const char *ifname = "can0";

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
	
	//Why we can not set non blocking socket? It throws error "read socket: Resource temporarily unavailable"????
	//fcntl(g_socketID, F_SETFL, O_NONBLOCK);
	/*struct timeval tv;
	tv.tv_sec = 0; //Timeout in seconds
	tv.tv_usec = 10000;
	setsockopt(g_socketID, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);*/
	return 0;
}


int TxFlowControl()
{
	
	unsigned char sData[] = { 0x30, 0x00, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	return TxData(sizeof(sData), sData);
}

void printBuffer(unsigned char* buffer, int nSize)
{
	printf("********** Buffer data Start ********************");
	for (size_t i = 0; i < nSize; i++)
	{
		printf("\t buffer[%d]: %02X\n", i, buffer[i]);
	}
}

//TBD 0x7F NRC 78 and 21
std::vector<unsigned char> TPExecuteRequest(int nMsgSize, unsigned char* pMsgData)
{	
	bool bFullRespRx = false;
	TxData(nMsgSize, pMsgData);
	unsigned char rxBuff[8];	
	int RespSize = 0;
	std::vector<unsigned char> vrxData;
	printf("Before Execute loop\n");	
	clock_t start_time = clock(); 
	clock_t end_time; 
	float fTimeOut = 6000.0;
	float fTimeElapse = 0;
	do
	{	
		int nData = RxData(rxBuff);	
		if (nData > 0)
		{
			//printBuffer(rxBuff, sizeof(rxBuff));
			int FrameType = rxBuff[0] & 0xF0;
			switch (FrameType)
			{
				case SINGLE_FRAME:
				{
					//TBD
					
				}
				break;
				case FIRST_FRAME:
				{
					//UUDT
					RespSize = rxBuff[1];
					printf("******* Inside FF DataSize:%d ***********\n", RespSize);
					//Set offce as 2 Frame ID and size
					for (size_t i = 2; i < nData; i++)
					{
						vrxData.push_back(rxBuff[i]);
					}
					//printf("Tx FlowControl Before");
					unsigned char sData[] = { 0x30, 0x00, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
					TxData(sizeof(sData), sData);
					//printf("Tx FlowControl After");

					
				}
				break;
				case CONSECUTIVE_FRAME:
				{
					for (size_t i = 1; i < nData; i++)
					{
						vrxData.push_back(rxBuff[i]);
					}
				}
				break;
				default:
					break;
			}
			if (RespSize <= vrxData.size() && fTimeElapse < fTimeOut)
			{
				bFullRespRx = true;
			}
		}
		end_time = clock() - start_time;
		fTimeElapse = (float)end_time/CLOCKS_PER_SEC;
		printf("Start: %f End:%f FullResp:%d \n", fTimeElapse, fTimeOut, bFullRespRx);
	} while ( fTimeElapse < fTimeOut && bFullRespRx != true);
	printf("Out Execute loop\n");	
	if (!bFullRespRx && fTimeElapse >= fTimeOut)
	{
		printf("Timeout Error occur, No response from ECU\n");
	}
	return vrxData;
}
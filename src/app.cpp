#include "app.h"

int AppInit()
{
	return TPInit();
}

std::vector<unsigned char> GetDummyData()
{
	std::vector<unsigned char> vTPResp;
	//FF
	vTPResp.push_back(0x59);
	vTPResp.push_back(0x02);
	vTPResp.push_back(0x01);
	vTPResp.push_back(0xF2);
	vTPResp.push_back(0x00);
	vTPResp.push_back(0x10);
	//SF
	vTPResp.push_back(0x01);
	vTPResp.push_back(0xF1);
	vTPResp.push_back(0x00);
	vTPResp.push_back(0x01);
	vTPResp.push_back(0x01);
	vTPResp.push_back(0xA1);
	vTPResp.push_back(0x03);
	//TF
	vTPResp.push_back(0x0C);
	vTPResp.push_back(0x01);
	vTPResp.push_back(0xF1);
	vTPResp.push_back(0x00);
	vTPResp.push_back(0x12);
	vTPResp.push_back(0x01);
	return vTPResp;
}

std::vector<DTC> GetActiveDTCs()
{
	std::vector<DTC> dtcList;
	unsigned char ActiveDTCReq[] = { 0x03, 0x019, 0x02, 0x01 };
	std::vector<unsigned char> vTPResp = TPExecuteRequest(sizeof(ActiveDTCReq), ActiveDTCReq);
	if (vTPResp.size() > 0)
	{
		//std::vector<unsigned char> vTPResp = GetDummyData();
		int nNumDTC = vTPResp.size() - 3;
		nNumDTC = nNumDTC / 4;
		printf("TP rxData size: %d Number of DTC: %d\n", vTPResp.size(), nNumDTC);
		int nDataOffet = 3;
		for (size_t i = 0; i < nNumDTC; i++)
		{
			DTC dtc;
			dtc.m_nID = 0;
			dtc.m_nStatus = 0;
			dtc.m_nID = vTPResp[nDataOffet + 4 * i] | vTPResp[nDataOffet + 4 * i + 1] << 8 | vTPResp[nDataOffet + 4 * i + 2] << 16;
			dtc.m_nStatus = vTPResp[nDataOffet + 4 * i + 3];
			//printf("%X %X %X %X\n", vTPResp[nDataOffet + 4*i], vTPResp[nDataOffet + 4*i+1], vTPResp[nDataOffet + 4*i+2], vTPResp[nDataOffet + 4*i+3]);
			//printf("*************************************\n");
			printf("DTC ID: %d Status: %d\n", dtc.m_nID, dtc.m_nStatus);
		}

	}
else
{
printf("No data from Tp layer");
}
	
}
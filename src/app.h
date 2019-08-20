#ifndef J1939_APP_SERVICES_H
#include "tp.h"
typedef struct _DTC
{
	int m_nID;
	int m_nStatus;
}DTC;
std::vector<DTC> GetActiveDTCs();
int AppInit();
#endif
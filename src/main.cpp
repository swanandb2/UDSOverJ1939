#include "tp.h"

unsigned char buff[8];

int main(void)
{
	if (0==TPInit())
	{
		//Transport layer init properly
	}
	//TBD
	/*unsigned char sData[] = {0x55,0xF9,0x19,0x02,0x01};
	
	int nbytes = TxData(s, sizeof(sData),  sData);
	
	while(1)
	{
		RxData(s, 0);
	}

	printf("Wrote %d bytes\n", nbytes);*/
	
	return 0;
}
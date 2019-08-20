#include "app.h"
#include <signal.h>

bool bKeepMonitoring = false;
void sig_handler(int sig) 
{
	switch (sig) 
	{
		case SIGKILL:
		{
			bKeepMonitoring = false;
			sleep(1); //Allow 1 second for cleanup
		}

	}
}

int main(void)
{	
	if (0 == AppInit())
	{
		signal(SIGKILL, sig_handler);
		//ATP layer init properly
		bKeepMonitoring = true;
		while (bKeepMonitoring)
		{
			std::vector<DTC> dtcList = GetActiveDTCs();
		}	

	}
	else
	{
		printf("Application failed to initialize");
	}
	return 0;
}
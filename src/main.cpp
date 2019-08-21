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
		std::vector<DTC> dtcList = GetActiveDTCs();
		// while (bKeepMonitoring)
		// {
			
			// sleep(2); //Allow 1 second for cleanup
		// }	

	}
	else
	{
		printf("Application failed to initialize");
	}
	return 0;
}
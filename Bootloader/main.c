
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
extern void UartInit(uint32_t baud);
extern void TimeBaseInit(void);

#define APPLICATION_ADDRESS     (uint32_t)0x08004000 
typedef  void (*pFunction)(void);
pFunction JumpToApplication;
uint32_t JumpAddress;
int main()
{
	UartInit(115200);
//	TimeBaseInit();

	printf("this is bootloader!!!!\n");
	printf(" 1 s later jump to application\n ");
	/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
	if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
	{
		printf(" jump to  application \n");
		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		JumpToApplication = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		JumpToApplication();
	}
	printf(" no application \n");
	while(1);
}



























/*****************************************************************/

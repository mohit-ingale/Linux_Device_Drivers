#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<iocth.h>
#include<sys/ioctl.h>

int main()
{
	int fd,delay;
	fd=open("/dev/mydevice",O_RDWR);
	if(fd<0)
	{
		printf("sorry! failed to open device\n");
		exit(0);
	}	
	while(1)
	{
		printf("enter delay = ");
		scanf("%d",&delay);
		ioctl(fd,CMD_WRITE,delay);
	}
}

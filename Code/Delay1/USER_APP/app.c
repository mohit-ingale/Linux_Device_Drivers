#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include"ioctl.h"

int main()
{
	char ubuff[64];
	int fd,size,del;

	fd=open("/dev/device1",O_RDWR);
	if(fd==-1)
	{
		perror("Error Reading\n");
		exit(EXIT_FAILURE);
	}
	printf("enter delay :");
	scanf("%d",&del);
	ioctl(fd,BASIC_DELAY,del);
	size=read(fd,ubuff,sizeof(ubuff));
	printf("%s\n",ubuff);
	close(fd);

	return 0;
}

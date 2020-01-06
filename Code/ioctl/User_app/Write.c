#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include <sys/ioctl.h>

#include"ioct.h"
int main()
{
	int fd,size;
	char ubuff[]="Hello sumit";
	fd=open("/dev/CharDriver12",O_RDWR);
	if(fd==-1)
	{
		perror("Error Writing\n");
		exit(EXIT_FAILURE);
	}
	ioctl(fd,BASIC_RESET);
	ioctl(fd,BASIC_PRINT,56);
	size=write(fd,ubuff,sizeof(ubuff));
	size=read(fd,ubuff,sizeof(ubuff));
		printf("%s\n",ubuff);
	close(fd);
	return 0;
}

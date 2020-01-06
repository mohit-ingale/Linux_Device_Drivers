#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

int main()
{
	char ubuff[256];
	int fd,size;
	
	fd=open("/dev/cirular",O_RDWR);
	if(fd==-1)
	{
		perror("Error Writing\n");
		exit(EXIT_FAILURE);
	}
	write(fd,"Hello from user",15);
	read(fd,ubuff,sizeof(ubuff));
	printf("ubuff=%s\n",ubuff);
	close(fd);
	return 0;
}

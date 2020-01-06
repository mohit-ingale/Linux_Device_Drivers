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
	
	fd=open("/dev/CharDriver",O_RDWR);
	if(fd==-1)
	{
		perror("Error Writing\n");
		exit(EXIT_FAILURE);
	}
	size=write(fd,"Hello",5);
	close(fd);
	return 0;
}

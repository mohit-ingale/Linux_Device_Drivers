#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

int main()
{
	int fd,size;
	char ubuff[]="From Driver 2";
	fd=open("/dev/CharDriver2",O_RDWR);
	if(fd==-1)
	{
		perror("Error Writing\n");
		exit(EXIT_FAILURE);
	}
	size=write(fd,ubuff,sizeof(ubuff));
	close(fd);
	return 0;
}

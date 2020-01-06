#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

int main()
{
	char ubuff[64];
	int fd,size;
	
	fd=open("/dev/CharDriver1",O_RDWR);
	if(fd==-1)
	{
		perror("Error Reading\n");
		exit(EXIT_FAILURE);
	}
	size=read(fd,ubuff,sizeof(ubuff));
	printf("%s\n",ubuff);
	close(fd);
	
	return 0;
}

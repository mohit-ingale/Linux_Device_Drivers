#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include <unistd.h>

int main()
{
	int fd,a,b;
	char ac[4],bc[4];
	char buff[100];
	fd=open("/dev/mydevice",O_RDWR);
	if (fd < 0){
      perror("Failed to open the device...");
      return -1;
   }
	printf("Enter two Numbers\n");
	scanf("%d %d",&a,&b);
	sprintf(ac,"%d",a);
	sprintf(bc,"%d",b);
	//printf("ac=%s\n",ac);
//	printf("bc=%s\n",bc);
	write(fd,ac,sizeof(ac));
	write(fd,bc,sizeof(bc));
	read(fd,buff,100);
	printf("result= %s\n",buff);
}

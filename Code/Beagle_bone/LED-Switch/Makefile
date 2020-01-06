obj-m := LEDSwitch.o


KERNELDIR = /lib/modules/$(shell uname -r)/build
#KERNELDIR = /lib/modules/4.4.9-ti-r25/build
PWD := $(shell pwd)


default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	#$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
#	$(MAKE) IOCTL_Application
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	depmod -a


# Specify the output file
obj-m := prog2.o
# Setup the variables
KERN_SRC = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
# make target for compiling the module
modules :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
# make target for cleaning up the space
clean :
	make -C $(KERN_SRC) M=$(PWD) clean

/*
===============================================================================
Driver Name		:		Delay
Author			:		M
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"Delay.h"
#include<linux/fs.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include"ioctl.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("M");

dev_t dev;
int delay;
char kbuff[]="Delay!";
struct cdev cdv;
int dev_open(struct inode *inode, struct file *filp)
{
	return 0;
}

int dev_release(struct inode *inode, struct file *filp)
{
	return 0;
}
ssize_t dev_read(struct file *filp, char __user *ubuff, size_t size, loff_t *offset)
{
	int j;
	j=jiffies+(HZ*delay);
	while(time_before(jiffies,j)){
		cpu_relax();
	}
	copy_to_user(ubuff,kbuff,sizeof(kbuff));
	return 0;
}
long dev_ioctl(struct file *filp, unsigned int cmd, unsigned long data)
{
	switch(cmd){
		case CMD_WRITE:
			delay=data;
			printk("delay=%d\n",delay);
			break;
		default:
			printk("out of order\n");
			return -EINVAL;
	}
	return 0;

}

struct file_operations fops={
		.open=dev_open,
		.release=dev_release,
		.read=dev_read,
		.unlocked_ioctl=dev_ioctl,
};

static int __init Delay_init(void)
{
	int ret;
	dev=MKDEV(255,0);
	ret=register_chrdev_region(dev,1,"device");
	if(ret)
	{
		printk("error registering device\n");
		return -EFAULT;
	}
	cdev_init(&cdv,&fops);
	ret=cdev_add(&cdv,dev,1);
	if(ret)
	{
		printk("error adding\n");
		unregister_chrdev_region(dev,1);
		return -EFAULT;
	}
	return 0;
}

static void __exit Delay_exit(void)
{	

}

module_init(Delay_init);
module_exit(Delay_exit);


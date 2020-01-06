/*
===============================================================================
Driver Name		:		delay
Author			:		MOHIT
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"delay.h"
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include "ioctl.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MOHIT");
int del;
dev_t dev;
struct cdev *my_cdev;


int delay_open(struct inode *inode, struct file *filp)
{
	printk("device opened successfully\n");
	return 0;
}
int delay_release(struct inode *inode, struct file *filp)
{
	printk("device closed successfully\n");
	return 0;
}
ssize_t delay_read(struct file *filp, char __user *ubuff, size_t size, loff_t *offset)
{
	unsigned long int j;
	char kbuff[]="hello user";
	j=jiffies+del*HZ;
	while(time_before(jiffies,j))
	{
		cpu_relax();
	}
	copy_to_user(ubuff,kbuff,sizeof(kbuff));
	printk("data coppied successfully\n");
	//int min_cnt;
	return 0;

}
ssize_t delay_write(struct file *filp, const char __user *ubuff, size_t size, loff_t *offset)
{
	return 0;
}
long delay_ioctl(struct file *filp, unsigned int cmd, unsigned long data)
{
	switch(cmd)
	{
	case BASIC_DELAY:
			del=data;
			break;
		default:
			return -ENOTTY;
	}
	return 0;
}

struct file_operations fops={
		.open=delay_open,
		.release=delay_release,
		.read=delay_read,
		.write=delay_write,
		.unlocked_ioctl=delay_ioctl,
};

static int __init delay_init(void)
{
	int major,minor,ret;
	alloc_chrdev_region(&dev,0,1,"delay");
	major=MAJOR(dev);
	minor=MINOR(dev);
	printk("Major No.= %d \t Minor No.=%d\n",major,minor);
	printk("Initlializing Delay Module\n");
	my_cdev=cdev_alloc();
	my_cdev->ops=&fops;
	ret=cdev_add(my_cdev,dev,1);
	if(ret)
	{
		printk("error adding device \n");
		return -EFAULT;
	}
	return 0;
}

static void __exit delay_exit(void)
{	
	cdev_del(my_cdev);
	unregister_chrdev_region(dev,1);
	printk("exiting delay module\n");
	PINFO("EXIT\n");

}

module_init(delay_init);
module_exit(delay_exit);


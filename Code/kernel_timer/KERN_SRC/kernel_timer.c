/*
===============================================================================
Driver Name		:		delay
Author			:		MOHIT
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"kernel_timer.h"
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <asm/param.h>
#include "ioctl.h"
#include <linux/gpio.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MOHIT");
int del=1;
int val=1;
unsigned int gpioLED = 49;
dev_t dev;
struct cdev *my_cdev;
struct timer_list stime;

void delay_create(unsigned long del1)
{
	printk("jiffies=%ld\tdata=%ld\n",jiffies,del1);
	val=!val;
	gpio_set_value(49,val);
	stime.expires=jiffies+del1*HZ;
		stime.data=del;
		stime.function=delay_create;
		add_timer(&stime);
	//del_timer(&stime);
	return;
}


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
	char kbuff[]="hello user";

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

	init_timer(&stime);
	printk("jiffies=%ld\n",jiffies);
			stime.expires=jiffies+del*HZ;
			stime.data=del;
			stime.function=delay_create;
			add_timer(&stime);
	printk("HZ=%d\n",HZ);
	ret=gpio_is_valid(gpioLED);
	gpio_request(49,"sysfs");
	gpio_direction_output(49,val);
	gpio_export(49,false);
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
	del_timer(&stime);
	gpio_unexport(49);
	cdev_del(my_cdev);
	unregister_chrdev_region(dev,1);
	printk("exiting delay module\n");
	PINFO("EXIT\n");

}

module_init(delay_init);
module_exit(delay_exit);


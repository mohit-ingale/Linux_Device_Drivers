/*
===============================================================================
Driver Name		:		charDrive
Author			:		M
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"charDrive.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("M");
dev_t dev;
char kbuff[256];

int s_open(struct inode *ip, struct file *filp)
{
	return 0;
}
int s_release(struct inode *ip, struct file *filp)
{
	return 0;
}
ssize_t s_read(struct file *filp, char __user *ubuff, size_t size, loff_t *off)
{
	printk("Read func from user\n");
	copy_to_user(ubuff,kbuff,sizeof(kbuff));
	return 0;
}
ssize_t s_write(struct file *filp, const char __user *ubuff, size_t size, loff_t *off)
{
	copy_from_user(kbuff,ubuff,sizeof(kbuff));
	printk("Read form user:%s\n",kbuff);
	return 0;
}


struct file_operations fop=
{
	.open=s_open,
	.release=s_release,
	.read=s_read,
	.write=s_write,
};

struct cdev cdv;




static int __init charDrive_init(void)
{
	int reg,ret;
	printk("Initializing ChDriver\n");
	dev=MKDEV(42,0);
	reg=register_chrdev_region(dev,1,"ChDriver");
	if(reg)
	{
		printk("Registering error\n");
		return reg;
	}

	cdev_init(&cdv,&fop);

	ret=cdev_add(&cdv,dev,1);
	if(ret)
	{
		printk("Error adding driver\n");
		unregister_chrdev_region(dev,1);
		return -EFAULT;
	}
	printk("ChDriver initialized\n");
	return 0;
}

static void __exit charDrive_exit(void)
{
	printk("Exiting ChDriver\n");
	unregister_chrdev_region(dev,1);
}

module_init(charDrive_init);
module_exit(charDrive_exit);

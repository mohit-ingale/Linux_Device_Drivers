/*
===============================================================================
Driver Name		:		CirBuff
Author			:		Mohit
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"ioctl.h"
#include"ioct.h"
#include<linux/ioctl.h>
#define CSIZE 64
#define MINORS 3
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohit");

dev_t dev;
struct devs{
		struct circ_buf cbuff;
		struct cdev cdv;
		wait_queue_head_t wq;
}devs[MINORS];
int s_open(struct inode *inode, struct file *filp)
{
	struct devs *d_dev;

	d_dev=container_of(inode->i_cdev,struct devs,cdv);
	filp->private_data=d_dev;
	return 0;
}
int s_release(struct inode *ip, struct file *filp)
{
	return 0;
}
ssize_t s_read(struct file *filp, char __user *ubuff, size_t size, loff_t *off)
{
	int min_cnt;
	int ret;
	struct devs *d_dev;
	d_dev=filp->private_data;
	printk("Read func from user\n");
	wait_event_interruptible(d_dev->wq,CIRC_CNT(d_dev->cbuff.head,d_dev->cbuff.tail,CSIZE));
	min_cnt=min(CIRC_CNT(d_dev->cbuff.head,d_dev->cbuff.tail,CSIZE),size);
	printk("mincnt=%d\n",min_cnt);
	ret = copy_to_user(ubuff,&d_dev->cbuff.buf[d_dev->cbuff.tail],min_cnt);
	if(ret){
		printk("Error in copy to user\n");
		return -EFAULT;
	}
	d_dev->cbuff.tail=(d_dev->cbuff.tail+min_cnt)&(CSIZE-1);
	return 0;
}
ssize_t s_write(struct file *filp, const char __user *ubuff, size_t size, loff_t *off)
{
	int min_cnt,ret;
	struct devs *d_dev;
		d_dev=filp->private_data;
	min_cnt=min(CIRC_SPACE(d_dev->cbuff.head,d_dev->cbuff.tail,CSIZE),(int)size);
	printk("Write mincnt=%d\n",min_cnt);
	ret = copy_from_user(&d_dev->cbuff.buf[d_dev->cbuff.head],ubuff,min_cnt);
	if(ret){
			printk("Error in copy to user\n");
			return -EFAULT;
	}
	d_dev->cbuff.head=(d_dev->cbuff.head+min_cnt)&(CSIZE-1);
	printk("Read form user:%s\n",&d_dev->cbuff.buf[d_dev->cbuff.tail]);
	wake_up(&d_dev->wq);
	return 0;
}

long ioctl(struct file *filp, unsigned int cmd, unsigned long argc)
{
	switch(cmd){
	case BASIC_RESET:
			printk("this is basic reset command\n");
			break;
	case BASIC_PRINT:
			printk("print = %ld\n",argc);
			break;
	default:
			return -ENOTTY;
	}
	return 0;
}
struct file_operations fop=
{
	.open=s_open,
	.release=s_release,
	.read=s_read,
	.write=s_write,
	.unlocked_ioctl=ioctl,
};


static int __init CirBuff_init(void)
{
	int reg,ret,i,j;
	printk("Initializing ChDriver\n");
	dev=MKDEV(42,0);
	reg=register_chrdev_region(dev,MINORS,"ChDriver");
	if(reg)
	{
		printk("Registering error\n");
		return reg;
	}
	for(i=0;i<MINORS;i++)
	{
		cdev_init(&devs[i].cdv,&fop);
		devs[i].cbuff.buf=kmalloc(CSIZE,GFP_KERNEL);
		if(devs[i].cbuff.buf==NULL)
		{
			printk("error allocating space\n");
			goto err1;
		}
		init_waitqueue_head(&devs[i].wq);
	}
	for(j=0;j<MINORS;j++)
	{
		ret=cdev_add(&devs[j].cdv,dev,3);
		if(ret)
		{
			goto err2;
		}
		printk("ChDriver initialized\n");
			return 0;
	}

	err2:
	for(j--;j>=0;j--)
	{
		cdev_del(&devs[j].cdv);
	}

	err1:
	for(i--;i>=0;i--)
	{
		kfree(&devs[i].cbuff.buf);
		return -EFAULT;
	}


}

static void __exit CirBuff_exit(void)
{
	int i;
	printk("Exiting ChDriver\n");
	for(i=0;i<MINORS;i++)
	{
		kfree(&devs[i].cbuff.buf);
		cdev_del(&devs[i].cdv);

	}

	unregister_chrdev_region(dev,3);
}

module_init(CirBuff_init);
module_exit(CirBuff_exit);

#include"MultiDrive.h"
#define MINORS 2
#define CSIZE 64
MODULE_LICENSE("GPL");
MODULE_AUTHOR("M");

dev_t dev,dev1;

struct multi{
struct circ_buf cbuff;
struct cdev cdv;
wait_queue_head_t wq;
}mul[2];

int s_open(struct inode *ip, struct file *filp)
{
	struct multi *temp;
	temp=container_of(ip->i_cdev,struct multi,cdv);
	filp->private_data=temp;
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
	struct multi *temp;
	printk("Read func from user\n");
	temp=filp->private_data;
	wait_event_interruptible(temp->wq,CIRC_CNT(temp->cbuff.head,temp->cbuff.tail,CSIZE));
	min_cnt=min(CIRC_CNT(temp->cbuff.head,temp->cbuff.tail,CSIZE),size);
	printk("mincnt=%d\n",min_cnt);
	ret = copy_to_user(ubuff,&temp->cbuff.buf[temp->cbuff.tail],min_cnt);
	if(ret){
		printk("Error in copy to user\n");
		return -EFAULT;
	}
	temp->cbuff.tail=(temp->cbuff.tail+min_cnt)&(CSIZE-1);
	return 0;
}
ssize_t s_write(struct file *filp, const char __user *ubuff, size_t size, loff_t *off)
{
	int min_cnt,ret;
	struct multi *temp;
	temp=filp->private_data;
	min_cnt=min(CIRC_SPACE(temp->cbuff.head,temp->cbuff.tail,CSIZE),(int)size);
	printk("Write mincnt=%d\n",min_cnt);
	ret = copy_from_user(&temp->cbuff.buf[temp->cbuff.head],ubuff,min_cnt);
	if(ret){
			printk("Error in copy to user\n");
			return -EFAULT;
	}
	temp->cbuff.head=(temp->cbuff.head+min_cnt)&(CSIZE-1);
	printk("Read form user:%s\n",&temp->cbuff.buf[temp->cbuff.tail]);
	wake_up(&temp->wq);
	return 0;
}


struct file_operations fop=
{
	.open=s_open,
	.release=s_release,
	.read=s_read,
	.write=s_write,
};


static int __init MultiDrive_init(void)
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

		cdev_init(&mul[i].cdv,&fop);
		mul[i].cbuff.buf=kmalloc(CSIZE,GFP_KERNEL);
		if(mul[i].cbuff.buf==NULL)
		{
			printk("error allocating space\n");
			goto err1;
		}
		init_waitqueue_head(&mul[i].wq);
	}
	for(j=0;j<MINORS;j++)
	{
		dev1=MKDEV(MAJOR(dev),MINOR(dev)+j);
		ret=cdev_add(&mul[j].cdv,dev1,1);
		if(ret)
		{

			printk("Error adding driver\n");
			goto err2;
		}
	}

	printk("ChDriver initialized\n");
	return 0;

	err2:
		for(j--;j>=0;j--)
		{
			cdev_del(&mul[j].cdv);
		}
	err1:
		for(i--;i>=0;i--)
		{
			kfree(mul[i].cbuff.buf);
		}
		unregister_chrdev_region(dev,MINORS);
		return -EFAULT;
}

static void __exit MultiDrive_exit(void)
{
	int i;
	printk("Exiting ChDriver\n");
	for(i=0;i<MINORS;i++)
	{
		cdev_del(&mul[i].cdv);
		kfree(mul[i].cbuff.buf);
	}
	unregister_chrdev_region(dev,MINORS);
}

module_init(MultiDrive_init);
module_exit(MultiDrive_exit);

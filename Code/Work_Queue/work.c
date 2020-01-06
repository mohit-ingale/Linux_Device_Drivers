#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>	//dev_t
#include<linux/kdev_t.h> //Holds MACROs for MAJOR and MINOR
#include<linux/fs.h>	//file operations structure and file structure
#include<linux/cdev.h>	//Char Device Registration- cdev structure
#include<asm/uaccess.h>	//copy to and copy from functions
#include<linux/errno.h>
MODULE_LICENSE("GPL");
#include<linux/workqueue.h>
#include<linux/interrupt.h>
#define MAX 1024


		int major=47;
		int minor=0;
		dev_t device_number;
		struct work_struct work;
		struct workqueue_struct queue;
		unsigned long j;
		char Kbuff[MAX];

		void workfun(void * data)
			{	
				data="Surya Hi";
				strcpy(Kbuff,data);
			}

		int my_open(struct inode *inode,struct file *filp)
				{
					printk("\nU are in Open function\n");
					return 0;
				}

		int my_release(struct inode *inode,struct file *filp)
				{
					printk("\nU are in Release function\n");
					return 0;
				}

		ssize_t my_read(struct file *filp,char __user *Ubuff,size_t count,loff_t *offp)
			{
				int retval,copyto;
				copyto=copy_to_user((char *)Ubuff,(char *)Kbuff,count);
				printk("Copied from Tasklet %s to Ubuff:",Kbuff);
				return count;
				if(copyto < 0)
				{
					printk("\nError in copy to user\n");
					retval=-EFAULT;
					return retval;
				}

			}

		struct file_operations fops={		//file operations to be done on driver

		.owner=THIS_MODULE,
		.open=my_open,
		.release=my_release,
		.read=my_read,
		};

		struct cdev *my_cdev;
		static int __init dev_init(void)
		{
			//Allocation of device numbers for drivers use.
			int alloc_devnumber,charreg;
			device_number=MKDEV(major,minor);
			alloc_devnumber=register_chrdev_region(device_number,1,"TASK_Device");
			if(alloc_devnumber < 0)
				{
					printk("Device Number is not  allocated\n");
					//exit(EXIT_FAILURE);
				}
			//Char device registration
			my_cdev=cdev_alloc();
			my_cdev->ops=&fops;
			&queue= create_singlethread_workqueue("My Work Queue");
			INIT_WORK(&work,workfun);
			queue_work(&queue,&work);


			charreg=cdev_add(my_cdev,device_number,1);
			if(charreg < 0)
				{
					printk("Device is not added to the system\n");
					//exit(EXIT_FAILURE);
				}
			return 0;
		}

		static void __exit dev_clean(void)
		{

			cdev_del(my_cdev);
			destroy_workqueue(&queue);
			unregister_chrdev_region(device_number,1);
			printk("Bye Bye Driver\n");
		}



		module_init(dev_init);
		module_exit(dev_clean);


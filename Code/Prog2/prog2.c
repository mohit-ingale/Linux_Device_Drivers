#include<linux/init.h>
#include<linux/module.h>
#include<linux/stat.h>
MODULE_LICENSE("GPL");
int kernel_add(int ,int);
int kernel_add(int a,int b)
{
  return a+b;
}
EXPORT_SYMBOL(kernel_add);

static int param_init(void)
{
  printk("Initilizing Addition Module\n");
  return 0;
}

static void param_return(void)
{
  printk("Exiting Addition module\n");
  return;
}

module_init(param_init);
module_exit(param_return);

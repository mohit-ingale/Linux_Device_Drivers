/*
 * This program implements the basic functionalities of the a char driver.....................
 *
 * Santosh Sam Koshy
 * Member Technical Staff
 * C-DAC, Hyderabad
 *
 */

//Initialization Functions...........
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>

#define NAME SAM //Create a Device Number that can be used by the applications as well....
#define SUCCESS 1
#define DEBOUNCE_TIME 200
static unsigned int gpioLED = 49;
static unsigned int gpioSwitch = 115;
static bool gpioVal = 0;

unsigned long IRQflags = IRQF_TRIGGER_RISING;
static int irqNumber, numberPresses;

MODULE_LICENSE("GPL");

static irq_handler_t  MyInterruptHandler (unsigned int irq, void *dev_id, struct pt_regs *regs);

//Init Module
static int __init CharDevice_init(void)
{
	int result;

	if (!gpio_is_valid(gpioLED)){
      		printk(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
      		return -ENODEV;
  }
	gpio_request(gpioLED, "sysfs");          // gpioLED is hardcoded to 49, request it
	gpio_direction_output(gpioLED, 1);   // Set the gpio to be in output mode and on
	// gpio_set_value(gpioLED, ledOn);          // Not required as set by line above (here for reference)
	gpio_export(gpioLED, false);

	if (!gpio_is_valid(gpioSwitch)){
      		printk(KERN_INFO "GPIO_TEST: invalid Switch GPIO\n");
      		return -ENODEV;
  }
	gpio_request(gpioSwitch, "sysfs");          // gpioLED is hardcoded to 49, request it
	gpio_direction_input(gpioSwitch);   // Set the gpio switch to be in input mode 
	gpio_set_debounce(gpioSwitch, DEBOUNCE_TIME);          // Not required as set by line above (here for reference)
	gpio_export(gpioSwitch, false);

	irqNumber = gpio_to_irq(gpioSwitch);
	//printk(KERN_INFO "EBB Button: The button is mapped to IRQ: %d\n", irqNumber);

   // This next call requests an interrupt line
	result = request_irq(irqNumber,             // The interrupt number requested
												(irq_handler_t) MyInterruptHandler, // The pointer to the handler function below
												IRQflags,              // Use the custom kernel param to set interrupt type
												"LED-SWITCH HANDLER",  // Used in /proc/interrupts to identify the owner
												NULL);                 // The *dev_id for shared interrupt lines, NULL is okay

	return 0;
}


//Cleanup Module
void __exit CharDevice_exit(void)
{
	free_irq(irqNumber, NULL);
  gpio_unexport(gpioLED);
	gpio_unexport(gpioSwitch);

	printk(KERN_ALERT "\nI have unregistered the stuff that was allocated.....Goodbye for ever.....\n");
	return;
}

static irq_handler_t MyInterruptHandler (unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	gpioVal = !gpioVal;                      // Invert the LED state on each button press
	gpio_set_value(gpioLED, gpioVal);      // Set the physical LED accordingly
	//getnstimeofday(&ts_current);         // Get the current time as ts_current
	//ts_diff = timespec_sub(ts_current, ts_last);   // Determine the time difference between last 2 presses
	//ts_last = ts_current;                // Store the current time as the last time ts_last
	printk(KERN_INFO "LED-SWITCH: The button state is currently: %d\n", gpio_get_value(gpioSwitch));
	numberPresses++;                     // Global counter, will be outputted when the module is unloaded
	return (irq_handler_t) IRQ_HANDLED;  // Announce that the IRQ has been handled correctly
}
 

	
//Module over ride functions
module_init(CharDevice_init);
module_exit(CharDevice_exit);

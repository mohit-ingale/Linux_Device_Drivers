/*
 =================================================================================================
 Driver Name : DEFERRED_KTIMERS
 Author      : CDAC
 Version     :
 License	 : GPL
 Description : Sample driver code demonstrating the concept of kernel timers.
   	   	   	   The driver code begins by initialising the timer variable, with init_timer().
			   Next the timer variable is filled with appropriate values and finally the
 	 	 	   timer is registered with the kernel by add_timer().
 	 	 	   Before the module is unloaded, the timer is deleted from the kernel by del_timer()
 ================================================================================================
 */

#include"deferred_ktimers.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");

/* Declare a timer variable */
struct timer_list ktimer;

/* Function executed once the timer is fired */
void sample_timer(unsigned long data) {

	PINFO("Inside the timer function, with data = %lu\n", data);

	/* To make the timer periodic, uncomment the following line */
//	add_timer(&ktimer);
}

/* Initialisation function */
static int __init deferred_ktimers_init(void)
{
	PINFO("In init() function\n");

	/* Initialise the timer */
	init_timer(&ktimer);

	/* Setup the timer parameters */
	ktimer.data = 100;					/* Set a data(or address) to be accessed in the timer function 	*/
	ktimer.function = sample_timer;		/* Set the function to be called once the timer expires 		*/
	ktimer.expires = jiffies + 100;		/* Set the delay value relative to jiffies 						*/

	/* Register the timer with the kernel */
	add_timer(&ktimer);

	PINFO("Timer added with an delay of 100ms \n");

	return 0;
}

/* Cleanup function */
static void __exit deferred_ktimers_exit(void)
{
	PINFO("In exit() function\n");

	/* Delete the timer entry from the kernel */
	del_timer(&ktimer);
}

module_init(deferred_ktimers_init)
module_exit(deferred_ktimers_exit)

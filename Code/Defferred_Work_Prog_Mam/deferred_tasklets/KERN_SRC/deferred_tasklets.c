/*
 ============================================================================
 Driver Name : DEFERRED_TASKLETS
 Author      : CDAC
 Version     :
 License	 : GPL
 Description : Sample driver code demonstrating the concept of tasklets.
 	 	 	   The driver loads by initialising the tasklet by tasklet_init()
 	 	 	   and scheduled using tasklet_schedule(). Thus the tasklet gets
 	 	 	   executed at a specific moment as chosen by the kernel(almost
 	 	 	   immediately).
 	 	 	   Finally, before the module unloads, the tasklet is destroyed,
 	 	 	   by calling tasklet_kill().
 ============================================================================
 */

#include"deferred_tasklets.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");

/* Declare a variable of type 'tasklet_struct' */
struct tasklet_struct tasklet;

/*	Tasklet function, executed when the tasklet is scheduled */
void tasklet_func(unsigned long data)
{
	PINFO("Tasklet executing \n");
}

/* Initialisation function */
static int __init deferred_tasklets_init(void)
{
	PINFO("In init() function\n");

	/* Initialise the tasklet */
	tasklet_init(&tasklet,			/* Tasklet variable 								*/
			       tasklet_func,	/* Tasklet function 								*/
			       10); 			/* Value(or address) to be passed to the tasklet 	*/

	/* Schedule the tasklet to run */
	tasklet_schedule(&tasklet);

	return 0;
}

/* Cleanup function */
static void __exit deferred_tasklets_exit(void)
{
	PINFO("In exit() function\n");

	/* Destroy the tasklet */
	tasklet_kill(&tasklet);
}

module_init(deferred_tasklets_init)
module_exit(deferred_tasklets_exit)

/*
 =============================================================================
 Driver Name : DEFERRED_SHAREDQUEUES
 Author      : CDAC
 Version     :
 License	 : GPL
 Description : Sample driver code demonstrating the concept of Shared queues.
 	 	 	   The driver loads by first initialising the work by calling
 	 	 	   INIT_WORK() and then the corresponding function is scheduled
 	 	 	   to the shared queue using schedule_work().
 =============================================================================
 */
#include"deferred_sharedqueues.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");

/*
 * Declare a variable of type 'struct work_struct', which specifies the parameters
 * of a particular work that has to be added to the shared queue
 */
struct work_struct work;

/* Shared Queue Function */
void shrq_func(struct work_struct *work)
{
	PINFO("Shared queue's work executing \n");
}

/* Initialisation function */
static int __init deferred_sharedqueues_init(void)
{
	PINFO("In init() function\n");

	/* Initialise the work */
	INIT_WORK(&work, shrq_func);

	/* Schedule the work */
	schedule_work(&work);

	return 0;
}

/* Cleanup function */
static void __exit deferred_sharedqueues_exit(void)
{
	PINFO("In exit() function\n");
}

module_init(deferred_sharedqueues_init)
module_exit(deferred_sharedqueues_exit)

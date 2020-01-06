Readme - Shared Queues
======================

Concept
-------
	By default the kernel provides a queue to which your works can be queued. But if you are using this queue,
you must be aware that this queue is shared with many others. This means that you should not monopolise the 
queue for queue for long periods of time(no long sleeps), and it may take longer for your tasks to get their 
turn in the processor.

Syntax & Semantics
------------------
	The shared queue lies on 'struct work_struct', defined in <linux/workqueue.h>.
	
	First create and initialise the work to be submitted :
	struct work_struct work;
	INIT_WORK(&work, shrq_func);
	
	Now add the work to the work, i.e just schedule the work :
	schedule_work(&work);
	
	
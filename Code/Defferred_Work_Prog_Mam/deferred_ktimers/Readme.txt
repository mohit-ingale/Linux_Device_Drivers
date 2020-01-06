Readme - Kernel Timers
======================

Concept
-------
	The kernel timers are used to schedule execution of a function at a particular time in the future,
based on the clock tick, and can be used for a variety of tasks; for example, polling a device by 
checking its state at regular intervals when the hardware can't fire interrupts.

	A kernel timer is a data structure that instructs the kernel to execute a user-defined function with
a user-defined function with a user-defined time. They are run asynchronously. When a timer runs, the process
that scheduled it could be asleep, executing on a different processor or quite probably exited altogether.

	The kernel timers run as the result of a 'software interrupt'. When running in this sort of atomic context,
the code is subject to a number of constraints :
	-- No access to user space is allowed
	-- The 'current' pointer is not meaningful
	-- No sleeping or scheduling is performed.
	
	Another important feature of timers that should not be forgotten, though, is that they are a potential 
source of race conditions, even on uniprocessor systems. This is a direct result of their being asynchronous. 
Therefore any data structures should be protected by either by being atomic types or by using spinlocks.

Syntax & Semantics
------------------
	The foundation of the kernel timers is the structure 'struct timer_list' defined in <linux/timer.h> :
	struct timer_list {
		unsigned long expires;
		void (*function)(unsigned long);
		unsigned long data;
			...
	};
	
	First, declare and initialise the timer :
	struct timer_list ktimer;
	init_timer(&ktimer);
	
	Next setup the environment for the timer
	ktimer.data = 10;
	ktimer.function = sample_timer;
	ktimer.expires = jiffies + 100 	// Dealy of 100ms
	
	Finally, link the timer to the kernel
	add_timer(&ktimer);
	
	The timer function looks something like this :
	void sample_timer(unsigned long data)
	{
		/* Your code here */
	}
	
	If you wish the timer to be periodical, add add_timer(&ktimer) to the last line of the
timer function.
	
	Before the module is unloaded, delete the timer using :
	del_timer(&ktimer);
	
	Kernel exports many other APIs such as : mod_timer(), timer_pending() etc.
	
	  
	
	
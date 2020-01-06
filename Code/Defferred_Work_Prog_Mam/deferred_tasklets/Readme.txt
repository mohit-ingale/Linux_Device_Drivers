Readme - Tasklets
=================

Concept
-------
	One of the kernel facility related to timing issues is the tasklet mechanism. It is mostly used
in interrupt management. Unlike kernel timers, you can't ask to execute the function at a specific 
time. By scheduling a tasklet, you simply ask for it to be executed at a later time chosen by the 
kernel. This behaviour is especially useful with interrupt handlers, where the hardware interrupt must
be managed as quickly as possible, but most of the data management can be safely delayed to a later
time. Actually, a tasklet, just like a kernel timer, is executed (in atomic mode) in the context of a
“soft interrupt,” a kernel mechanism that executes asynchronous tasks with hardware interrupts enabled.

	Tasklets offer a number of interesting features :
		-- A tasklet can be disabled and re-enabled later.
		-- Just like timers, a tasklet can register itself.
		-- A tasklet can be scheduled to execute at a normal or higher priority.
		-- The same tasklet never runs simultaneously on more than one processor.
		-- Tasklets usually runs on the processor that scheduled it.

Syntax & Semantics
------------------
	At the base of the tasklet lies, 'struct tasklet_struct', defined in <linux/interrupt.h> :
	struct tasklet_struct {
		void (*func)(unsigned long);
		unsigned long data;
	};
	
	First, declare & initialise the tasklet :
	struct tasklet_struct tasklet;
	tasklet_init(&tasklet, tasklet_func, 100);
	
	At a later point in the code, when the moment has arised, schedule the tasklet :
	tasklet_schedule(&tasklet);
	
	Finally, before unloading the module, or when the tasklet is no longer needed :
	tasklet_kill(&tasklet);

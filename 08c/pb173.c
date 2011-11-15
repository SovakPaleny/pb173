#include <linux/io.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static void pb173_tasklet_fn(unsigned long data);

static DECLARE_TASKLET(pb173_tasklet, pb173_tasklet_fn, 0);

static void pb173_tasklet_fn(unsigned long data)
{
	printk(KERN_DEBUG "tasklet");
	dump_stack();
}

static int my_init(void)
{
	tasklet_schedule(&pb173_tasklet);
	
	return 0;
}

static void my_exit(void)
{

	tasklet_kill(&pb173_tasklet);

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

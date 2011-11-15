#include <linux/io.h>
#include <linux/module.h>
#include <linux/timer.h>

static void pb173_on_time(unsigned long data);

static DEFINE_TIMER(pb173_timer, pb173_on_time, 0, 30);

static void pb173_on_time(unsigned long data)
{
	printk(KERN_DEBUG "tick HZ: %i jiffies: %lu", HZ, jiffies);
	mod_timer(&pb173_timer, jiffies + msecs_to_jiffies(1000));
}

static int my_init(void)
{
	mod_timer(&pb173_timer, jiffies + msecs_to_jiffies(1000));

	return 0;
}

static void my_exit(void)
{
	del_timer_sync(&pb173_timer);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

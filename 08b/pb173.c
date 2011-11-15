#include <linux/io.h>
#include <linux/module.h>
#include <linux/workqueue.h>

static void pb173_work_fn(struct work_struct *work);

static DECLARE_WORK(pb173_work, pb173_work_fn);

static struct workqueue_struct *pb173_workqueue;

static void pb173_work_fn(struct work_struct *work)
{
	printk(KERN_DEBUG "load module xfs");
	request_module("xfs");
}

static int my_init(void)
{
	pb173_workqueue = create_workqueue("pb173_workqueue");
	queue_work(pb173_workqueue, &pb173_work);
	return 0;
}

static void my_exit(void)
{
	cancel_work_sync(&pb173_work);
	destroy_workqueue(pb173_workqueue);

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

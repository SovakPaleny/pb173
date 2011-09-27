#include <linux/module.h>
#include <linux/slab.h>

static int my_init(void)
{
	printk(KERN_INFO "Hello world");
	return 0;
}

static void my_exit(void)
{
	void* mem = kmalloc(1000, GFP_KERNEL);
	if (mem) {
		strcpy(mem, "bye");
		printk(KERN_INFO "%s", mem);
	} else {
		printk(KERN_INFO "nelze alokovat");
	}
	kfree(mem);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

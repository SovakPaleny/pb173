#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>

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
		
		printk(KERN_INFO "kmalloc: %pS", mem);
		char a = 'A';
		printk(KERN_INFO "stack: %p", &a);
		printk(KERN_INFO "jiffies: %p", &jiffies);
		printk(KERN_INFO "function: %p", my_init);
		printk(KERN_INFO "function2: %p", bus_register);
		printk(KERN_INFO "function2: %pF", __builtin_return_address(0));
	} else {
		printk(KERN_INFO "nelze alokovat");
	}
	kfree(mem);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

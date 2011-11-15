#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/delay.h>

static int my_init(void)
{
	void** memory = kmalloc(4000* sizeof(void*), GFP_KERNEL);
	int i;
	int memory_max = 0;
	for (i = 0; i < 10; i++, memory_max++) {
		void* virt = (char *) __get_free_pages(GFP_ATOMIC, 10);
		if (virt == NULL) {
			printk(KERN_INFO "pb173 max1: %i", i);
			break;
		}
		memory[memory_max] = virt;
	}

	msleep(5000);

	for (i = 0; i < 10; i++, memory_max++) {
		void* virt = (char *) __get_free_pages(GFP_ATOMIC, 10);
		if (virt == NULL) {
			printk(KERN_INFO "pb173 max2: %i", i);
			break;
		}
		memory[memory_max] = virt;
	}
	
	for (i = 0; i < 10; i++, memory_max++) {
		void* virt = (char *) __get_free_pages(GFP_KERNEL, 10);
		if (virt == NULL) {
			printk(KERN_INFO "pb173 max3: %i", i);
			break;
		}
		memory[memory_max] = virt;
	}

	for (i = 0; i < memory_max; i++) {
		free_page((unsigned long)memory[i]);
	}
	memory_max = 0;

	for (i = 0; i < 10; i++, memory_max++) {
		void* virt = (char *) __get_free_pages(GFP_KERNEL, 10);
		if (virt == NULL) {
			printk(KERN_INFO "pb173 max4: %i", i);
			break;
		}
		memory[memory_max] = virt;
	}


	for (i = 0; i < memory_max; i++) {
		free_page((unsigned long)memory[i]);
	}
	memory_max = 0;
	
	kfree(memory);

	return -EIO;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

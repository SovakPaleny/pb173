#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/io.h>

static int my_init(void)
{
	phys_addr_t phys = 0x000000001fff0470;
	u32 *virt = ioremap(phys, 8);
	printk(KERN_INFO "pb173: sig: %s len: %x", &virt[0], virt[1]);
	iounmap(virt);
	return -EIO;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

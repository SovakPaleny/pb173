#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/gfp.h>

static int my_init(void)
{
	char* virt = (char *) __get_free_page(GFP_KERNEL);
	strcpy(virt, "ahoj");
	phys_addr_t phys = virt_to_phys(virt);
	struct page* page = virt_to_page(virt);
	SetPageReserved(page);
	u32 *map = ioremap(phys, PAGE_SIZE);
	unsigned long pfn = page_to_pfn(page);
	
	printk(KERN_INFO "pb173: virt: %p", virt);
	printk(KERN_INFO "pb173: phys: %llx", phys);
	printk(KERN_INFO "pb173: page: %p", page);
	printk(KERN_INFO "pb173: map: %p", map);
	printk(KERN_INFO "pb173: page_to_pfn: %lx", pfn);
	printk(KERN_INFO "pb173: obsah virt: %s", virt);
	printk(KERN_INFO "pb173: obsah map: %s", map);
	
	iounmap(virt);
	ClearPageReserved(page);
	free_page((unsigned long)virt);
	return -EIO;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/atomic.h>
//#include <asm/bitmaps.h>
#include <asm/bitops.h>

static int my_init(void)
{
/*
	atomic_t a = ATOMIC_INIT(-3);
	int a1 = atomic_inc_return(&a);
	printk(KERN_INFO "pb173 +1: %i", a1);
	
	atomic_add(3, &a);
	atomic_sub(1, &a);
	return atomic_read(&a);
*/
	char buff[100];
	unsigned long bit;
	DECLARE_BITMAP(pole, 100);
	bitmap_zero(pole, 100);
	set_bit(2, pole);
	set_bit(63, pole);
	set_bit(76, pole);
	printk(KERN_INFO "pb173: %lx", pole[BIT_WORD(63)]);
	bitmap_scnprintf(buff, 100, pole, 100);
	printk(KERN_INFO "pb173: %s", buff);
	for_each_set_bit(bit, pole, 100) {
	    printk(KERN_INFO "pb173: %lu", bit);
	}
	
	printk(KERN_INFO "pb173: prvni bit: %lu", find_first_bit(pole, 100));

	
	return -EIO;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

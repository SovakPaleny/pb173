#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

static int pb173_pocet = 4;

#define PB173_IOCTL_GET _IOR('u', 20, int)
#define PB173_IOCTL_SET _IOW('u', 21, int)

static int pb173_open(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG "pb173_open");
	return 0;
}

static int pb173_release (struct inode * inode, struct file * file)
{
	printk(KERN_DEBUG "pb173_release");
	return 0;
}

static ssize_t pb173_write (struct file *file, const char __user *user, size_t count, loff_t *off)
{
	char buff[100];
	int max_cnt = min(count, (size_t) 100);
	int cnt = copy_from_user(buff, user, max_cnt);
	if (cnt == 0) {
		buff[max_cnt] = 0;
		printk(KERN_DEBUG "pb173_write: %s", buff);
		return max_cnt;
	} else {
		return -EFAULT;
	}
}

static ssize_t pb173_read (struct file *file, char __user *user, size_t count, loff_t *off)
{
	char* text = "Ahoj";
	int cnt_max = min(pb173_pocet, min(strlen(text), count));
	int cnt = copy_to_user(user, text, cnt_max);
	if (cnt == 0) {
		return strlen(text);
	} else  {
		return -EFAULT;
	}
}

static long pb173_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int pocet;
	switch (cmd) {
	case PB173_IOCTL_GET:
		printk(KERN_INFO "pb173: ioctl get");
		copy_to_user((int *)arg, &pb173_pocet, sizeof(pb173_pocet));
		return 0;
	case PB173_IOCTL_SET:
		printk(KERN_INFO "pb173: ioctl set");
		printk(KERN_INFO "pb173: ioctl arg: %lu", arg);
		pocet = (int)arg;
		if (pocet >= 5 || pocet <= 0) {
			printk(KERN_INFO "pb173: parametr neni v rozsahu (0,5>: %i", pocet);
			return -EFAULT;
		} else {
			pb173_pocet = pocet;
		}
		return 0;
	}
	
	return -EFAULT;
}

static struct file_operations pb173_fops = {
	.owner = THIS_MODULE,
	.open = pb173_open,
	.release = pb173_release,
	.read = pb173_read,
	.write = pb173_write,
	.unlocked_ioctl = pb173_unlocked_ioctl,
};

static struct miscdevice pb173_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pb173",
	.fops = &pb173_fops,
};

static int pb173_init(void)
{
	printk(KERN_DEBUG "pb173_init");
	printk(KERN_DEBUG "pb173 PB173_IOCTL_GET: %lu", PB173_IOCTL_GET);
	printk(KERN_DEBUG "pb173 PB173_IOCTL_SET: %lu", PB173_IOCTL_SET);
	misc_register(&pb173_misc);
	return 0;
}

static void pb173_exit(void)
{
	printk(KERN_DEBUG "pb173_exit");
	misc_deregister(&pb173_misc);
}


module_init(pb173_init);
module_exit(pb173_exit);

MODULE_LICENSE("GPL");

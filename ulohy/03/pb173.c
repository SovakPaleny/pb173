#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

static char buffer[128];
static size_t size = 0;
DEFINE_MUTEX(pb173_mutex);

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
/*
	char buff[100];
	int max_cnt = min(count, 100);
	int cnt = copy_from_user(buff, user, max_cnt);
	if (cnt == 0) {
		buff[max_cnt] = 0;
		printk(KERN_DEBUG "pb173_write: %s", buff);
		return max_cnt;
	} else {
		return -EFAULT;
	}
*/
	char usrbuff[5];
	int i;
	size_t max_cnt;
	
	mutex_lock(&pb173_mutex);
	
	max_cnt = min(count, min(5, 128 - size));
	if (max_cnt <= 0) {
		mutex_unlock(&pb173_mutex);
		printk(KERN_INFO "pb172: k zapsani 0 bajtu");
		return -EFAULT;
	}

	if (size + max_cnt > 128) {
		mutex_unlock(&pb173_mutex);
		printk(KERN_INFO "pb172: zapsano maximum, nelze dale zapisovat");
		return -EFAULT;
	}
	
	int copy = copy_from_user(usrbuff, user, max_cnt);
	if (0 != copy) {
		mutex_unlock(&pb173_mutex);
		printk(KERN_INFO "pb173: nelze kopirovat od uzivatele: %i", copy);
		return -EFAULT;
	}

	for (i = 0; i < max_cnt; i++) {
		buffer[i + *off] = usrbuff[i];
		msleep(100);
	}
	*off += max_cnt;
	size = *off;

	mutex_unlock(&pb173_mutex);
	
	msleep(100);

	return max_cnt;
}

static ssize_t pb173_read (struct file *file, char __user *user, size_t count, loff_t *off)
{
	mutex_lock(&pb173_mutex);
	
	size_t max_cnt = min(size - *off, count);
	int cnt = copy_to_user(user, buffer + *off, max_cnt);
	
	mutex_unlock(&pb173_mutex);
	
	*off += max_cnt;	
	
	if (cnt == 0) {
		return max_cnt;
	} else  {
		return -EFAULT;
	}
}

static struct file_operations pb173_fops_r = {
	.owner = THIS_MODULE,
	.open = pb173_open,
	.release = pb173_release,
	.read = pb173_read,
};

static struct file_operations pb173_fops_w = {
	.owner = THIS_MODULE,
	.open = pb173_open,
	.release = pb173_release,
	.write = pb173_write,
};

static struct miscdevice pb173_misc_r = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pb173r",
	.fops = &pb173_fops_r,
};

static struct miscdevice pb173_misc_w = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pb173w",
	.fops = &pb173_fops_w,
};

static int pb173_init(void)
{
	printk(KERN_DEBUG "pb173_init");
	misc_register(&pb173_misc_r);
	misc_register(&pb173_misc_w);
	return 0;
}

static void pb173_exit(void)
{
	printk(KERN_DEBUG "pb173_exit");
	misc_deregister(&pb173_misc_r);
	misc_deregister(&pb173_misc_w);
}


module_init(pb173_init);
module_exit(pb173_exit);

MODULE_LICENSE("GPL");

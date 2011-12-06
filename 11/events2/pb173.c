#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>

static DEFINE_MUTEX(my_lock);
static char str[10];
static DECLARE_WAIT_QUEUE_HEAD(my_wait);

static ssize_t my_read(struct file *filp, char __user *buf, size_t count,
		loff_t *off)
{
	ssize_t ret;
	
	mutex_lock(&my_lock);
	int len = strlen(str);
	mutex_unlock(&my_lock);
	
	if (*off < len || len == 0) {
		ret = wait_event_interruptible_exclusive(my_wait, ({
			mutex_lock(&my_lock);
			len = strlen(str);
			mutex_unlock(&my_lock);
			len;
		}) > 5);
		if (ret < 0) {
			return ret;
		}

/*
		mutex_lock(&my_lock);
		strcpy(str, "");
		mutex_unlock(&my_lock);
*/
	}

	mutex_lock(&my_lock);
	ret = simple_read_from_buffer(buf, count, off, str, strlen(str));
	mutex_unlock(&my_lock);

	return ret;
}

static ssize_t my_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *off)
{
	ssize_t ret = count;

	mutex_lock(&my_lock);
	if (copy_from_user(str, buf, min_t(size_t, count, sizeof str - 1)))
		ret = -EFAULT;

	str[sizeof str - 1] = 0;
	mutex_unlock(&my_lock);

	wake_up(&my_wait);

	return ret;
}

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static struct miscdevice my_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &my_fops,
	.name = "my_name",
};

static int my_init(void)
{
	return misc_register(&my_misc);
}

static void my_exit(void)
{
	misc_deregister(&my_misc);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

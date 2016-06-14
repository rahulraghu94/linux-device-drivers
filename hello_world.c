#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DRIVER_VERSION ""
#define DRIVER_DESC "Simple char device"
#define WRITE_SUCCESS 0
#define DEVICE_NAME "eudyptula"
#define BUFFER_LEN 80
#define CHALLENGE_ID "60fac94fefeb"
#define CHALLENGE_SIZE 12

static char msg[BUFFER_LEN] = {0};
static char *id = CHALLENGE_ID;
static struct miscdevice my_dev;

module_init(init_module);
module_exit(cleanup_module);

static ssize_t my_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	return simple_read_from_buffer(buffer, length, offset, id,
			strlen(id));
}

static ssize_t my_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	/*
	if(!simple_write_to_buffer(msg, BUFFER_LEN, offset, buffer, length))
		return -EINVAL;

	pr_info("Size is %d\n", (int)length);
	if (!strncmp(msg, CHALLENGE_ID, CHALLENGE_SIZE) && (length  == CHALLENGE_SIZE))
		return length;

	return -EINVAL;
	*/

		int max, bytes_to_write;

	char *kbuff = kmalloc(len, GFP_KERNEL);
	if (!kbuff)
		return -ENOMEM;

	max = len - *f_ops;

	if (max > count)
		bytes_to_write = count;
	else
		bytes_to_write = max;

	if (copy_from_user(kbuff + *f_ops, buf, bytes_to_write)) {
		pr_debug("Failed to write to the char driver\n");
		kfree(kbuff);
		return -EFAULT;
	}

	*f_ops += bytes_to_write;
	if (memcmp(kbuff, text, len - 1) != 0) {
		pr_debug("Invalid value\n");
		kfree(kbuff);
		return -EINVAL;
	}

	kfree(kbuff);
	return count;

}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static int init_module(void)
{
	static int retval;

	my_dev.minor = MISC_DYNAMIC_MINOR;
	my_dev.name = DEVICE_NAME;
	my_dev.fops = &fops;
	retval = misc_register(&my_dev);

	if (retval) {
		pr_debug("Error: %i", retval);
		return retval;
	}

	pr_debug("Minor: %i\n", my_dev.minor);

	return 0;
}

static void cleanup_module(void)
{
	misc_deregister(&my_dev);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rahul");

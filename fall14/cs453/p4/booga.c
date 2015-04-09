/**
 * Author: Aaron Woods
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/kernel.h> /* printk() */
#include <linux/version.h> /* printk() */
#include <linux/init.h>  /*  for module_init and module_cleanup */
#include <linux/slab.h>  /*  for kmalloc/kfree */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>	/* for the proc filesystem */
#include <linux/seq_file.h>
#include <linux/random.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include "booga.h"

MODULE_AUTHOR("Aaron Woods");
MODULE_LICENSE("GPL");

// Global Variables
static int booga_major = BOOGA_MAJOR;
static int booga_nr_devs = BOOGA_NR_DEVS;    /* number of bare example devices */
module_param(booga_major, int, 0);
module_param(booga_nr_devs, int, 0);

static booga_stats *booga_device_stats;
static long int boogaStats[4];
static long int strCount[4];


static booga_dev *booga_device;

static struct proc_dir_entry *booga_proc_file;

// Function Prototypes

/**
 * This function is used to open the booga device.  It extracts
 * the minor number that will be used, and updates a counter for
 * the number of times the device has been opened.  This is
 * done inside of a semaphore, so it is thread-safe.
 */
static int booga_open(struct inode *inode, struct file *filp);

/**
 * Releases control of the device after its use has been completed.
 */
static int booga_release(struct inode *inode, struct file *filp);

/**
 * Reads from the minor number of the device, that was determined in the open function.
 */
static ssize_t booga_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

/**
 * Writes to the minor number of the device, that was determined in the open function.
 */
static ssize_t booga_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

/**
 * Initializes the device, assigns values to all the device variables, as well
 * as the global variables that are needed.  Also registers the device major number,
 * and creates a file for all the proc information.
 */
static int __init booga_init(void);

/**
 * Unregisters the device major number, and deletes the proc file that was created in
 * the __init function.
 */
static void __exit booga_exit(void);

/**
 * Initializes all of the devices stats for the booga driver.
 */
static void init_booga_device_stats(void);

/**
 * Prints the driver stat information to /proc/driver/booga
 */
static int booga_proc_show(struct seq_file *m, void *v);

/**
 * Opens the /proc/driver directory, and calls the booga_proc_show function.
 */
static int booga_proc_open(struct inode *inode, struct file *filp);

static struct file_operations booga_fops = {
	.read = booga_read,
	.write = booga_write,
	.unlocked_ioctl = NULL,
	.open = booga_open,
	.release = booga_release,
};

static int booga_open(struct inode *inode, struct file *filp)
{
	booga_device->number = NUM(inode->i_rdev);

	if(booga_device->number >= booga_nr_devs)
	{
		return -ENODEV;
	}

	filp->f_op = &booga_fops;

	if(down_interruptible(&booga_device_stats->sem))
	{
		return -ERESTARTSYS;
	}

	booga_device_stats->num_open++;
	boogaStats[booga_device->number]++;
	up(&booga_device_stats->sem);

	try_module_get(THIS_MODULE);
	
	printk("<1> /dev/booga%d opened.\n", booga_device->number);

	return 0;
}

static int booga_release(struct inode *inode, struct file *filp)
{
	if(down_interruptible(&booga_device_stats->sem))
	{
		return -ERESTARTSYS;
	}

	booga_device_stats->num_close++;

	up(&booga_device_stats->sem);

	printk("<1> /dev/booga%d released.\n", booga_device->number);

	module_put(THIS_MODULE);
	
	return 0;
}

static ssize_t booga_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int result;
	int i;
	int status;
	int choice;
	int messageLen;
	int index;
	char randVal;
	char* message;

	get_random_bytes(&randVal, 1);

	choice = (randVal & 0x7F) % 4;
	
	switch(choice)
	{
		case 0: message = "booga! booga! ";
				break;

		case 1: message = "googoo! gaga! ";
				break;

		case 2: message = "neka! maka! ";
				break;

		case 3: message = "wooga! wooga! ";
				break;

		default: message = "modulus divison no longer works, the end is near.";
				 break;
	}

	strCount[choice]++;

	printk("<1> booga_read invoked on /dev/booga%d.\n", booga_device->number);

	messageLen = strlen(message);

	booga_device->data = (char *) kmalloc(sizeof(char) * count, GFP_KERNEL);
	if(!booga_device->data)
	{
		result = -ENOMEM;
		goto fail_malloc;
	}

	printk("<1> booga_read malloced a data buffer of size %ld.\n", count);
	
	for(i = 0; i < count; i++)
	{
		index = i % messageLen;
		booga_device->data[i] = message[index];
	}
	
	printk("<1> finished copying to device.\n");

	status = __copy_to_user(buf, booga_device->data, count);

	if(status > 0)
	{
		printk("<1> booga: Could not copy %d bytes.\n", status);
	}

	if(down_interruptible(&booga_device_stats->sem))
	{
		return -ERESTARTSYS;
	}
	
	booga_device_stats->bytes_read += count;
	booga_device_stats->num_read++;

	if(booga_device->data)
	{
		kfree(booga_device->data);
	}
	
	printk("<1> booga_read freed a data buffer of size %ld.\n", count);

	up(&booga_device_stats->sem);

	return count;

	fail_malloc:
		return result;
}

static ssize_t booga_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	struct task_struct *task = current;

	if(booga_device->number == 3)
	{
		printk(KERN_WARNING "Tried to write to /dev/booga3.\n");
		
		send_sig(SIGTERM, task, 0);
	}

	printk("<1> booga_write invoked on /dev/booga%d.\n", booga_device->number);

	if(down_interruptible(&booga_device_stats->sem))
	{
		return -ERESTARTSYS;
	}

	booga_device_stats->bytes_written += count;
	booga_device_stats->num_write++;

	up(&booga_device_stats->sem);

	return count;
}

static const struct file_operations booga_proc_fops = {
	.owner = THIS_MODULE,
	.open = booga_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init booga_init(void)
{
	int result;
	int i;

	result = register_chrdev(booga_major, "booga", &booga_fops);

	if(result < 0)
	{
		printk(KERN_WARNING "booga: can't get major number %d.\n", booga_major);
		return result;
	}

	if(booga_major == 0)
	{
		booga_major = result;
	}

	printk("<1> booga device driver: loaded at major number %d.\n", booga_major);

	
	booga_device_stats = (booga_stats*) kmalloc(sizeof(booga_stats), GFP_KERNEL);
	if(!booga_device_stats)
	{
		result = -ENOMEM;
		goto fail_malloc;
	}

	init_booga_device_stats();

	booga_device = (struct booga_dev*) kmalloc(sizeof(booga_dev), GFP_KERNEL);

	if(!booga_device)
	{
		result = -ENOMEM;
		goto fail_malloc;
	}

	proc_mkdir("driver", NULL);

	booga_proc_file = proc_create("driver/booga", 0, NULL, &booga_proc_fops);
	if(!booga_proc_file)
	{
		result = -ENOMEM;
		goto fail_malloc;
	}

	for(i = 0; i < 4; i++)
	{
		boogaStats[i] = 0;
	}

	for(i = 0; i < 4; i++)
	{
		strCount[i] = 0;
	}

	return 0;

	fail_malloc:
		unregister_chrdev(booga_major, "booga");
		return result;
}

static void __exit booga_exit(void)
{
	remove_proc_entry("driver/booga", NULL);
	kfree(booga_device_stats);
	unregister_chrdev(booga_major, "booga");
	printk("<1>Booga device driver unloaded.\n");
}

static void init_booga_device_stats(void)
{
	booga_device_stats->num_read = 0;
	booga_device_stats->num_write = 0;
	booga_device_stats->num_open = 0;
	booga_device_stats->num_close = 0;
	booga_device_stats->bytes_read = 0;
	booga_device_stats->bytes_written = 0;
	sema_init(&booga_device_stats->sem, 1);
}

static int booga_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "bytes read = %ld\n", booga_device_stats->bytes_read);
	seq_printf(m, "bytes written = %ld\n", booga_device_stats->bytes_written);
	seq_printf(m, "number of opens:\n");
	seq_printf(m, "\t/dev/booga0 = %ld times\n", boogaStats[0]);
	seq_printf(m, "\t/dev/booga1 = %ld times\n", boogaStats[1]);
	seq_printf(m, "\t/dev/booga2 = %ld times\n", boogaStats[2]);
	seq_printf(m, "\t/dev/booga3 = %ld times\n", boogaStats[3]);
	seq_printf(m, "strings output:\n");
	seq_printf(m, "\tbooga! booga! = %ld times\n", strCount[0]);
	seq_printf(m, "\tgoogoo! gaga! = %ld times\n", strCount[1]);
	seq_printf(m, "\tneka! maka! = %ld times\n", strCount[2]);
	seq_printf(m, "\twooga! wooga! = %ld times\n", strCount[3]);

	return 0;
}

static int booga_proc_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, booga_proc_show, NULL);
}

module_init(booga_init);
module_exit(booga_exit);

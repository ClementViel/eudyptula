/*SPDX-License-Identifier: GPL-2.0*/

#define LINUX
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/capability.h>
#include <linux/debugfs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>

static const char *touser = "dd9412d43b02";
static struct dentry *dir;
static struct dentry *file;
static struct file_operations debugfs_ops;
static unsigned char *page;
static struct semaphore foo_sem;


int init_module(void)
{
	pr_debug("task08 inserted.\n");
	/* create debugfs dir and file */
	dir = debugfs_create_dir("eudyptula", 0);
	if (!dir) {
		pr_alert("could not create debugfs dir\n");
		return -1;
	}

	file = debugfs_create_file("foo", 0644, dir, NULL, &debugfs_ops);
	if (!file) {
		pr_alert("could not create debugfs file\n");
		return -1;
	}
	page = (unsigned char *)__get_free_page(GFP_KERNEL);
	if (!page) {
		pr_alert("page is null\n");
		return -1;
	}

	sema_init(&foo_sem, 1);
	return 0;
}

static ssize_t dbg_read(struct file *file, char *buf,
			size_t len, loff_t *ppos)
{
	int size = strlen(touser);
	/* if buffer size is less than string size, returns error */
	if (len < size)
		return -EINVAL;

	if (*ppos != 0)
		return 0;
	/* acquire lock */
	/* read */
	/* return to user */
	down(&foo_sem);
	if (copy_to_user(buf, page, size)) {
		return -EINVAL;
		up(&foo_sem);
	}
	up(&foo_sem);
	*ppos = size;
	return size;
}

static ssize_t dbg_wrte(struct file *file,
			 char __user *buf, size_t size, loff_t *offp)
{
	int return_value;
	
	down(&foo_sem);
	if (*offp != 0)
		return 0;
	
	if (size <= PAGE_SIZE) {
		return_value = copy_from_user( page, buf, size);
		
	}
	up(&foo_sem);
	return size;
}

void cleanup_module(void)
{
	/* remove debugfs dire and file*/
	free_page((unsigned long *) page);
	debugfs_remove_recursive(dir);
	
	pr_debug("task08 removed.\n");
}

static struct file_operations debugfs_ops = {
	.read = dbg_read,
	.write = dbg_wrte,
};


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Clement Viel");
MODULE_DESCRIPTION("Simple Hellow World kernel module");

/*
 * rd_wr.c -- a module which generates an oops when read
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: rd_wr.c,v 1.3 2004/09/26 07:02:43 gregkh Exp $
 */


//#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/types.h>  /* size_t */
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");


int rd_wr_major = 0;
char stack_buf[16]="i love you, man";
ssize_t rd_wr_read(struct file *filp, char __user *buf,
		    size_t count, loff_t *pos)
{
	int ret;

	if (count > 16)
		count = 16; /* copy 16 bytes to the user */
	ret = copy_to_user(buf, stack_buf, count);
//	if (!ret)
//		return count;
//	return ret;
	return 0;
}

ssize_t rd_wr_write (struct file *filp, const char __user *buf, size_t count,
		loff_t *pos)
{
	int ret;

	if (count > 16)
		count = 16; /* copy 16 bytes to the user */
	ret = copy_from_user(stack_buf, buf,count);
	if (!ret)
		return count;
	return ret;
}



struct file_operations rd_wr_fops = {
	.read =  rd_wr_read,
	.write = rd_wr_write,
	.owner = THIS_MODULE
};


int rd_wr_init(void)
{
	int result;

	/*
	 * Register your major, and accept a dynamic number
	 */
	result = register_chrdev(rd_wr_major, "rd_wr", &rd_wr_fops);
	if (result < 0)
		return result;
	if (rd_wr_major == 0)
		rd_wr_major = result; /* dynamic */

	printk("rd_wr rd_wr_major:0x%x\n", rd_wr_major);

	return 0;
}

void rd_wr_cleanup(void)
{
	unregister_chrdev(rd_wr_major, "rd_wr");
}

module_init(rd_wr_init);
module_exit(rd_wr_cleanup);


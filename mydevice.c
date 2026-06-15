#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydevice"

static int major;
static char message[256] = "Hello from Kernel Device Driver!\n";
static int msg_len = 0;

static int dev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydevice: Device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydevice: Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off) {
    copy_to_user(buf, message, msg_len);
    printk(KERN_INFO "mydevice: Sent %d bytes to user\n", msg_len);
    return msg_len;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *off) {
    copy_from_user(message, buf, len);
    msg_len = len;
    printk(KERN_INFO "mydevice: Received %zu bytes from user\n", len);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

static int __init mydevice_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "mydevice: Failed to register\n");
        return major;
    }
    printk(KERN_INFO "mydevice: Driver Loaded Successfully! Major number = %d\n", major);
    return 0;
}

static void __exit mydevice_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "mydevice: Driver Unloaded\n");
}

module_init(mydevice_init);
module_exit(mydevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Character Device Driver - Portfolio Project");

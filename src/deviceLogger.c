#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artur Weber");
MODULE_DESCRIPTION("Device Monitoring Kernel Module");

static int notify_device(struct notifier_block *self, unsigned long action, void *dev) {
    struct usb_device *usb_dev = (struct usb_device *)dev;

    if (action == USB_DEVICE_ADD) {
        char manufacturer[256], product[256], serial[256], size[16];
        char buf[256];
        int result;

        strcpy(manufacturer, usb_dev->manufacturer);
        strcpy(product, usb_dev->product);
        strcpy(serial, usb_dev->serial);

        printk(KERN_INFO "New USB Device Connected: Manufacturer: %s, Product: %s, Serial Number: %s\n",
            manufacturer, product, serial);
    }

    return NOTIFY_OK;
}

static struct notifier_block usb_notifier = {
    .notifier_call = notify_device,
};

static int __init usb_notify_init(void) {
    usb_register_notify(&usb_notifier);
    return 0;
}

static void __exit usb_notify_exit(void) {
    usb_unregister_notify(&usb_notifier);
}

module_init(usb_notify_init);
module_exit(usb_notify_exit);

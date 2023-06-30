#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#include "deviceLogger.h"

static int notify_device(struct notifier_block *self, unsigned long action, void *dev) {
    struct usb_device *usb_dev = (struct usb_device *)dev;

    if (action == USB_DEVICE_ADD) {
        char manufacturer[256], product[256], serial[256];

        strcpy(manufacturer, usb_dev->manufacturer);
        strcpy(product, usb_dev->product);
        strcpy(serial, usb_dev->serial);

        printk(KERN_INFO "New USB Device Connected: Manufacturer: %s, Product: %s, Serial Number: %s\n",
            manufacturer, product, serial);
    }

    return NOTIFY_OK;
}

struct notifier_block usb_notifier = {
    .notifier_call = notify_device,
};

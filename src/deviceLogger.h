#ifndef __DEVICELOGGER_H__
#define __DEVICELOGGER_H__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

extern struct notifier_block usb_notifier;

#endif /* __DEVICELOGGER_H__ */
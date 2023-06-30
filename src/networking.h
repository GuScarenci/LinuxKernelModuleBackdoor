#ifndef __NETWORKING_H__
#define __NETWORKING_H__

#define KBD_IQR 1 // Keyboard IRQ number.
#define IP_ADDRESS "127.0.0.1"
#define PORT 9090

int create_socket(char const* ip_address, uint32_t port);
int send_message(char const* message);
int shutdown_socket(void);

#endif /* __NETWORKING_H__ */

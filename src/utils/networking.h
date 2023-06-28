#ifndef __NETWORKING_H__
#define __NETWORKING_H__

int create_socket(char const* ip_address, uint32_t port);
int send_message(char const* message);
int shutdown_socket(void);

#endif /* __NETWORKING_H__ */

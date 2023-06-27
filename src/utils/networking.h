#ifndef __NETWORKING_H__
#define __NETWORKING_H__

struct socket *create_socket(char const* ip_address, uint32_t port);
int send_message(struct socket *sock, char const* message);

#endif /* __NETWORKING_H__ */
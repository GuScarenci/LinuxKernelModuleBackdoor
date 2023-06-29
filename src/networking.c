#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/tcp.h>
#include <linux/socket.h>

#include "networking.h"

static struct socket* sock = NULL;

int create_socket(char const* ip_address, uint32_t port) {
    struct sockaddr_in addr;
    int32_t ret;

    // Create a TCP socket
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        printk(KERN_ERR "Failed to create socket: %d\n", ret);
        return ret;  // return errno
    }

    // Set the destination IP address and port
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = in_aton(ip_address);

    // Connect to the server
    ret = sock->ops->connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in), 0);
    if (ret < 0) {
        printk(KERN_ERR "Failed to connect: %d\n", ret);
        sock_release(sock);
        return ret; // return errno
    }

    return 0;
}


int send_message(char const* message) {
    struct kvec vec;
    struct msghdr msg;
    int ret;

    // Configure the message struct
    memset(&msg, 0, sizeof(msg));
    vec.iov_base = (void *) message;
    vec.iov_len = strlen(message);

    // Send the message
    ret = kernel_sendmsg(sock, &msg, &vec, 1, vec.iov_len);
    if (ret < 0) {
        printk(KERN_ERR "socket_example: error sending message\n");
        return ret;
    }

    return 1;
}


int shutdown_socket(void) {
    sock_release(sock);
    return 0;
}


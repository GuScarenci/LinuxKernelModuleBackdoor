#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/tcp.h>
#include <linux/socket.h>

MODULE_LICENSE("GPL");

static int __init hello_init(void)
{
    struct socket *sock;
    struct sockaddr_in addr;
    struct msghdr msg;
    struct iovec iov;
    int ret;

    // Create a TCP socket
    ret = sock_create_kern(AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        printk(KERN_ERR "Failed to create socket: %d\n", ret);
        return ret;
    }

    // Set the destination IP address and port
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);  // Set the desired port number
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {  // Set the desired IP address
        printk(KERN_ERR "Invalid address\n");
        sock_release(sock);
        return -EINVAL;
    }

    // Connect to the server
    ret = sock->ops->connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in), 0);
    if (ret < 0) {
        printk(KERN_ERR "Failed to connect: %d\n", ret);
        sock_release(sock);
        return ret;
    }

    // Prepare the message to send
    memset(&msg, 0, sizeof(struct msghdr));
    iov.iov_base = "hello";
    iov.iov_len = strlen("hello");
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Send the message
    ret = kernel_sendmsg(sock, &msg, &iov, 1, iov.iov_len);
    if (ret < 0) {
        printk(KERN_ERR "Failed to send message: %d\n", ret);
        sock_release(sock);
        return ret;
    }

    // Close the socket
    sock_release(sock);

    printk(KERN_INFO "Message sent successfully\n");

    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);

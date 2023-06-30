#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/string.h>

static int __init execute_python_init(void){
	printk(KERN_INFO "Executing screenshot...\n");

    	/*char *argv[] = { "/bin/bash", "-c", "gnome-screenshot -f /tmp/screenshot.png", NULL };
    	char *argv[] = { "/bin/bash", "-c", "touch women.txt", NULL };

    	static char *envp[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

    	call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);

    	printk(KERN_INFO "Screenshot executed.\n");
    	*/
    	
    	/*char *argv[] = { "/bin/ls", "-l", NULL };
    	static char *envp[] = {
        "HOME=/",
        "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin",
        NULL
    	};
    	call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    	return 0;*/
    	
    	char *argv[] = { "/bin/bash", "-c", "/usr/bin/ls", NULL };
    	static char *envp[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
	call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
	
	printk(KERN_INFO "Screenshot executed.\n");
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

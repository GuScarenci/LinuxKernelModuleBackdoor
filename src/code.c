#include <linux/module.h>
#include <linux/kernel.h>

/***************Funções de Módulo****************/

MODULE_LICENSE("GPL");

static int segmentsDisplay_init(void)
{
    printk(KERN_ALERT "MÓDULO INICIALIZADO!");
    return 0;
}

static void segmentsDisplay_exit(void)
{
    printk(KERN_ALERT "MÓDULO REMOVIDO!");
}

module_init(segmentsDisplay_init);
module_exit(segmentsDisplay_exit);



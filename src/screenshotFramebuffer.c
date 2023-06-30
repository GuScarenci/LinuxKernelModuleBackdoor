#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/fb.h>

MODULE_LICENSE("GPL");

struct fb_info *fbinfo;
struct file *file;

// Function to capture the screen
int screenshot_ppm(void) {
	struct fb_info *info;
	void *screen_base;
	void *buf;
	long screen_size;
	struct file *file;
    struct file *outfile;
    char header[18];
    loff_t pos;
    u32 *pixel;
    u8 rgb[3];
    int x, y;    
    
    file = filp_open("/dev/fb0", O_RDONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ALERT "filp_open error\n");
        return -1;
    }

    info = file->private_data;
    screen_base = info->screen_base;
    screen_size = info->screen_size;

    // Now screen_base points to the start of the video memory and
    // screen_size contains the size of the video memory.
    // You can copy the contents of the video memory to a buffer.

    buf = kmalloc(screen_size, GFP_KERNEL);
    if (!buf) {
        printk(KERN_ALERT "kmalloc error\n");
        filp_close(file, 0);
        return -1;
    }
    memcpy(buf, screen_base, screen_size);
    // Now buf contains a snapshot of the screen.
    // You can write this data to an image file.
    // The exact procedure depends on the image format you want to use.
    // For simplicity, let's assume you're writing a raw PPM file.
    outfile = filp_open("/home/screenshot.ppm", O_WRONLY | O_CREAT, 0666);
    if (IS_ERR(outfile)) {
        printk(KERN_ALERT "filp_open error\n");
        kfree(buf);
        filp_close(file, 0);
        return -1;
    }
	
	pos = 0;
    sprintf(header, "P6\n%d %d\n255\n", info->var.xres, info->var.yres);
    if (kernel_write(outfile, header, strlen(header), &pos) < 0) {
        printk(KERN_ERR "kernel_write error\n");
        kfree(buf);
        filp_close(outfile, 0);
        filp_close(file, 0);
        return -1;
    }
    pos = strlen(header);
    pixel = (u32 *)buf;
    for (y = 0; y < info->var.yres; y++) {
        for (x = 0; x < info->var.xres; x++) {
            u32 color = *pixel++;
            u8 r, g, b;
            if (info->var.red.length == 8)
                r = (color >> info->var.red.offset) & 0xFF;
            else
                r = (color >> info->var.red.offset) & ((1 << info->var.red.length) - 1);
            if (info->var.green.length == 8)
                g = (color >> info->var.green.offset) & 0xFF;
            else
                g = (color >> info->var.green.offset) & ((1 << info->var.green.length) - 1);
            if (info->var.blue.length == 8)
                b = (color >> info->var.blue.offset) & 0xFF;
            else
                b = (color >> info->var.blue.offset) & ((1 << info->var.blue.length) - 1);
            rgb[0] = r; rgb[1] = g; rgb[2] = b;
            if (kernel_write(outfile, rgb, 3, &pos) < 0) {
                printk(KERN_ERR "kernel_write error\n");
                kfree(buf);
                filp_close(outfile, 0);
                filp_close(file, 0);
                return -1;
            }
        }
    }
    filp_close(outfile, 0);
    kfree(buf);
    filp_close(file, 0);
    return 0;
}



// Module initialization function
static int __init screenshot_module_init(void) {

	printk(KERN_INFO "Taking print...\n");
	
    file = filp_open("/dev/fb0", O_RDONLY, 0);
    if (!file || IS_ERR(file)) {
        pr_err("Failed to open framebuffer device\n");
        return -1;
    }

    screenshot_ppm();

    filp_close(file, NULL);
    
    printk(KERN_INFO "Print taken.\n");

    return 0;
}

// Module exit function
static void __exit screenshot_module_exit(void) {
    // Cleanup code, if needed
}

module_init(screenshot_module_init);
module_exit(screenshot_module_exit);

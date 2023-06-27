#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/fb.h>

//
//https://chat.openai.com/share/322a425c-2bfe-47df-bc1a-fb235fe07e8f
//

MODULE_LICENSE("GPL");

struct fb_info *fbinfo;
struct file *file;

// Function to capture the screen
void capture_screen(void) {
    struct fb_var_screeninfo var_screeninfo;
    unsigned int screen_size;
    char *buffer;

    // Get the framebuffer information
    fbinfo = framebuffer_alloc(0, NULL);
    if (!fbinfo) {
        pr_err("Failed to allocate framebuffer\n");
        return;
    }

    if (fbinfo->flags & FBINFO_FLAG_HWACCEL_DISABLED) {
        pr_err("Hardware acceleration is disabled\n");
        framebuffer_release(fbinfo);
        return;
    }

    // Get the screen information
    if (fb_ioctl(fbinfo, FBIOGET_VSCREENINFO, &var_screeninfo) == -EINVAL) {
        pr_err("Failed to get var_screeninfo\n");
        framebuffer_release(fbinfo);
        return;
    }

    // Calculate the screen size
    screen_size = var_screeninfo.xres_virtual * var_screeninfo.yres_virtual * var_screeninfo.bits_per_pixel / 8;

    // Allocate memory for the buffer
    buffer = vmalloc(screen_size);
    if (!buffer) {
        pr_err("Failed to allocate buffer\n");
        framebuffer_release(fbinfo);
        return;
    }

    // Read the screen data into the buffer
    if (fb_read(fbinfo, buffer, screen_size, &file->f_pos) < 0) {
        pr_err("Failed to read framebuffer\n");
        vfree(buffer);
        framebuffer_release(fbinfo);
        return;
    }

    // Process the captured screen data as needed
    // ...

    // Free the allocated resources
    vfree(buffer);
    framebuffer_release(fbinfo);
}

// Module initialization function
static int __init screenshot_module_init(void) {
    file = filp_open("/dev/fb0", O_RDONLY, 0);
    if (!file || IS_ERR(file)) {
        pr_err("Failed to open framebuffer device\n");
        return -1;
    }

    capture_screen();

    filp_close(file, NULL);

    return 0;
}

// Module exit function
static void __exit screenshot_module_exit(void) {
    // Cleanup code, if needed
}

module_init(screenshot_module_init);
module_exit(screenshot_module_exit);

KDIR                  ?= /lib/modules/$(shell uname -r)/build
CC                     = gcc
MODULE_NAME            = backdoor

SOURCE_DIR             = $(CURDIR)
MODULE_PATH            = $(SOURCE_DIR)/$(MODULE_NAME).ko
UTILS_SOURCE_FILES     = $(wildcard utils/%.c)
UTILS_OBJS             = $(patsubst %.c, %.o, $(UTILS_SOURCE_FILES))

obj-m                 += $(MODULE_NAME).o
$(MODULE_NAME)-objs   += $(UTILS_OBJS)

.PHONY: all clean module insert remove

all: module

insert:
	sudo insmod $(MODULE_PATH)

remove:
	sudo rmmod $(MODULE_PATH)

clean:
	sudo $(MAKE) --debug -C $(KDIR) M=$(SOURCE_DIR) clean

module:
	sudo $(MAKE) --debug CC=$(CC) -C $(KDIR) M=$(SOURCE_DIR) modules

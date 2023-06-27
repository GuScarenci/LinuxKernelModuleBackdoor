KDIR        ?= /lib/modules/$(shell uname -r)/build
CC           = gcc
MODULE_NAME  = deviceLogger

SOURCE_DIR   = $(CURDIR)/src
MODULE_PATH  = $(SOURCE_DIR)/$(MODULE_NAME).ko
obj-m       += MODULE_NAME.o

.PHONY: all clean module insert remove

all: module

insert:
	sudo insmod $(MODULE_PATH)

remove:
	sudo rmmod $(MODULE_PATH)

clean:
	sudo $(MAKE) -C $(SOURCE_DIR) clean

module:
	sudo $(MAKE) -C $(SOURCE_DIR) all

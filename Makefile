KDIR        ?= /lib/modules/$(shell uname -r)/build
CC           = gcc

include src/Kbuild

SOURCE_DIR   = $(CURDIR)/src
MODULE_PATH  = $(SOURCE_DIR)/$(MODULE_NAME).ko

.PHONY: all clean module insert remove

all: module

insert:
	sudo insmod $(MODULE_PATH)

remove:
	sudo rmmod $(MODULE_PATH)

clean:
	$(MAKE) -C $(KDIR) M=$(SOURCE_DIR) clean

module: clean
	$(MAKE) -C $(KDIR) M=$(SOURCE_DIR) modules

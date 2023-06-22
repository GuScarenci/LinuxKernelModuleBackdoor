KDIR       ?= /lib/modules/kernels/$(shell uname -r)
CC          = gcc
SOURCE_DIR  = $(CURDIR)/src

.PHONY: all clean module insert remove

all: module

insert:
	sudo insmod src/backdoor.ko

remove:
	sudo rmmod src/backdoor.ko

clean:
	$(MAKE) --debug -C $(KDIR) M=$(SOURCE_DIR) clean

module:
	$(MAKE) --debug CC=$(CC) -C $(KDIR) M=$(SOURCE_DIR) modules
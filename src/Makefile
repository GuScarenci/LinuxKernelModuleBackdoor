SOURCE_FILES = $(shell find src/ -type f |grep '\.c')
obj-m       += $(patsubs %.c, %.o, $(SOURCE_FILES))

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(CURDIR) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(CURDIR) clean`

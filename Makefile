CFLAGS    += -std=gnu99 -Wall -Wextra
CPPFLAGS  += -Wall -Wextra
LDFLAGS   += -lpthread
VDFLAGS    = --track-origins=yes -v --leak-check=full --show-leak-kinds=all

EXECUTABLE ?= build/main
ZIPFILE    ?= ../zipfile.zip
CFILES      = $(shell find src/ -type f |grep '\.c')
CPPFILES    = $(shell find src/ -type f |grep '\.cpp$$')
OFILES      = $(patsubst src/%.c,build/obj/%.o, $(CFILES))
OFILES     += $(patsubst src/%.cpp,build/obj/%.o, $(CPPFILES))
SOURCE_FILES = $(CURDIR)/src

CC   = gcc
CCPP = g++

.PHONY: all clean zip run debug gdb valgrind


all:
	make --debug -C /lib/modules/$(shell uname -r)/build M=$(SOURCE_FILES) modules 

clean:
	@rm -f $(ZIPFILE)
	@rm -rf build/
	make --debug -C /lib/modules/$(shell uname -r)/build M=$(SOURCE_FILES) clean

zip: clean
	7za a $(ZIPFILE) ./*

gdb: debug
	gdb $(EXECUTABLE)

run: $(EXECUTABLE)
	@./$(EXECUTABLE) $(ARGS)

valgrind: debug
	valgrind $(VDFLAGS) $(EXECUTABLE)

debug: CFLAGS+=-g3 -O0 
debug: CPPFLAGS+=-g3 -O0
debug: clean
debug: $(EXECUTABLE)


$(EXECUTABLE): $(OFILES)
	@mkdir -p build
	$(CCPP) $(LDFLAGS) -o $@ $^

build/obj/%.o: src/%.c src/%.h
	@mkdir -p build
	@mkdir -p build/obj
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

build/obj/%.o: src/%.c
	@mkdir -p build
	@mkdir -p build/obj
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

build/obj/%.o: src/%.cpp src/%.hpp
	@mkdir -p build
	@mkdir -p build/obj
	$(CCPP) $(CPPFLAGS) -c -o $@ $<

build/obj/%.o: src/%.cpp
	@mkdir -p build
	@mkdir -p build/obj
	$(CCPP) $(CPPFLAGS) -c -o0 $@ $<
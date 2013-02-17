CFLAGS := $(CFLAGS) -Wall -O2 -mtune=native -g
INC    := -I. $(INC)
DEFINES:= $(DEFINES)
CC     := gcc
BINARY := watchpuppy
DEPS   := build/main.o build/check_pid.o build/check_tcp.o build/hook.o

.PHONY: all clean dev test

all: build $(DEPS) link

dev: clean
	DEFINES="-DDEV" $(MAKE)

build:
	-mkdir build bin

%.o: $(patsubst build/%o,%c,$@)
	$(CC) $(CFLAGS) $(DEFINES) $(INC) -c -o $@ $(patsubst build/%o,%c,$@)

link: $(DEPS)
	$(CC) $(CFLAGS) $(DEFINES) $(INC) -o bin/$(BINARY) $(DEPS)

clean:
	rm -fv $(DEPS) bin/$(BINARY)

test:
	$(MAKE) -C test

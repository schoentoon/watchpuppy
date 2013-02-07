CFLAGS := $(CFLAGS) -Wall -O2 -mtune=native -g
INC    := -I. $(INC)
CC     := gcc
BINARIES:= listen

.PHONY: all clean dev

all: $(BINARIES)

dev: clean
	$(MAKE)

%: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ $@.c

clean:
	rm -fv $(BINARIES)

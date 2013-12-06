CFLAGS := $(CFLAGS) -Wall -O2 -mtune=native -g
MFLAGS := -shared -fPIC
DEFINES:= $(DEFINES)
CC     := gcc
SAMPLES:= ssl.so

.PHONY: all clean

all: $(SAMPLES)

ssl.so: ssl.c
	$(CC) $(CFLAGS) $(MFLAGS) $(DEFINES) -o ssl.so ssl.c $(shell pkg-config --libs --cflags openssl)

clean:
	rm -fv $(SAMPLES)
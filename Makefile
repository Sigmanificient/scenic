# Nb Makefile. C23, strict warnings, sanitizer-ready.

CC      ?= cc
CFLAGS  ?= -std=c23 -O2 -g \
           -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
           -Wstrict-prototypes -Wmissing-prototypes \
           -Wno-unused-parameter \
           -fno-strict-aliasing
LDFLAGS ?=

ifdef SANITIZE
    CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address,undefined
endif

INCLUDES = -Iinclude -Isrc

SRC_FILES = \
    src/main.c \
    src/arena.c \
    src/error.c \
    src/hash.c \
    src/store.c \
    src/resolve.c \
    src/validate.c \
    src/fetch.c \
    src/sandbox.c \
    src/build.c \
    src/run.c \
    src/realize.c \
    src/activate.c

PKG_FILES = $(shell find pkgs -name '*.c')

CONFIG_FILE ?= etc-example/config.c

OBJ_FILES = $(SRC_FILES:.c=.o) $(PKG_FILES:.c=.o)

.PHONY: all clean registry switch

all: nb

nb: $(OBJ_FILES) $(CONFIG_FILE:.c=.o) pkgs/all.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

registry:
	./tools/gen-registry.sh > pkgs/all.h.tmp
	mv pkgs/all.h.tmp pkgs/all.h
	./tools/gen-registry.sh --c > pkgs/all.c.tmp
	mv pkgs/all.c.tmp pkgs/all.c

clean:
	find . -name '*.o' -delete
	rm -f nb

switch: nb
	./nb switch

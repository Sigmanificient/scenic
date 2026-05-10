CC      ?= cc
CFLAGS  ?= -std=c99 -O2 -D_POSIX_C_SOURCE=200809L -g \
           -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
           -Wstrict-prototypes -Wmissing-prototypes \
           -Wno-unused-parameter \
           -fno-strict-aliasing
LDFLAGS ?=

PROFILE ?= release
ifdef SANITIZE
    PROFILE := sanitize
    CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address,undefined
endif

BUILD_DIR := .build/$(PROFILE)

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

ALL_SRCS  = $(SRC_FILES) $(PKG_FILES) $(CONFIG_FILE)
OBJ_FILES = $(patsubst %.c,$(BUILD_DIR)/%.o,$(ALL_SRCS))
DEP_FILES = $(OBJ_FILES:.o=.d)

.PHONY: all clean registry switch

all: scn

scn: $(BUILD_DIR)/scn
	@ln -sf $< $@

$(BUILD_DIR)/scn: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c -o $@ $<

-include $(DEP_FILES)

registry:
	./tools/gen-registry.sh > pkgs/all.h.tmp
	mv pkgs/all.h.tmp pkgs/all.h
	./tools/gen-registry.sh --c > pkgs/all.c.tmp
	mv pkgs/all.c.tmp pkgs/all.c

clean:
	rm -rf .build scn

switch: scn
	./scn switch

debug:
	$(MAKE) $(MAKEFLAGS) SANITIZE=1

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

CC := gcc
WARNINGS = -pedantic -Wcast-align -Wpointer-arith \
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-aliasing \
	-Wmissing-declarations -Winline -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal
INCLUDES := -Iinclude/ -Iextern/
CFLAGS = -std=c11 -D_POSIX_C_SOURCE=200112L $(INCLUDES) $(WARNINGS)

SOURCES = $(strip $(call rwildcard,src/,*.c) $(call rwildcard,extern/ccan/,*.c))
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

.PHONY: libvial.a clean

all: libvial.a

libvial.a: $(OBJECTS)
	@echo Archiving...
	ar rcs $@ $^

clean:
	rm -f $(OBJECTS)

%.o: %.c
	@echo Compiling $<
	@$(CC) -o $@ -c $< $(CFLAGS)

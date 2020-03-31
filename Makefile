rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

WARNINGS := -pedantic -Wcast-align -Wpointer-arith \
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-aliasing \
	-Wmissing-declarations -Winline -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal
INCLUDES := -Iinclude/ -Iextern/
CFLAGS := -std=c11 -D_POSIX_C_SOURCE=200112L $(INCLUDES) $(WARNINGS)

SOURCES = $(strip $(call rwildcard,src/,*.c) $(call rwildcard,extern/ccan/,*.c))
OBJECTS = $(SOURCES:%.c=%.o)

TEST_SOURCES = $(wildcard test/*.c)
TEST_OBJECTS = $(TEST_SOURCES:%.c=%)

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

.PHONY: all test install uninstall clean

all: libvial.a test

install: libvial.a
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 libvial.a $(DESTDIR)$(PREFIX)/lib/
	cp -a --remove-destination include/vial/ $(DESTDIR)$(PREFIX)/include/
	chown 644 -R $(DESTDIR)$(PREFIX)/include/vial/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libvial.a
	rm -rf $(DESTDIR)$(PREFIX)/include/vial/

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS)

libvial.a: $(OBJECTS)
	@echo Archiving...
	ar rcs $@ $^

%.o: %.c
	@echo Compiling $<
	@$(CC) -o $@ -c $< $(CFLAGS)

test: $(TEST_OBJECTS)

test/%: test/%.c libvial.a
	@echo Compiling $<
	@$(CC) $< -o $@ $(CFLAGS) -lm -L. -lvial

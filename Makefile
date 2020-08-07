rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

WARNINGS := -pedantic -Wcast-align -Wpointer-arith \
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-aliasing \
	-Wmissing-declarations -Winline -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal
INCLUDES := -Iinclude/
CFLAGS := -std=c11 -D_POSIX_C_SOURCE=200112L $(INCLUDES) $(WARNINGS)

SOURCES = $(strip $(call rwildcard,src/,*.c))
OBJECTS = $(SOURCES:%.c=%.o)

TEST_SOURCES = $(wildcard test/*.c)
TEST_OBJECTS = $(TEST_SOURCES:%.c=%)

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

ifeq ($(ASM_INTEL_AES),1)
	ASM_SOURCES += src/asm/intel_aes.s
	CFLAGS += -DASM_INTEL_AES=1
endif

ASM_OBJECTS = $(ASM_SOURCES:%.s=%.o)

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
	rm -f $(strip $(call rwildcard,src/,*.o))

dependencies.mk: $(SOURCES) $(strip $(call rwildcard,include/vial/,*.h))
	python3 generate_make_dependencies.py $(SOURCES) > dependencies.mk

libvial.a: $(OBJECTS) $(ASM_OBJECTS)
	@echo Archiving...
	ar rcs $@ $^

include dependencies.mk

src/asm/%.o: src/asm/%.s
	$(AS) -o $@ -c $<

%.o: %.c
	@echo Compiling $<
	@$(CC) -o $@ -c $< $(CFLAGS)

test: $(TEST_OBJECTS)

test/%: test/%.c libvial.a
	@echo Compiling $<
	@$(CC) $< -o $@ $(CFLAGS) -L. -lvial -lm

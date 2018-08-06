CFLAGS = -Wall -O3 -fomit-frame-pointer -fno-asynchronous-unwind-tables
# -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -m64

# CC = clang

# SRCS = src/forthwith.c src/utilities.c src/access.c src/dict.c src/inner.c src/core.c
# OBJS = $(SRCS:src/%.c=_build/%.o)

_build/forthwith-linux.a: _build/forthwith-linux.o
	ar rcs $@ $<

_build/forthwith-linux: src/forthwith-linux.c
	$(CC) -o $@.S $(CFLAGS) -S $<
	$(CC) -o $@ $(CFLAGS) $<

%.o: %.c *.h
	${CC} ${CFLAGS} $< -c -o $@

_build/%.o: src/%.c
	${CC} ${CFLAGS} $< -E -o $@.post.c
	${CC} ${CFLAGS} $< -S -o $@.S
	${CC} ${CFLAGS} $< -c -o $@


test: forthwith
	./forthwith test.fth


clean:
	rm -Rf _build/*

.PHONY: clean examples test


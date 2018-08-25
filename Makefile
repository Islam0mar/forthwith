BASE_CFLAGS = -Wall -fomit-frame-pointer -fno-asynchronous-unwind-tables -m64 -Wno-unused-function -Isrc/

CFLAGS = -Wall -O3 -DFW_TRACE -g $(BASE_CFLAGS)
# CFLAGS = -Wall -O3 $(BASE_CFLAGS)


# CC = clang
CC = gcc

# PRU_LINKER_COMMAND_FILE=./AM335x_PRU.cmd
PINCLUDE=--include_path=src/ --include_path=$(PRU_LIB)/pru/include/ --include_path=$(PRU_LIB)/pru/include/am335x
PSTACK_SIZE=0x100
PHEAP_SIZE=0x100

#Common compiler and linker flags (Defined in 'PRU Optimizing C/C++ Compiler User's Guide)
PCFLAGS=-v3 -O3 --c99 -k --display_error_number --endian=little --hardware_mac=on --obj_directory=_build/beagle-pru/ --pp_directory=_build/beagle-pru/ -ppd -ppa -DFW_NO_CORE_MULTIPLY -DFORTHWITH_NO_CHECKS
#Linker flags (Defined in 'PRU Optimizing C/C++ Compiler User's Guide)
PLFLAGS=--reread_libs --warn_sections --stack_size=$(PSTACK_SIZE) --heap_size=$(PHEAP_SIZE)

pru: _build/beagle-pru/forthwith-pru.lib _build/beagle-pru/porting-guide-pru
linux: _build/linux-x86-64/forthwith-linux _build/linux-x86-64/test-forthwith-linux _build/linux-x86-64/porting-guide

_build/linux-x86-64/forthwith-linux.a: _build/linux-x86-64/forthwith-linux.o
	ar rcs $@ $<

_build/linux-x86-64/forthwith-linux: _build/linux-x86-64/forthwith-main.o _build/linux-x86-64/forthwith-linux.o
	$(CC) -o $@.S $(CFLAGS) -S $^
	$(CC) -o $@ $(CFLAGS) $^

_build/linux-x86-64/test-forthwith-linux: src/test/test.c _build/linux-x86-64/forthwith-linux.o
	$(CC) -o $@ $(CFLAGS) -Isrc/ -Isrc/linux-x86-64/ $^


_build/linux-x86-64/porting-guide: src/linux-x86-64/porting-guide.c _build/linux-x86-64/forthwith-linux.o
	${CC} ${CFLAGS} $< -E -o $@.post.c
	$(CC) -o $@ $(CFLAGS) $^
	$(CC) -o $@.S $(CFLAGS) -S $^

_build/beagle-pru/porting-guide-pru: src/beagle-pru/porting-guide.c _build/beagle-pru/forthwith-pru.lib
	$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(PINCLUDE) $(PCFLAGS) -fe $@ $<
	# $(PRU_CGT)/bin/clpru --section_sizes=on $(PCFLAGS) -z -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(PLFLAGS) -o $@ $^ -m$(MAP) $(LINKER_COMMAND_FILE) --library=libc.a $(PRU_LIBS)
	$(PRU_CGT)/bin/dispru --all $@ > $@.S

_build/beagle-pru/forthwith-pru.lib: _build/beagle-pru/forthwith-pru.o
	$(PRU_CGT)/bin/arpru r $@ $^

# _build/%.o: src/%.c
# 	${CC} ${CFLAGS} $< -E -o $@.post.c
# 	${CC} ${CFLAGS} $< -S -o $@.S
# 	${CC} ${CFLAGS} $< -c -o $@

# _build/%.o: src/test/%.c
#	${CC} ${CFLAGS} $< -c -o $@

_build/linux-x86-64/%.o: src/linux-x86-64/%.c
	${CC} ${CFLAGS} $< -E -o $@.post.c
	${CC} ${CFLAGS} $< -S -o $@.S
	${CC} ${CFLAGS} $< -c -o $@


_build/beagle-pru/%.o: src/beagle-pru/%.c
	$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(PINCLUDE) $(PCFLAGS) -fe $@ $<
	$(PRU_CGT)/bin/dispru --all $@ > $@.S


test: forthwith
	./forthwith test.fth


clean:
	rm -Rf _build/*
	mkdir _build/linux-x86-64/
	mkdir _build/linux-arm/
	mkdir _build/beagle-pru/

.PHONY: clean examples test


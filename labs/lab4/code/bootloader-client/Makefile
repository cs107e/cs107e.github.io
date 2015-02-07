CFLAGS = -Wall -O2 
CC = gcc
SRC = pi-install.c fatal.c serial.c
OBJS = $(SRC:.c=.o)

all : pi-install 

pi-install:  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o pi-install -lm

clean :
	rm -f *.o *~ Makefile.bak pi-install

depend:
	makedepend *.[ch] 
# DO NOT DELETE

fatal.o: /usr/include/errno.h /usr/include/features.h
fatal.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
fatal.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
fatal.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
fatal.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
fatal.o: /usr/include/asm-generic/errno-base.h /usr/include/stdio.h
fatal.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
fatal.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
fatal.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fatal.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
fatal.o: /usr/include/endian.h /usr/include/bits/endian.h
fatal.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
fatal.o: /usr/include/sys/select.h /usr/include/bits/select.h
fatal.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
fatal.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
fatal.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
fatal.o: /usr/include/string.h demand.h
pi-install.o: /usr/include/errno.h /usr/include/features.h
pi-install.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
pi-install.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pi-install.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pi-install.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
pi-install.o: /usr/include/asm-generic/errno-base.h /usr/include/fcntl.h
pi-install.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pi-install.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
pi-install.o: /usr/include/string.h /usr/include/stdio.h /usr/include/libio.h
pi-install.o: /usr/include/_G_config.h /usr/include/wchar.h
pi-install.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pi-install.o: /usr/include/sys/types.h /usr/include/time.h
pi-install.o: /usr/include/endian.h /usr/include/bits/endian.h
pi-install.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
pi-install.o: /usr/include/sys/select.h /usr/include/bits/select.h
pi-install.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
pi-install.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pi-install.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
pi-install.o: /usr/include/termios.h /usr/include/bits/termios.h
pi-install.o: /usr/include/sys/ttydefaults.h /usr/include/unistd.h
pi-install.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
pi-install.o: /usr/include/bits/confname.h /usr/include/getopt.h demand.h

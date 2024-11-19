CC=gcc
CFLAGS=-fno-stack-protector -D_FORTIFY_SOURCE=0 -O1 -DDEBUG -g
# CFLAGS=-fno-stack-protector

all: pw_checker_naive pw_checker pw_checkerQ3 foo libhack.so

foo : foo.o
	$(CC) $(CFLAGS) -o $@ $<

pw_checker_naive: pw_checker_naive.o
	$(CC) $(CFLAGS) -Wall -Wextra -O2 -g -o $@ $<
	objcopy --strip-unneeded $@
pw_checker: pw_checker.o libpasswds.so
	$(CC) $(CFLAGS) -Wall -Wextra -DNDEBUG -O2 $< -o $@ -Wl,-rpath,./ -L./ -lpasswds -lcrypto -lssl
	objcopy --strip-unneeded $@
pw_checkerQ3: pw_checkerQ3.o libpasswds.so
	$(CC) $(CFLAGS)  -Wextra   $< -o $@ -Wl,-rpath,./ -L./ -lpasswds -lcrypto -lssl
	objcopy --strip-unneeded $@
lib%.so: %.c
	$(CC) $(CFLAGS) -Wall -Wextra -shared -fPIC  $< -o $@
	objcopy --strip-unneeded $@
%.o: %.c
# $(CC) $(CFLAGS) -Wall -Wextra -g -O2  -c $< -o $@
	$(CC) $(CFLAGS)  -c $< -o $@

.PHONY: clean clear
clean:
	-rm -v pw_checker_naive.o pw_checker.o
clear: clean
	-rm -v pw_checker_naive pw_checker libpasswds.so libhack.so

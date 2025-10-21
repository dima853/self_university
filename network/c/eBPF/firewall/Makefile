<<<<<<< HEAD
# Для Kali Linux
CLANG := clang
CFLAGS := -target bpf -O2 -Wall -g

# Пути для заголовков (попробуй разные варианты)
INCLUDES := -I/usr/include/x86_64-linux-gnu \
            -I/usr/include/linux
=======
# WSL2 eBPF Firewall Makefile
HEADER_PATH := /usr/src/linux-headers-6.8.0-86-generic
>>>>>>> 9a5a21fd3e08918a631ea5b4888a86ac8e3f93a9

CLANG_INCLUDES := -I$(HEADER_PATH)/include \
                  -I$(HEADER_PATH)/arch/x86/include \
                  -I$(HEADER_PATH)/arch/x86/include/generated \
                  -I$(HEADER_PATH)/include/uapi \
                  -I$(HEADER_PATH)/include/generated/uapi

CFLAGS := -target bpf -O2 -Wall -g

all: firewall.bpf.o

firewall.bpf.o: firewall.bpf.c
<<<<<<< HEAD
	$(CLANG) $(CFLAGS) $(INCLUDES) -c $< -o $@

firewall: firewall.c
	$(CC) -o $@ $< -lbpf
=======
	clang $(CFLAGS) $(CLANG_INCLUDES) -c $< -o $@
>>>>>>> 9a5a21fd3e08918a631ea5b4888a86ac8e3f93a9

clean:
	rm -f *.o

.PHONY: all clean
# WSL2 eBPF Firewall Makefile
HEADER_PATH := /usr/src/linux-headers-6.8.0-86-generic

CLANG_INCLUDES := -I$(HEADER_PATH)/include \
                  -I$(HEADER_PATH)/arch/x86/include \
                  -I$(HEADER_PATH)/arch/x86/include/generated \
                  -I$(HEADER_PATH)/include/uapi \
                  -I$(HEADER_PATH)/include/generated/uapi

CFLAGS := -target bpf -O2 -Wall -g

all: firewall.bpf.o

firewall.bpf.o: firewall.bpf.c
	clang $(CFLAGS) $(CLANG_INCLUDES) -c $< -o $@

clean:
	rm -f *.o

.PHONY: all clean
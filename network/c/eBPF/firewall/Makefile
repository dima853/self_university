# Для Kali Linux
CLANG := clang
CFLAGS := -target bpf -O2 -Wall -g

# Пути для заголовков (попробуй разные варианты)
INCLUDES := -I/usr/include/x86_64-linux-gnu \
            -I/usr/include/linux

all: firewall.bpf.o firewall

firewall.bpf.o: firewall.bpf.c
	$(CLANG) $(CFLAGS) $(INCLUDES) -c $< -o $@

firewall: firewall.c
	$(CC) -o $@ $< -lbpf

clean:
	rm -f firewall.bpf.o firewall

.PHONY: all clean
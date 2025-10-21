CLANG ?= clang
CFLAGS := -O2 -Wall

all: firewall.bpf.o firewall

firewall.bpf.o: firewall.bpf.c
	$(CLANG) -target bpf $(CFLAGS) -c $< -o $@

firewall: firewall.c
	$(CC) $(CFLAGS) -o $@ $< -lbpf

clean:
	rm -f firewall.bpf.o firewall

.PHONY: all clean
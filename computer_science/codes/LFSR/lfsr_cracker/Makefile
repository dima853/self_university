CC=gcc
CFLAGS=-Wall -Wextra -std=c99
SRC_DIR=src
BUILD_DIR=build

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/lfsr.c $(SRC_DIR)/bm_attack.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/lfsr.o $(BUILD_DIR)/bm_attack.o

TARGET=lfsr_cracker

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
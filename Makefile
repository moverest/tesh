BUILD_DIR = build
SRC_DIR = src

CC = gcc
CFLAGS = -Wall
CFLAGS_DEBUG = -g -DDEBUG

.PHONY: all
all: $(BUILD_DIR) $(BUILD_DIR)/tesh

.PHONY: debug
debug: CFLAGS += $(CFLAGS_DEBUG)
debug: all

.PHONY: clean
clean:
	rm -rvf $(BUILD_DIR)

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) -o $@ $(filter %.o, $^) $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

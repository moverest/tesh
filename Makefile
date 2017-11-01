BUILD_DIR = build
SRC_DIR = src

CFLAGS = -Wall
CFLAGS_DEBUG = -g -DDEBUG
CC = gcc $(CFLAGS)

.PHONY: all
all: $(BUILD_DIR) $(BUILD_DIR)/tesh $(BUILD_DIR)/test_tokenizer tesh

.PHONY: debug
debug: CFLAGS += $(CFLAGS_DEBUG)
debug: all

.PHONY: clean
clean:
	rm -rvf $(BUILD_DIR) tesh

$(BUILD_DIR)/tesh: $(BUILD_DIR)/tokenizer.o
$(BUILD_DIR)/tokenizer: $(SRC_DIR)/tokenizer.h
$(BUILD_DIR)/test_tokenizer: $(BUILD_DIR)/tokenizer.o $(BUILD_DIR)/test.o
$(BUILD_DIR)/test: $(SRC_DIR)/test.h

tesh: run
	cp run tesh

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) -o $@ $(filter %.o, $^) $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

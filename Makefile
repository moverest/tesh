BUILD_DIR = build
SRC_DIR = src

CFLAGS = -Wall -std=gnu99 -ldl
CFLAGS_DEBUG = -g -DDEBUG
CC = gcc $(CFLAGS)

.PHONY: all
all: $(BUILD_DIR) $(BUILD_DIR)/tesh tesh

.PHONY: tests
tests: $(BUILD_DIR) $(foreach f, test_tokenizer test_parser test_vector test_input_reader, $(BUILD_DIR)/$f)
.PHONY: debug
debug: CFLAGS += $(CFLAGS_DEBUG)
debug: all tests

.PHONY: clean
clean:
	rm -rvf $(BUILD_DIR) tesh

$(BUILD_DIR)/tesh: $(foreach f, tokenizer input_reader vector parser built_in, $(BUILD_DIR)/$f.o)
$(BUILD_DIR)/tokenizer.o: $(SRC_DIR)/tokenizer.h
$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.h
$(BUILD_DIR)/built_in.o: $(SRC_DIR)/built_in.h
$(BUILD_DIR)/vector.o: $(SRC_DIR)/vector.h
$(BUILD_DIR)/input_reader.o: $(SRC_DIR)/input_reader.h
$(BUILD_DIR)/test_input_reader: $(foreach f, input_reader.o test.o vector.o, $(BUILD_DIR)/$f)
$(BUILD_DIR)/test_tokenizer: $(BUILD_DIR)/tokenizer.o $(BUILD_DIR)/test.o
$(BUILD_DIR)/test_parser: $(foreach f, parser.o test.o vector.o tokenizer.o built_in.o, $(BUILD_DIR)/$f)
$(BUILD_DIR)/test_vector: $(BUILD_DIR)/vector.o $(BUILD_DIR)/test.o
$(BUILD_DIR)/test: $(SRC_DIR)/test.h

tesh: $(BUILD_DIR)/tesh
	cp $(BUILD_DIR)/tesh tesh

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) -o $@ $(filter %.o, $^) $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

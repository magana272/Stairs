TARGET_EXEC := main

customer_TEST_TARGET := customer_test

BUILD_DIR := ./build

SRC_DIRS := ./src
CC = gcc

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
# CPPFLAGS := $(INC_FLAGS) -g -w
CPPFLAGS := $(INC_FLAGS) -g -w

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -g -o $@ $(LDFLAGS)
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


sim:
	gcc -I./src \
	-I./src/stair \
	-I./src/scheduler\
	-I./src/simulation\
	-I./src/utils\
	-I./src/utils/argparser\
	-I./src/utils/queue\
	-I./src/utils/log\
	-I./src/lib -I./src/customer\
	-g\
	-c\
	src/scheduler/scheduler.c\
	-o build/./src/scheduler/scheduler.c.o



.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

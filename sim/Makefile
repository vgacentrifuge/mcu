
CC:=zig cc

CFLAGS:=$(CFLAGS) -Wall -Wextra -O2 -Iraylib -I../inc -I../vendored -Iinc -g
LDFLAGS:=$(LDFLAGS) -Wall -Wextra -O2 -Lraylib -g

BUILD_DIR:=build

# These are the files from the actual codebase that get included in our sim
SRC_FILES:=../src/app.c ../src/debug.c ../src/ui.c ../src/state.c ../src/image.c
# Files belonging to the sim
SIM_FILES:=$(wildcard src/*.c)

SIM_OBJECT_FILES:=$(patsubst src/%.c,$(BUILD_DIR)/sim/%.o,$(SIM_FILES))
SRC_OBJECT_FILES:=$(patsubst ../src/%.c,$(BUILD_DIR)/src/%.o,$(SRC_FILES))

RAYLIB_FILE:=raylib/libraylib.a

.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/sim $(BUILD_DIR)/src $(BUILD_DIR)/mcu_sim

$(BUILD_DIR)/mcu_sim: $(SIM_OBJECT_FILES) $(SRC_OBJECT_FILES) $(RAYLIB_FILE)
	$(CC) $(LDFLAGS) $(SIM_OBJECT_FILES) $(SRC_OBJECT_FILES) -o $@ -lraylib -lm

$(BUILD_DIR)/src/%.o: ../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/sim/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RAYLIB_FILE):
	cd raylib && zig build

$(BUILD_DIR) $(BUILD_DIR)/sim $(BUILD_DIR)/src:
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

purge: clean
	rm -rf $(RAYLIB_FILE)

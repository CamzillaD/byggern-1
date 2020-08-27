PROGRAM_WITH_JTAG := yes
SOURCE_DIR := src
BUILD_DIR := build

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $@

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$<:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e

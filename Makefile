ASM = nasm
CC = gcc

BIN_DIR = ./bin

BOOT_SRC_DIR = ./src/boot
BOOT_IMG = $(BIN_DIR)/boot.o
BOOT_ASM_FILE = $(BOOT_SRC_DIR)/boot.asm

KERNEL_SRC_DIR = ./src/kernel
KERNEL_START_FILE = $(KERNEL_SRC_DIR)/start.asm
KERNEL_START_OBJECT_FILE = $(BIN_DIR)/start.o
KERNEL_MAIN_FILE = $(KERNEL_SRC_DIR)/main.c
KERNEL_MAIN_ELF_FILE = $(BIN_DIR)/main.o
KERNEL_DISPLAY_FILE = $(KERNEL_SRC_DIR)/display.c
KERNEL_DISPLAY_ELF_FILE = $(BIN_DIR)/display.o
KERNEL_UITOA_FILE = $(KERNEL_SRC_DIR)/uitoa.c
KERNEL_UITOA_ELF_FILE = $(BIN_DIR)/uitoa.o

KERNEL_ELF_FILE = $(BIN_DIR)/kernel.elf
KERNEL_IMAGE = $(BIN_DIR)/kernel.img
KERNEL_LINKER_SCRIPT = $(KERNEL_SRC_DIR)/linker.ld
KERNEL_FLAGS = -Wall -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie

DISK_IMG=$(BIN_DIR)/os.img

QEMU = qemu-system-x86_64 -usb -device qemu-xhci -s -drive format=raw,file=$(DISK_IMG)
QEMU_DEBUG = qemu-system-x86_64 -usb -device qemu-xhci -s -S -drive format=raw,file=$(DISK_IMG)

build: $(BOOT_IMG) $(KERNEL_IMAGE)
	dd if=$(BOOT_IMG) of=$(DISK_IMG)
	dd seek=1 conv=sync if=$(KERNEL_IMAGE) of=$(DISK_IMG) bs=512 
#	dd seek=6 conv=sync if=/dev/zero of=$(DISK_IMG) bs=512 count=6

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(BOOT_IMG): $(BIN_DIR) $(BOOT_SRC_DIR)/*.asm 
	$(ASM) -i $(BOOT_SRC_DIR) -f bin $(BOOT_ASM_FILE) -o $(BOOT_IMG)	

$(KERNEL_IMAGE): $(BIN_DIR) $(KERNEL_SRC_DIR)/*.asm $(KERNEL_SRC_DIR)/*.c $(KERNEL_SRC_DIR)/*.h
	$(ASM) -i $(KERNEL_SRC_DIR) -f elf32 $(KERNEL_START_FILE) -o $(KERNEL_START_OBJECT_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_MAIN_FILE) -o $(KERNEL_MAIN_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_DISPLAY_FILE) -o $(KERNEL_DISPLAY_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_UITOA_FILE) -o $(KERNEL_UITOA_ELF_FILE)
	ld -melf_i386 -T$(KERNEL_LINKER_SCRIPT) $(KERNEL_START_OBJECT_FILE) $(KERNEL_MAIN_ELF_FILE) $(KERNEL_DISPLAY_ELF_FILE) $(KERNEL_UITOA_ELF_FILE) -o $(KERNEL_ELF_FILE)
	objcopy -O binary $(KERNEL_ELF_FILE) $(KERNEL_IMAGE)

run: build
	$(QEMU)	

debug: build
	$(QEMU_DEBUG)

clean: $(BIN_DIR)
	rm -f $(BIN_DIR)/*
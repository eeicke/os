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
KERNEL_MEMMAP_FILE = $(KERNEL_SRC_DIR)/memmap.c
KERNEL_MEMMAP_ELF_FILE = $(BIN_DIR)/memmap.o
KERNEL_UITOA_FILE = $(KERNEL_SRC_DIR)/uitoa.c
KERNEL_UITOA_ELF_FILE = $(BIN_DIR)/uitoa.o
KERNEL_MATH_FILE = $(KERNEL_SRC_DIR)/math.c
KERNEL_MATH_ELF_FILE = $(BIN_DIR)/math.o
KERNEL_STRING_FILE = $(KERNEL_SRC_DIR)/string.c
KERNEL_STRING_ELF_FILE = $(BIN_DIR)/string.o
KERNEL_ATA_FILE = $(KERNEL_SRC_DIR)/ata.c
KERNEL_ATA_ELF_FILE = $(BIN_DIR)/ata.o
KERNEL_ELF_FILE = $(BIN_DIR)/kernel.elf
KERNEL_IMAGE = $(BIN_DIR)/kernel.img
KERNEL_LINKER_SCRIPT = $(KERNEL_SRC_DIR)/linker.ld
KERNEL_FLAGS = -Wall -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie

SECONDARY_DRIVE_NAME = second.img
SECONDARY_DRIVE = $(BIN_DIR)/$(SECONDARY_DRIVE_NAME)

DISK_IMG=$(BIN_DIR)/os.img

QEMU = qemu-system-x86_64 -usb -device qemu-xhci -s -drive format=raw,file=$(DISK_IMG) -drive format=raw,file=$(SECONDARY_DRIVE)
QEMU_DEBUG = qemu-system-x86_64 -usb -device qemu-xhci -s -S -drive format=raw,file=$(DISK_IMG) -drive format=raw,file=$(SECONDARY_DRIVE)

build: $(BOOT_IMG) $(KERNEL_IMAGE)
	dd if=$(BOOT_IMG) of=$(DISK_IMG)
	dd seek=1 conv=sync if=$(KERNEL_IMAGE) of=$(DISK_IMG) bs=512 
	dd seek=2048 conv=sync if=helloworld of=$(DISK_IMG) bs=512

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(SECONDARY_DRIVE):
	dd if=/dev/zero of=$(BIN_DIR)/second.img bs=512 count=8192

$(BOOT_IMG): $(BIN_DIR) $(BOOT_SRC_DIR)/*.asm 
	$(ASM) -i $(BOOT_SRC_DIR) -f bin $(BOOT_ASM_FILE) -o $(BOOT_IMG)	

$(KERNEL_IMAGE): $(BIN_DIR) $(KERNEL_SRC_DIR)/*.asm $(KERNEL_SRC_DIR)/*.c $(KERNEL_SRC_DIR)/*.h
	$(ASM) -i $(KERNEL_SRC_DIR) -f elf32 $(KERNEL_START_FILE) -o $(KERNEL_START_OBJECT_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_MAIN_FILE) -o $(KERNEL_MAIN_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_DISPLAY_FILE) -o $(KERNEL_DISPLAY_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_MEMMAP_FILE) -o $(KERNEL_MEMMAP_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_UITOA_FILE) -o $(KERNEL_UITOA_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_STRING_FILE) -o $(KERNEL_STRING_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_MATH_FILE) -o $(KERNEL_MATH_ELF_FILE)
	$(CC) $(KERNEL_FLAGS) $(KERNEL_ATA_FILE) -o $(KERNEL_ATA_ELF_FILE)
	ld -melf_i386 -T$(KERNEL_LINKER_SCRIPT) $(KERNEL_START_OBJECT_FILE) $(KERNEL_MAIN_ELF_FILE) $(KERNEL_DISPLAY_ELF_FILE) $(KERNEL_MEMMAP_ELF_FILE) $(KERNEL_UITOA_ELF_FILE) $(KERNEL_STRING_ELF_FILE) $(KERNEL_MATH_ELF_FILE) $(KERNEL_ATA_ELF_FILE)  -o $(KERNEL_ELF_FILE)
	objcopy -O binary $(KERNEL_ELF_FILE) $(KERNEL_IMAGE)

run: build $(SECONDARY_DRIVE)
	$(QEMU)	

debug: build $(SECONDARY_DRIVE)
	$(QEMU_DEBUG)

resetseconddrive: $(BIN_DIR)
	rm $(BIN_DIR)/second.img
	dd if=/dev/zero of=$(BIN_DIR)/second.img bs=512 count=8192

clean: $(BIN_DIR)
	find $(BIN_DIR)/ ! -name '$(SECONDARY_DRIVE_NAME)' -type f -exec rm -rf {} +

cleanall: $(BIN_DIR)
	rm -f $(BIN_DIR)/*
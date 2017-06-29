BOOT   := boot.bin
KERNEL := kernel.bin
GAME   := game.bin
MY_SHELL   := my_shell.bin
IDLE   := idel.bin
TESTCASE := testcase.bin
IMAGE  := disk.bin

MY_FILES := in.txt
MY_FILES += out.txt

CC      := gcc
LD      := ld
OBJCOPY := objcopy
DD      := dd
QEMU    := qemu-system-i386
GDB     := gdb
STU_ID	:= 151242002
CFLAGS := -Wall -Wfatal-errors #开启所有警告, 视警告为错误, 第一个错误结束编译
CFLAGS += -MD #生成依赖文件
CFLAGS += -std=gnu11 -m32 -c #编译标准, 目标架构, 只编译
CFLAGS += -I./include -I./kernel/include -I./game/include #头文件搜索目录
CFLAGS += -O0 #不开优化, 方便调试
CFLAGS += -fno-builtin -fno-stack-protector#禁止内置函数
CFLAGS += -ggdb3 #GDB调试信息

QEMU_OPTIONS := -serial stdio #以标准输入输为串口(COM1)
# QEMU_OPTIONS += -d int #输出中断信息
QEMU_OPTIONS += -monitor telnet:127.0.0.1:1111,server,nowait #telnet monitor

QEMU_DEBUG_OPTIONS := -S #启动不执行
QEMU_DEBUG_OPTIONS += -s #GDB调试服务器: 127.0.0.1:1234

GDB_OPTIONS := -ex "target remote 127.0.0.1:1234"
GDB_OPTIONS += -ex "symbol $(KERNEL)"

OBJ_DIR        := obj
LIB_DIR        := lib
BOOT_DIR       := boot
KERNEL_DIR     := kernel
GAME_DIR       := game
IDLE_DIR       := idle
MY_SHELL_DIR   := my_shell
TESTCASE_DIR   := testcase
OBJ_LIB_DIR    := $(OBJ_DIR)/$(LIB_DIR)
OBJ_BOOT_DIR   := $(OBJ_DIR)/$(BOOT_DIR)
OBJ_KERNEL_DIR := $(OBJ_DIR)/$(KERNEL_DIR)
OBJ_GAME_DIR   := $(OBJ_DIR)/$(GAME_DIR)
OBJ_IDLE_DIR   := $(OBJ_DIR)/$(IDLE_DIR)
OBJ_MY_SHELL_DIR   := $(OBJ_DIR)/$(MY_SHELL_DIR)
OBJ_TESTCASE_DIR   := $(OBJ_DIR)/$(TESTCASE_DIR)


KERNEL_LD_SCRIPT := $(shell find $(KERNEL_DIR) -name "*.ld")
GAME_LD_SCRIPT   := $(shell find $(GAME_DIR) -name "*.ld")

LIB_C := $(wildcard $(LIB_DIR)/*.c)
LIB_O := $(LIB_C:%.c=$(OBJ_DIR)/%.o)

BOOT_S := $(wildcard $(BOOT_DIR)/*.S)
BOOT_C := $(wildcard $(BOOT_DIR)/*.c)
BOOT_O := $(BOOT_S:%.S=$(OBJ_DIR)/%.o)
BOOT_O += $(BOOT_C:%.c=$(OBJ_DIR)/%.o)

KERNEL_C := $(shell find $(KERNEL_DIR) -name "*.c")
KERNEL_S := $(shell find $(KERNEL_DIR) -name "*.S")
KERNEL_O := $(KERNEL_C:%.c=$(OBJ_DIR)/%.o)
KERNEL_O += $(KERNEL_S:%.S=$(OBJ_DIR)/%.o)

GAME_C := $(shell find $(GAME_DIR) -name "*.c")
GAME_S := $(shell find $(GAME_DIR) -name "*.S")
GAME_O := $(GAME_C:%.c=$(OBJ_DIR)/%.o)
GAME_O += $(GAME_S:%.S=$(OBJ_DIR)/%.o)

MY_SHELL_C := $(shell find $(MY_SHELL_DIR) -name "*.c")
MY_SHELL_S := $(shell find $(MY_SHELL_DIR) -name "*.S")
MY_SHELL_O := $(MY_SHELL_C:%.c=$(OBJ_DIR)/%.o)
MY_SHELL_O += $(MY_SHELL_S:%.S=$(OBJ_DIR)/%.o)

IDLE_C := $(shell find $(IDLE_DIR) -name "*.c")
IDLE_S := $(shell find $(IDLE_DIR) -name "*.S")
IDLE_O := $(IDLE_C:%.c=$(OBJ_DIR)/%.o)
IDLE_O += $(IDLE_S:%.S=$(OBJ_DIR)/%.o)

TESTCASE_C := $(shell find $(TESTCASE_DIR) -name "*.c")
TESTCASE_S := $(shell find $(TESTCASE_DIR) -name "*.S")
TESTCASE_O := $(TESTCASE_C:%.c=$(OBJ_DIR)/%.o)
TESTCASE_O += $(TESTCASE_S:%.S=$(OBJ_DIR)/%.o)

$(IMAGE): $(BOOT) $(KERNEL) data.disk
	@$(DD) if=/dev/zero of=$(IMAGE) count=5000         > /dev/null # 准备磁盘文件
	@$(DD) if=$(BOOT) of=$(IMAGE) conv=notrunc          > /dev/null # 填充 boot loader
	@$(DD) if=$(KERNEL) of=$(IMAGE) seek=1 conv=notrunc > /dev/null # 填充 kernel, 跨过 mbr
	@$(DD) if=data.disk of=$(IMAGE) seek=201 conv=notrunc   > /dev/null

data.disk : formatting.c $(MY_SHELL) $(MY_FILES)
	gcc $< -o formatting
	./formatting $@ $(MY_SHELL) $(MY_FILES)

$(BOOT): $(BOOT_O)
	$(LD) -e start -Ttext=0x7C00 -m elf_i386 -nostdlib -o $@.out $^
	$(OBJCOPY) --strip-all --only-section=.text --output-target=binary $@.out $@
	@rm $@.out
	boot/genboot.pl $@

$(OBJ_BOOT_DIR)/%.o: $(BOOT_DIR)/%.S
	@mkdir -p $(OBJ_BOOT_DIR)
	$(CC) $(CFLAGS) -Os $< -o $@

$(OBJ_BOOT_DIR)/%.o: $(BOOT_DIR)/%.c
	@mkdir -p $(OBJ_BOOT_DIR)
	$(CC) $(CFLAGS) -Os $< -o $@

$(KERNEL): $(KERNEL_LD_SCRIPT)
$(KERNEL): $(KERNEL_O) $(LIB_O)
	$(LD) -m elf_i386 -T $(KERNEL_LD_SCRIPT) -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
	kernel/genkernel.pl $@

$(MY_SHELL): $(MY_SHELL_O) $(LIB_O)
	$(LD) -e main -m elf_i386 -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

$(GAME) : $(GAME_LD_SCRIPT)
$(GAME) : $(GAME_O) $(LIB_O)
	$(LD) -m elf_i386 -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
	#$(LD) -m elf_i386 -T $(GAME_LD_SCRIPT) -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

$(IDLE): $(IDLE_O) $(LIB_O)
	$(LD) -m elf_i386 -e main -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
	idle/genidle.pl $@

$(TESTCASE) : $(TESTCASE_O) $(LIB_O)
	$(LD) -m elf_i386 -e main -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

$(OBJ_LIB_DIR)/%.o : $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_LIB_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_KERNEL_DIR)/%.o: $(KERNEL_DIR)/%.[cS]
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_GAME_DIR)/%.o: $(GAME_DIR)/%.[cS]
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_MY_SHELL_DIR)/%.o: $(MY_SHELL_DIR)/%.[cS]
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_IDLE_DIR)/%.o: $(IDLE_DIR)/%.[cS]
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_TESTCASE_DIR)/%.o: $(TESTCASE_DIR)/%.[cS]
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

DEPS := $(shell find -name "*.d")
-include $(DEPS)

.PHONY: qemu debug gdb clean

qemu: $(IMAGE)
	$(QEMU) $(QEMU_OPTIONS) $(IMAGE)

# Faster, but not suitable for debugging
qemu-kvm: $(IMAGE)
	$(QEMU) $(QEMU_OPTIONS) --enable-kvm $(IMAGE)

debug: $(IMAGE)
	$(QEMU) $(QEMU_DEBUG_OPTIONS) $(QEMU_OPTIONS) $(IMAGE)

gdb:
	$(GDB) $(GDB_OPTIONS)

clean:
	@rm -rf $(OBJ_DIR) 2> /dev/null
	@rm -rf $(BOOT)    2> /dev/null
	@rm -rf $(GAME)    2> /dev/null
	@rm -rf $(KERNEL)  2> /dev/null
	@rm -rf $(IMAGE)   2> /dev/null
	@rm -rf $(IDLE)	   2> /dev/null
	@rm -rf $(MY_SHELL) 2> /dev/null
	@rm -rf $(TESTCASE) 2> /dev/null

cleandisk:
	rm -rf data.disk  2> /dev/null

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

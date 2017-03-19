.code16               # 告诉 gcc 之后生成 16 位机器指令
                      # 开启保护模式之前 QEMU 只能执行 16 位机器指令

.globl start          # 将符号 start 暴露给链接器
start:                # start 表示代码的起始地址
  cli                 # 关中断
  seta20.1:
  inb     $0x64,%al               # Wait for not busy
  testb   $0x2,%al
  jnz     seta20.1

  movb    $0xd1,%al               # 0xd1 -> port 0x64
  outb    %al,$0x64

seta20.2:
  inb     $0x64,%al               # Wait for not busy
  testb   $0x2,%al
  jnz     seta20.2

  movb    $0xdf,%al               # 0xdf -> port 0x60
  outb    %al,$0x60
  mov $0x13, %ah      # 设置模式：显式字符串
  mov $0x01, %al      # 设置显式模式：(1) 使用 BL 指定的样式 (2) 更新光标
  mov $0x00, %bh      # 设置代码页，与字体相关； QEMU 下仅第一个代码页可用
  mov $0x4F, %bl      # 设置字符样式：红底白字无高亮
  mov $(end-str), %cx # 优雅地计算字符串的长度
  mov $str, %dx       # 将 ES:BP 指向字符串 (Step 1)
  mov %dx, %bp        # 将 ES:BP 指向字符串 (Step 2)
  mov $0x00, %dh      # 首字符输出在第 1 行
  mov $0x00, %dl      # 首字符输出在第 1 列
  int $0x10           # BIOS 中断，VGA 相关服务
loop:                 # loop 表示下面这条指令的首地址
  jmp loop            # 无限循环，防止继续执行超出内存

str:                  # str 表示字符串首地址
  .ascii "Hello, World!"
end:                  # end 表示字符串之后的地址，用于计算长度
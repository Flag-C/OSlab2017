OUTPUT_FORMAT(
    "elf32-i386",  /* Default       */
    "elf32-i386",  /* Big endian    */
    "elf32-i386"   /* Little endian */
)
OUTPUT_ARCH(i386)
ENTRY(game_main_loop)  /* entry */

SECTIONS
{
    . = 0x400000;

    PROVIDE(start = .);

    .text : AT(0x400000) {
        *(.text .text.*)
    }

    PROVIDE(etext = .);

    .rodata : {
        *(.rodata .rodata.*)
    }

    . = ALIGN(0x1000);

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    PROVIDE(end = .);
}

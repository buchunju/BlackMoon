ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS{
	
    . = 0x7e00;

    .text :  {
        *(.text*)
    }
    
    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        bss_begin = .;
        *(COMMON)
        *(.bss*)
        bss_end = .;
    }

    /DISCARD/ : {
        *(*)
    }

    end = .;
}
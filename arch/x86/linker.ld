ENTRY(_start)

SECTIONS {
	 . = 1M;

	 .boot :
	 {
		/* ensure that the multiboot header is at the beginning */
		*(.multiboot_header)
		. = ALIGN(4K);
	}

	.text :
	{
		*(.text)
		. = ALIGN(4K);
	}

	.bss :
	{
		*(.bss)
		. = ALIGN(4K);
	}

	.data :
	{
		*(.data)
		. = ALIGN(4K);
	}

	.rodata :
	{
		*(.rodata)
		. = ALIGN(4K);
	}
}

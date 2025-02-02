/* multiboot header constants [GRUB] */
.set ALIGN,  1<<0 
.set MEMINFO,  1<<1
.set FLAGS,  ALIGN | MEMINFO  /* 1 | 2 = 3*/
.set MAGIC, 0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

/* MULTIBOOT HEADER , marks prgoram as kernel.*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*stack setup, growing downwarrds, 4 byte alligned and setting esp ( stack pointer )  to top of stack*/

.section .bss
.align 16
stack_buttom:
.skip 16384 /*4mb stack*/
stack_top:
/*we are nearing entry point for kernel, bootloader jumps here once kernel loaded*/
.section .text
.global _start
.type _start, @function
_start:
	/*bootloader loaded into 32-bit protected mode on x86
	complete power over the machine in this kernel*/
	
	/*im settup up the stack, done this before in concurrency in mips so this is quite familiar*/
	mov $stack_top, %esp
	call kernel_main
	

	/*Infinite loop by disabling cli, wait for interrupt hlt locking up computer and jump to hlt if it wakes up, infinite loop*/
	cli
1:	hlt
	jmp 1b

/*.size of start is current location in assembly - start address of _start:*/
.size _start, . - _start

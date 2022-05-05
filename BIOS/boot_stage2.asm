BITS 16

section .text
EXTERN _dark_entry
GLOBAL _start

_start:
	jmp _stage2
	db  "MOON"   ; marker that this is stage 2
	
	STAGE2_MSG:   db "MOON",10,13,0
	%include "bios/print.inc"
	%include "bios/gdt.inc"

_stage2:
	xor ebx, ebx
	xor eax, eax
	xor ecx, ecx
	xor edx, edx
	mov si, ax
	mov di, ax
	mov sp, 0x7e00
	mov bp, sp
	; mov es, sp
	; mov ds, sp

	mov bx, STAGE2_MSG
	call print

	cli

	lgdt[gdt_descriptor]	; Load gdt
	
	mov eax, cr0 		
	or eax, 0x1		; Set bit 0 to 1
	mov cr0, eax
	
	jmp CODE_SEG:init_pm 	; 4. Far jump using a different segment


BITS 32

init_pm:
	mov ax, DATA_SEG 	; 5. Update segment registers
	mov ds, ax		
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000	; 6. update the stack right at the top of the free space
	mov esp, ebp

	call enable_A20

	mov ebx, BEGIN
	call print_pm

	call _dark_entry

	jmp $

enable_A20:
	call is_A20_on
	cmp eax, 1
	je finish
	call enable_a20_keyboard
	cmp eax, 1
	je finish
	call enable_a20_fast
	cmp eax, 1
	je finish
	mov ebx, A20_FAILED
	call print_pm

	jmp $

finish:
	ret

is_A20_on:   
 
	pushad
	mov edi,  0x112345  ;odd megabyte address.
	mov esi,  0x012345  ;even megabyte address.
	mov [esi],esi     ;making sure that both addresses contain diffrent values.
	mov [edi],edi     ;(if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi)) 
	cmpsd             ;compare addresses to see if the're equivalent.
	popad
	jne A20_on        ;if not equivalent , A20 line is set.
	mov eax, 0
	ret               ;if equivalent , the A20 line is cleared.
 
A20_on:

	mov eax, 1
	ret

; enable_a20_bios:

enable_a20_keyboard:
        cli
 
        call    a20wait
        mov     al,0xAD
        out     0x64,al
 
        call    a20wait
        mov     al,0xD0
        out     0x64,al
 
        call    a20wait2
        in      al,0x60
        push    eax
 
        call    a20wait
        mov     al,0xD1
        out     0x64,al
 
        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al
 
        call    a20wait
        mov     al,0xAE
        out     0x64,al
 
        call    a20wait
        sti
        call is_A20_on
        ret
 
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret

enable_a20_fast:
	in   al,  0x92
	test al,  2
	jnz  after
	or   al,  2
	and  al,  0xFE
	out  0x92,al
after:
	call is_A20_on
	ret

boot_device db 0

%include "bios/pm_print.inc"

A20_FAILED:   db "Failed: A20",0
BEGIN 		  db "PM Mode",0

times 510 - ($ - $$) db 0
db 0x41
db 0x41
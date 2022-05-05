BITS 16
org 0x7C00

section .bootsector

global _boot

_boot:
	jmp _begin
	nop

	;======== FAT32 BPB =====
	oem_name              : dq "ATLAS1.0"
	sector_size           : dw 512
	sector_per_cluster    : db 2
	reserved_sectors      : dw 6
	fat_number            : db 1
	files_in_root         : dw 0
	sectors_in_fs         : dw 0
	media_type            : db 0xf0
	fat_size_in_sectors   : dw 0
	sector_per_track      : dw 32
	head_count            : dw 64
	sectors_hidden        : dd 0
	sectors_in_fs_ext     : dd 200000
	fat_32_count          : dd 780
	ext_flags             : dw 0b0000000100000000
	fs_version            : dw 0
	root_cluster          : dd 3
	fs_info               : dw 0xFFFF
	backup_boot_sector    : dw 3
	reserved              : dw 6 dup(0)
	drive_number          : db 0
	unused                : db 0
	ext_boot_signature    : db 0x29
	volume_serial_number  : dd 0xdeadbeef
	volume_label          : db "ATLAS 1.0  "
	fs_type_label         : dq "FAT32   "

_begin:

	cli
	cld
	xor ax, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax
	mov es, ax
	mov sp, 0x7c00
	mov bp, sp

	mov [boot_device], dl

	mov bx, BOOT_MSG
	call print


loadFAT:
	mov ax, 0x44c
	mov [sector_loc], ax

disk_check:

	xor ax, ax
	xor ecx, ecx
	xor ebx, ebx
	xor dx, dx
	mov ax, 1 ; read one sector at a time
	mov cx, [sector_loc]

	mov dl, [boot_device]
	mov bx, 0x7E00
	mov esi, 0
	call _read_sectors

	mov DWORD ebx, [0x7e02] 
	cmp ebx, 0x4e4f4f4d ; maagic number
	je fin_check
	xor ebx, ebx

	; mov WORD dx, [0x7e02]
	; call print_hex

	mov ax, [loop_count]
	inc ax
	mov [loop_count], ax

	cmp WORD [loop_count], 0x20
	je not_found

	mov ax, [sector_loc]
	inc ax
	mov [sector_loc], ax

	jmp disk_check
	
fin_check:
	mov bx, FOUND
	call print

	xor ax, ax
	xor ecx,ecx
	xor ebx,ebx
	xor dx, dx
	mov ax, 42 ; number of sectors
	mov cx, [sector_loc]

	mov dl, [boot_device]
	mov bx, 0x7E00
	mov esi, 0
	call _read_sectors

	jmp 0x7e00
	jmp $

not_found:
	mov bx, STAGE_MISSING
	call print
	jmp $


sector_loc  dw 0
loop_count  dw 0
boot_device db 0

%include "bios/print.inc"
%include "bios/disk.inc"

BOOT_MSG      db "DARK",0
FOUND		  db "::", 0

STAGE_MISSING db "Missing stage2", 10, 13, 0

times 510 - ($ - $$) db 0
db 0x55
db 0xaa
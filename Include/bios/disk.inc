BITS 16
_read_sectors:
	mov [packet.count], ax
	mov [packet.buffer], ebx
	mov [packet.value],  cx
	mov si, packet
	mov ah, 0x42
	int 0x13
	ret

align 4
packet:
	.size		db 0x10
	.reserved	db 0x00
	.count		dw 0x00
	.buffer		dd 0x00
	.value		dq 0x00
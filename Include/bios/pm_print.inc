[bits 32]

%define VIDEO_MEMORY 0xB8000	; Location of VGA display
%define GREEN_ON_BLACK 0x02		; color byte for each character
%define COLUMNS 80

print_pm:
	pusha

	mov edi, VIDEO_MEMORY
	
print_string_pm_loop:
	mov al, [ebx]	; ebx is the address of character
	mov ah, GREEN_ON_BLACK

	cmp al, 0
	je print_string_pm_done

	mov word[edi], ax	; store char + attr in video memory
	add ebx, 1
	add edi, 2
	
	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret

	add ax, [OFFSE]
	ret

OFFSE dw 0
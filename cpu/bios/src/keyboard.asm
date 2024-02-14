; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

include io.asm

; No arguments
; Return:
; 	A - key code
external .xKeyboardGetChar:
	call .xKeyboardIsDataReady
	; Wait until keyboard buffer has not data
	cmp 0, A
	je .xKeyboardGetChar
	; Set keyboard register
	mov 0x04, A
	call .xIOSetRegister
	; Read keyboard register
	call .xIORead
	ret

; No arguments
; Return:
; 	A - status (0 - empty, 1 - ready to read)
external .xKeyboardIsDataReady:
	; Get keyboard interrupt bit
	in A
	and 0b00100000, A
	cmp 0, A
	jne .xKeyboardIsDataReady_ready
	ret
.xKeyboardIsDataReady_ready:
	mov 1, A
	ret

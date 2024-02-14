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
; No return
external .xLedSetRunOff:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write run led off command
	mov 0x00, A
	call .xIOWriteHighHalf
	pop A
	ret
	
; No arguments
; No return
external .xLedSetRunOn:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write run led on command
	mov 0x10, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xLedSetErrorOff:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write error led off command
	mov 0x20, A
	call .xIOWriteHighHalf
	pop A
	ret
	
; No arguments
; No return
external .xLedSetErrorOn:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write error led on command
	mov 0x30, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; Return:
; 	A - status (0 - off, 1 - on)
external .xLedGetRunStatus:
	mov 0x1, A
	call .xIOSetRegister
	clr A
	call .xIOWriteLowHalf
	call .xIOReadHighHalf
	ret

; No arguments
; Return:
; 	A - status (0 - off, 1 - on)
external .xLedGetErrorStatus:
	mov 0x1, A
	call .xIOSetRegister
	call .xIOWriteLowHalf
	call .xIOReadHighHalf
	ret

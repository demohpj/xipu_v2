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
global .xFsOpenOs:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write open OS command
	call .xIOWriteLowHalf
	pop A
	ret

; No arguments
; No return
external .xFsResetName:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	mov 0x1, A
	; Write reset name command
	call .xIOWriteLowHalf
	pop A
	ret
	
; No arguments
; No return
external .xFsOpenApp:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	mov 0x2, A
	; Write open app command
	call .xIOWriteLowHalf
	pop A
	ret
	
; No arguments
; No return
external .xFsOpenFile:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	mov 0x3, A
	; Write open file command
	call .xIOWriteLowHalf
	pop A
	ret

; No arguments
; No return
external .xFsListApp:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	mov 0x4, A
	; Write list app command
	call .xIOWriteLowHalf
	pop A
	ret

; No arguments
; No return
external .xFsListFile:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	mov 0x5, A
	; Write list file command
	call .xIOWriteLowHalf
	pop A
	ret

; No arguments
; Return:
; 	A - status (0 - open failed, 1 - ready to read)
external .xFsGetStatus:
	mov 0x1, A
	call .xIOSetRegister
	clr A
	call .xIOWriteLowHalf
	call .xIOReadLowHalf
	ret

; No arguments
; Return:
; 	Y - high byte of data size
; 	X - low byte of data size
external .xFSGetSize:
	push A
	mov 0x2, A
	call .xIOSetRegister
	clr A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	mov A, X
	mov 0x2, A
	call .xIOSetRegister
	mov 0x1, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	mov A, Y
	pop A
	ret

; Arguments:
; 	A - next char of name to set
; No return
external .xFSSetNextNameChar:
	push A
	mov 0xA, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; No arguments
; Return:
; 	A - next data byte
external .xFSGetNextDataByte:
	mov 0xA, A
	call .xIOSetRegister
	call .xIORead
	ret

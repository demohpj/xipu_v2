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
global .xLcdInit:
	call .xLcdClear
	mov 0xf, A
	call .xLcdSetForeground
	clr A
	call .xLcdSetBackground
	call .xLcdSetCursorPosX
	call .xLcdSetCursorPosY
	call .xLcdRefresh
	ret

; No arguments
; No return
external .xLcdClear:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write clear command
	mov 0x40, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xLcdRefresh:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write refresh command
	mov 0x50, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xLcdScrollDown:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write scroll down command
	mov 0x60, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xLcdNewLine:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write new line command
	mov 0x70, A
	call .xIOWriteHighHalf
	pop A
	ret

; Arguments:
; 	A - foreground color (4 bit)
; No return
external .xLcdSetForeground:
	push A
	; Set lcd color register
	mov 0x5, A
	call .xIOSetRegister
	pop A
	; Write foreground color
	call .xIOWriteLowHalf
	ret

; Arguments:
; 	A - background color (4 bit)
; No return
external .xLcdSetBackground:
	push A
	push A
	; Set lcd color register
	mov 0x5, A
	call .xIOSetRegister
	pop A
	; Swap to get value in high half
	swap A
	; Write background color
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; Return:
; 	A - horizontal cursor position
external .xLcdGetCursorPosX:
	; Set lcd cursor X position register
	mov 0x06, A
	call .xIOSetRegister
	; Read lcd cursor X position register
	call .xIORead
	ret

; No arguments
; Return:
; 	A - vertical cursor position
external .xLcdGetCursorPosY:
	; Set lcd cursor Y position register
	mov 0x07, A
	call .xIOSetRegister
	; Read lcd cursor Y position register
	call .xIORead
	ret

; Arguments:
; 	A - new horizontal cursor position
; No return
external .xLcdSetCursorPosX:
	push A
	; Set lcd cursor X position register
	mov 0x6, A
	call .xIOSetRegister
	pop A
	; Write X position
	call .xIOWrite
	ret

; Arguments:
; 	A - new vertical cursor position
; No return
external .xLcdSetCursorPosY:
	push A
	; Set lcd cursor Y position register
	mov 0x7, A
	call .xIOSetRegister
	pop A
	; Write Y position
	call .xIOWrite
	ret

; Arguments:
; 	A - print char on screen
; No return
external .xLcdPrintChar:
	push A
	; Set lcd char register
	mov 0x8, A
	call .xIOSetRegister
	pop A
	; Write char
	call .xIOWrite
	ret

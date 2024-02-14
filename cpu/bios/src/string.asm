; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

include lcd.asm

; Arguments:
; 	X - low address
;	Y - high address
; No return
global .xStringPrint:
	push X
	push Y
	push A
.xStringPrint_loop:
	; Load char from RAM
	ldp A
	; Check if it is end of string
	cmp 0, A
	je .xStringPrint_end
	; Print char
	call .xLcdPrintChar
	; Incrementing address
	incp
	jmp .xStringPrint_loop
.xStringPrint_end:
	pop A
	pop Y
	pop X
	ret













































































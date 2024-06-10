; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

map ../../../bin/sys/map/bios.map

; Arguments:
; 	X - low address
;	Y - high address
; No return
external .xStringPrint:
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

; Hex values table
const hexTable  '0', '1', '2', '3', '4', '5', '6', '7', \
				'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'

; Arguments:
; 	A - value
; No return
external .xStringPrintHex:
	push A
	push A
	gp *hexTable
	swap A
	and 0x0F, A
	addp A
	ldp A
	call .xLcdPrintChar
	pop A
	gp *hexTable
	and 0x0F, A
	addp A
	ldp A
	call .xLcdPrintChar
	pop A
	ret

; Multiplier values
const u8toDecTable 100, 10, 1

; Arguments:
; 	A - value
; No return
external .xStringPrintU8:
	enter
	alloc 2
	; Print and exit if it is zero
	cmp 0, A
	jne .xStringPrintU8_print
	push A
	mov '0', A
	call .xLcdPrintChar
	pop A
	leave
	ret
.xStringPrintU8_print:	
	push A
	push B
	push X
	push Y
	; Save input value
	stf A, 32
	; Init offset counter
	clr B
	; Reset let print flag
	stf B, 33
.xStringPrintU8_loop:
	push B
	gp *u8toDecTable
	addp B
	; Load value from dec table
	ldp B
	; Load current value
	ldf 32, A
	clr X
.xStringPrintU8_count:
	stf A, 32
	cmp B, A
	jg .xStringPrintU8_countEnd
	; Substract and increment the counter
	sub B, A
	inc X
	jmp .xStringPrintU8_count
.xStringPrintU8_countEnd:
	cmp 0, X
	jne .xStringPrintU8_countPrint
	ldf 33, Y
	cmp 0, Y
	je .xStringPrintU8_countNoPrint
.xStringPrintU8_countPrint:
	; Set let print flag
	stf 1, 33
	mov X, A
	add '0', A
	call .xLcdPrintChar
.xStringPrintU8_countNoPrint:
	pop B
	loope 2, B, .xStringPrintU8_loop
	pop Y
	pop X
	pop B
	pop A
	leave
	ret

; Multiplier values
const u16toDecTableLow 0x10, 0xe8, 0x64, 0x0a, 0x01
const u16toDecTableHigh 0x27, 0x03, 0x00, 0x00, 0x00

; Arguments:
; 	X - low value
;	Y - high value
; No return
external .xStringPrintU16:
	enter
	alloc 5
	; Print and exit if it is zero
	cmp 0, X
	jne .xStringPrintU16_print
	cmp 0, Y
	jne .xStringPrintU16_print
	push A
	mov '0', A
	call .xLcdPrintChar
	pop A
	leave
	ret
.xStringPrintU16_print:
	push A
	push B
	push X
	push Y
	; Save input value
	stf X, 32
	stf Y, 33
	; Init offset counter
	clr B
	; Reset let print flag
	stf B, 34
.xStringPrintU16_loop:
	push B
	; Load low value from dec table
	gp *u16toDecTableLow
	addp B
	ldp A
	stf A, 35
	; Load high value from dec table
	gp *u16toDecTableHigh
	addp B
	ldp A
	stf A, 36
	; Load current value
	ldf 32, X
	ldf 33, Y
	clr B
.xStringPrintU16_count:
	stf X, 32
	stf Y, 33
	ldf 36, A
	cmp A, Y
	jg .xStringPrintU16_countEnd
	jl .xStringPrintU16_continue
	ldf 35, A
	cmp A, X
	jg .xStringPrintU16_countEnd
.xStringPrintU16_continue:	
	; Substract and increment the counter
	ldf 35, A
	subp A
	ldf 36, A
	sub A, Y
	inc B
	jmp .xStringPrintU16_count
.xStringPrintU16_countEnd:
	cmp 0, B
	jne .xStringPrintU16_countPrint
	ldf 34, A
	cmp 0, A
	je .xStringPrintU16_countNoPrint
.xStringPrintU16_countPrint:
	; Set let print flag
	stf 1, 34
	mov B, A
	add '0', A
	call .xLcdPrintChar
.xStringPrintU16_countNoPrint:
	pop B
	loope 4, B, .xStringPrintU16_loop
	pop Y
	pop X
	pop B
	pop A
	leave
	ret

; No arguments
; No return
.xStringReadBackspace:
	push A
	; Erasing last char
	mov 0x00, A
	call .xLcdPrintChar
	; Load cursor position
	call .xLcdGetCursorPosX
	; Move left cursor
	sub 2, A
	; Save cursor position
	call .xLcdSetCursorPosX
	pop A
	ret

; No arguments
; No return
.xStringReadShowPrompt:
	push A
	; Print cursor
	mov 0xCF, A
	call .xLcdPrintChar
	; Load cursor position
	call .xLcdGetCursorPosX
	; Move left cursor
	dec A
	; Save cursor position
	call .xLcdSetCursorPosX
	pop A
	ret

; Arguments:
;	A - buffer length
; 	X - low address
;	Y - high address
; No return
external .xStringRead:
	; Push registers to stack
	push A
	push B
	push X
	push Y
	mov A, B
	call .xStringReadShowPrompt
	call .xLcdRefresh
.xStringRead_loop:
	; Push buffer length
	push A
	call .xKeyboardGetChar
	; Check for backspace
	cmp 0x08, A
	je .xStringRead_backspace
	; Check for enter
	cmp 0x0d, A
	je .xStringRead_enter
	; Check for not printable char
	; 0x20 > A
	cmp 0x20, A
	jg .xStringRead_notPrint
	; 0x7f <= A
	cmp 0x7f, A
	jle .xStringRead_notPrint
	; Respect buffer size limit
	; 1 >= B
	cmp 1, B
	jge .xStringRead_notPrint
	dec B
	; Print readed char
	call .xLcdPrintChar
	call .xStringReadShowPrompt
	call .xLcdRefresh
	; Save char to buffer
	stp A
	; Incrementing address
	incp
	; Pop buffer length
	pop A
	jmp .xStringRead_loop
.xStringRead_notPrint:
	; Pop buffer length
	pop A
	jmp .xStringRead_loop
.xStringRead_backspace:
	; Pop buffer length
	pop A
	; Do not go back too far
	cmp A, B
	je .xStringRead_loop
	inc B
	; Decrementing address
	decp
	; Moving back prompt
	call .xStringReadBackspace
	call .xStringReadShowPrompt
	call .xLcdRefresh
	jmp .xStringRead_loop
.xStringRead_enter:
	; Pop buffer length
	pop A
	; Add null terminator to buffer
	stp 0x00
	; Pop from stack
	pop Y
	pop X
	pop B
	pop A
	ret

; Arguments:
; 	X - low address
;	Y - high address
; Return:
;	A - length
external .xStringLength:
	push X
	push Y
	push B
	mov 0, A
.xStringLength_loop:
	ldp B
	; 0 == B
	cmp 0, B
	; End of string found
	je .xStringLength_end
	; Increment the string pointer
	incp
	; Increment string length
	inc A
	jmp .xStringLength_loop
.xStringLength_end:
	pop B
	pop Y
	pop X
	ret

; Arguments:
;	A - char to find
; 	X - low address
;	Y - high address
; Return:
;	A - offset
external .xStringFind:
	enter
	alloc 1
	push X
	push Y
	push B
	stf A, 32
	mov 0, A
.xStringFind_loop:
	push A
	ldf 32, A
	ldp B
	; A == B
	cmp A, B
	pop A
	; Char found
	je .xStringFind_end
	; 0 == B
	cmp 0, B
	; End of string found
	je .xStringFind_stringEnded
	; Increment the string pointer
	incp
	; Increment string length
	inc A
	jmp .xStringFind_loop
.xStringFind_stringEnded:
	mov 0, A
.xStringFind_end:
	pop B
	pop Y
	pop X
	leave
	ret

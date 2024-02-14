; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

; No arguments
; No return
global .xIOInit:
	push A
	push B
	push X
	; Set counter
	mov 0, A
	; Set last clock value
	mov 0, B
	; Wait until IO board is not started
.xIOInit_wait:
	in X
	and 0x10, X
	cmp B, X
	je .xIOInit_wait
	mov X, B
	inc A
	; Wait until 4 edges are detected
	cmp 4, A
	jne .xIOInit_wait
	pop X
	pop B
	pop A
	ret

; Arguments:
; 	A - register address
; No return
global .xIOSetRegister:
	push X
	push A
	; Get bus status bit
	in X
	and 0x80, X
	; Send register address
	out 0x00
	and 0x0f, A
	or 0b11000000, A
	out A
	; Wait for status change
.xIOSetRegister_wait:
	in A
	and 0x80, A
	cmp A, X
	je .xIOSetRegister_wait
	pop A
	pop X
	ret

; Arguments:
; 	A - data to transmit
; No return
global .xIOWriteLowHalf:
	push X
	push A
	; Get bus status bit
	in X
	and 0x80, X
	; Send low half
	out 0x00
	and 0x0f, A
	or 0b11100000, A
	out A
	; Wait for status change
.xIOWriteLowHalf_wait:
	in A
	and 0x80, A
	cmp A, X
	je .xIOWriteLowHalf_wait
	pop A
	pop X
	ret

; Arguments:
; 	A - data to transmit
; No return
global .xIOWriteHighHalf:
	push X
	push A
	; Get bus status bit
	in X
	and 0x80, X
	; Send high half
	out 0x00
	swap A
	and 0x0f, A
	or 0b11110000, A
	out A
	; Wait for status change
.xIOWriteHighHalf_wait:
	in A
	and 0x80, A
	cmp A, X
	je .xIOWriteHighHalf_wait
	pop A
	pop X
	ret

; Arguments:
; 	A - data to transmit
; No return
global .xIOWrite:
	call .xIOWriteLowHalf
	call .xIOWriteHighHalf
	ret

; No arguments
; Return:
; 	A - received data
global .xIOReadLowHalf:
	; Push registers to stack
	push X
	push B
	push A
	in X
	and 0x80, X
	; Read low half
	out 0x00
	and 0x0f, A
	or 0b10100000, A
	out A
	; Wait for status change
.xIOReadLowHalf_wait:
	; Get bus status bit
	in A
	mov A, B
	and 0x80, A
	; Wait until status bit is not toggled
	cmp A, X
	je .xIOReadLowHalf_wait
	; Set low half to return
	and 0x0f, B
	pop A
	and 0xf0, A
	or B, A
	pop B
	pop X
	ret

; No arguments
; Return:
; 	A - received data
global .xIOReadHighHalf:
	push X
	push B
	push A
	; Get bus status bit
	in X
	and 0x80, X
	; Read high half
	out 0x00
	and 0x0f, A
	or 0b10110000, A
	out A
	; Wait for status change
.xIOReadHighHalf_wait:
	; Get bus status bit
	in A
	mov A, B
	and 0x80, A
	; Wait until status bit is not toggled
	cmp A, X
	je .xIOReadHighHalf_wait
	; Set high half to return
	swap B
	and 0xf0, B
	pop A
	and 0x0f, A
	or B, A
	pop B
	pop X
	ret
	
; No arguments
; Return:
; 	A - received data
global .xIORead:
	call .xIOReadLowHalf
	call .xIOReadHighHalf
	ret

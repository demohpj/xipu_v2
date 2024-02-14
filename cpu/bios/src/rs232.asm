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
; 	A - received data
external .xRS232Receive:
	call .xRS232IsRxDataReady
	; Wait until rs232 rx buffer has not data
	cmp 0, A
	je .xRS232Receive
	; Set rs232 register
	mov 0x9, A
	call .xIOSetRegister
	; Read rs232 rx register
	call .xIORead
	ret

; Arguments:
; 	A - data to send
; No return
external .xRS232Send:
	push A
	; Set rs232 register
	mov 0x9, A
	call .xIOSetRegister
	pop A
	; Write rs232 tx register
	call .xIOWrite
	ret

; No arguments
; Return:
; 	A - status (0 - empty, 1 - ready to read)
external .xRS232IsRxDataReady:
	; Get rs232 rx interrupt bit
	in A
	and 0b01000000, A
	cmp 0, A
	jne .xRS232IsRxDataReady_ready
	ret
.xRS232IsRxDataReady_ready:
	mov 1, A
	ret

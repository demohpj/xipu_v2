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
external .xRTCRead:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write rtc read command
	mov 0x90, A
	call .xIOWriteHighHalf
	pop A
	ret
	
; No arguments
; No return
external .xRTCWrite:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write rtc write command
	mov 0x80, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; Return:
;	A - year
external .xRTCGetYear:
	mov 0x2, A
	call .xIOSetRegister
	mov 0x4, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; No arguments
; Return:
;	A - month
external .xRTCGetMonth:
	mov 0x2, A
	call .xIOSetRegister
	mov 0x5, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; No arguments
; Return:
;	A - day
external .xRTCGetDay:
	mov 0x2, A
	call .xIOSetRegister
	mov 0x6, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; No arguments
; Return:
;	A - hour
external .xRTCGetHour:
	mov 0x2, A
	call .xIOSetRegister
	mov 0x7, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; No arguments
; Return:
;	A - minute
external .xRTCGetMinute:
	mov 0x2, A
	call .xIOSetRegister
	mov 0x8, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; No arguments
; Return:
;	A - second
external .xRTCGetSecond:
	mov 0x2, A
	call .xIOSetRegister
	mov 0x9, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; Arguments:
;	A - year
; No return
external .xRTCSetYear:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0x4, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
;	A - month
; No return
external .xRTCSetMonth:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0x5, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
;	A - day
; No return
external .xRTCSetDay:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0x6, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
;	A - hour
; No return
external .xRTCSetHour:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0x7, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
;	A - minute
; No return
external .xRTCSetMinute:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0x8, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
;	A - second
; No return
external .xRTCSetSecond:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0x9, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

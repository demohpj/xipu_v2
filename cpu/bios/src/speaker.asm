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
global .xSpeakerInit:
	call .xSpeakerPause
	call .xSpeakerClear
	ret

; No arguments
; No return
external .xSpeakerPlay:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write speaker play command
	mov 0xB0, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xSpeakerPause:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write speaker pause command
	mov 0xC0, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xSpeakerClear:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write speaker clear command
	mov 0xD0, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; No return
external .xSpeakerAddNote:
	push A
	; Set command register
	clr A
	call .xIOSetRegister
	; Write speaker add note command
	mov 0xE0, A
	call .xIOWriteHighHalf
	pop A
	ret

; No arguments
; Return:
;	A - free space in buffer
external .xSpeakerGetBufferFree:
	; Set command register
	clr A
	call .xIOSetRegister
	; Write speaker add note command
	mov 0xE0, A
	call .xIOWriteHighHalf
	; Set field address register
	mov 0x2, A
	call .xIOSetRegister
	; Select speaker free buffer field
	mov 0xC, A
	call .xIOWriteLowHalf
	; Select field register
	mov 0x3, A
	call .xIOSetRegister
	call .xIORead
	ret

; Arguments:
; 	A - note
; No return
external .xSpeakerSetNote:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0xC, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
; 	A - time
; No return
external .xSpeakerSetTime:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0xD, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
; 	A - fill
; No return
external .xSpeakerSetFill:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0xE, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

; Arguments:
; 	A - volume
; No return
external .xSpeakerSetVolume:
	push A
	mov 0x2, A
	call .xIOSetRegister
	mov 0xF, A
	call .xIOWriteLowHalf
	mov 0x3, A
	call .xIOSetRegister
	pop A
	call .xIOWrite
	ret

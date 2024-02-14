; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

map ../../../../bin/sys/map/bios.map
map ../../../../bin/sys/map/os.map

; Start message
const messageArgs "Typed arguments:"

; First label is the entry point
.entry:
	call .main
	ret

; Arguments
;	X - Arguments address low
;	Y - Arguments address high
; No return
.main:
	push A
	push X
	push Y
	; Load address to start message
	gp *messageArgs
	; Print start message
	call .xStringPrint
	pop Y
	pop X
	call .xLcdNewLine
	call .xLcdNewLine
	mov '"', A
	call .xLcdPrintChar
	; Print arguments
	call .xStringPrint
	call .xLcdPrintChar
	pop A
	ret

; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

map ../../../bin/sys/map/bios.map

include terminal.asm

; Start message to show
const messageSystem "XiOS 2.0.0"
const messageFreeRam "52K memory free"

; First label is the entry point
.entry:
	; Print name and system version
	gp *messageSystem
	call .xStringPrint
	call .xLcdNewLine
	; Print information about RAM for apps
	gp *messageFreeRam
	call .xStringPrint
	call .xLcdNewLine
	call .xLcdRefresh
	call .main
	ret

; No arguments
; No return	
.main:
	; Show a terminal prompt
	call .terminal
	ret

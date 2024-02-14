; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

map ../../../../bin/sys/map/os.map

; First label is the entry point
.entry:
	call .main
	ret

; Arguments
;	X - Arguments address low
;	Y - Arguments address high
; No return
.main:
	; Reset the terminal
	call .xTerminalReset
	ret

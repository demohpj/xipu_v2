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

; 4 full rectangles to see color
const colorBar 0xCF, 0xCF, 0xCF, 0xCF, ' ', 0x00

; Color names
const color_0 "Black          "
const color_1 "Dark blue      "
const color_2 "Dark green     "
const color_3 "Dark cyan      "
const color_4 "Dark red       "
const color_5 "Dark pink      "
const color_6 "Dark yellow    "
const color_7 "Silver         "
const color_8 "Grey           "
const color_9 "Blue           "
const color_a "Green          "
const color_b "Cyan           "
const color_c "Red            "
const color_d "Pink           "
const color_e "Yellow         "
const color_f "White          "

; First label is the entry point
.entry:
	call .main
	ret

; No arguments
; No return
.main:
	push A
	; Clear loop counter
	clr A
.main_loop:
	; Show dark color on left
	call .printColor
	; Show light color on right
	xor 0b1000, A
	call .printColor
	xor 0b1000, A
	; After last color do not print new line
	cmp 0xF, A
	je .main_loopNoNewline
	call .xLcdNewLine
.main_loopNoNewline:
	loope 7, A, .main_loop
	; Refresh the screen
	call .xLcdRefresh
	pop A
	ret

; Arguments:
; 	A - color (4 bit)
; No return
.printColor:
	push A
	push X
	push Y
	; Color should be 4 bit
	cmp 0xF, A
	; 0xF < A
	jl .printColor_end
	; Set foreground to color from reg A
	call .xLcdSetForeground
	; Load address to color bar
	gp *colorBar
	; Print color bar with a sample of color
	call .xStringPrint
	push A
	; Set foreground color to default
	mov 0xF, A
	call .xLcdSetForeground
	pop A
	; Load address to color name
	gp *color_0
	; Every color name takes 16 bytes of memory
	swap A
	; Move address to current color name
	addp A
	; Print color name
	call .xStringPrint
.printColor_end:
	pop Y
	pop X
	pop A
	ret

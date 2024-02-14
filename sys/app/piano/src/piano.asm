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
const keyHelpHeader "Keymap:"

; Piano key description
const keyHelp00 " !   @       $   %   ^       *   (     "
const keyHelp01 "c#2 d#2     f#2 g#2 a#2     c#3 d#3    "
const keyHelp02 " 1   2   3   4   5   6   7   8   9   0 "
const keyHelp03 "c2  d2  e2  f2  g2  a2  b2  c3  d3  e3 "

const keyHelp04 " Q   W   E       T   Y       I   O   P "
const keyHelp05 "f#3 g#3 a#3     c#4 d#4     f#4 g#4 a#4"
const keyHelp06 " q   w   e   r   t   y   u   i   o   p "
const keyHelp07 "f3  g3  a3  b3  c4  d4  e4  f4  g4  a4 "

const keyHelp08 "     S   D       G   H   J       L     "
const keyHelp09 "    c#5 d#5     f#5 g#5 a#5     c#6    "
const keyHelp10 " a   s   d   f   g   h   j   k   l     "
const keyHelp11 "b4  c5  d5  e5  f5  g5  a5  b5  c6     "

const keyHelp12 " Z       C   V   B                     "
const keyHelp13 "d#6     f#6 g#6 a#6                    "
const keyHelp14 " z   x   c   v   b   n   m             "
const keyHelp15 "d6  e6  f6  g6  a6  b6  c7             "

; Bottom help message
const keyHelpExit "ESC - Exit           +- Duration: "
const keyHelpClear "      "
const keyHelpMs "0ms"

; Tone numbers connected to keyboard keys
const tone  0, 17,  0,  0, 22, 24,  0,  0, 31,  0, 29,  0,  0,  0,  0,  0, \
		   32, 16, 18, 20, 21, 23, 25, 27, 28, 30,  0,  0,  0,  0,  0,  0, \
		   19,  0, 74, 70, 55, 38,  0, 58, 60, 46, 62,  0, 65,  0,  0, 48, \
		   50, 34,  0, 53, 41,  0, 72, 36,  0, 43, 67,  0,  0,  0, 26,  0, \
		    0, 51, 73, 69, 54, 37, 56, 57, 59, 45, 61, 63, 64, 76, 75, 47, \
		   49, 33, 39, 52, 40, 44, 71, 35, 68, 42, 66,  0,  0,  0,  0,  0

; Set tone duration
var toneDuration

; First label is the entry point
.entry:
	call .main
	ret

; No arguments
; No return
.main:
	push A
	push B
	; Set default tone duration
	st 50, *toneDuration
	; Print start messages
	call .printKeyHelp
	; Print current tone duration
	call .printMs
	; Stop playing sounds
	call .xSpeakerClear
	; Set sound values to default
	mov 8, A
	call .xSpeakerSetFill
	mov 15, A
	call .xSpeakerSetVolume
.main_loop:
	call .xKeyboardGetChar
	; Exit if clicked key is ESC
	cmp 0x1b, A
	je .main_end
	; Check for increse tone duration command
	cmp '=', A
	je .main_durationPlus
	cmp '+', A
	je .main_durationPlus
	; Check for decrese tone duration command
	cmp '-', A
	je .main_durationMinus
	cmp '_', A
	je .main_durationMinus
	; Play tone based on clicked key
	call .playTone
	jmp .main_loop
	; Increse tone duration
.main_durationPlus:
	ld *toneDuration, B
	cmp 250, B
	; 250 <= B
	jle .main_loop
	add 5, B
	st B, *toneDuration
	; Print current tone duration
	call .printMs
	jmp .main_loop
	; Decrese tone duration
.main_durationMinus:
	ld *toneDuration, B
	cmp 5, B
	; 5 >= B
	jge .main_loop
	sub 5, B
	st B, *toneDuration
	; Print current tone duration
	call .printMs
	jmp .main_loop
.main_end:
	pop B
	pop A
	ret

; No arguments
; No return
.printKeyHelp:
	push A
	push B
	push X
	push Y
	; Load address to start message
	gp *keyHelpHeader
	; Print start message
	call .xStringPrint
	call .xLcdNewLine
	; Load address to 
	gp *keyHelp00
	; Reset loop counter
	clr B
.printKeyHelp_loop:
	; New line
	call .xLcdNewLine
	; Check if it is an even or odd line
	mov B, A
	and 0x1, A
	cmp 0x1, A
	jne .printKeyHelp_loopWhite
	; Set foregound color to green
	mov 0x0a, A
	call .xLcdSetForeground
	jmp .printKeyHelp_loopPrint
.printKeyHelp_loopWhite:
	; Set foreground color to default
	mov 0x0f, A
	call .xLcdSetForeground
.printKeyHelp_loopPrint:
	; Print line
	call .xStringPrint
	; Move address to next line
	add 40, X
	incc Y
	; Calc keyboard row number
	mov B, A
	and 0x3, A
	cmp 0x3, A
	jne .printKeyHelp_loopNoNewLine
	; New line if it is a keyboard row was printed
	call .xLcdNewLine
.printKeyHelp_loopNoNewLine:
	; Repeat print lines 15+1 times
	loope 15, B, .printKeyHelp_loop
	; Set foreground color to default
	mov 0x0f, A
	call .xLcdSetForeground
	call .xLcdNewLine
	call .xLcdNewLine
	; Load address to bottom message
	gp *keyHelpExit
	; Print bottom message
	call .xStringPrint
	; Refresh the screen
	call .xLcdRefresh
	pop Y
	pop X
	pop B
	pop A
	ret

; No arguments
; No return
.printMs:
	push A
	push X
	push Y
	; Move print start at begin of the tone duration field
	mov 34, A
	call .xLcdSetCursorPosX
	; Load address to empty value
	gp *keyHelpClear
	; Clear field by empty value
	call .xStringPrint
	; Move print start at begin of the tone duration field
	call .xLcdSetCursorPosX
	; Load tone duration
	ld *toneDuration, A
	; Print new tone duration
	call .xStringPrintU8
	; Load address to second part of tone duration string
	gp *keyHelpMs
	; Print second part of tone duration string
	call .xStringPrint
	; Refresh the screen
	call .xLcdRefresh
	pop Y
	pop X
	pop A
	ret

; Arguments
; 	A - keyboard key
; No return
.playTone:
	push A
	push X
	push Y
	; Make sure the key is beetwen 0x20 and 0x7f
	cmp 0x20, A
	; 0x20 > A
	jg .playTone_end
	cmp 0x7f, A
	; 0x7f < A
	jl .playTone_end
	; Make offset
	sub 0x20, A
	; Load address to tone table
	gp *tone
	; Add offset to tone
	add A, X
	incc Y
	; Load tone number
	ldp A
	; Do not play empty tone
	cmp 0, A
	; 0 == A
	je .playTone_end
	call .xSpeakerSetNote
	; Load tone duration
	ld *toneDuration, A
	; Write tone duration
	call .xSpeakerSetTime
	; Stop playing sounds
	call .xSpeakerClear
	; Add tone to buffer
	call .xSpeakerAddNote
	; Play tone from buffer
	call .xSpeakerPlay
.playTone_end:
	pop Y
	pop X
	pop A
	ret

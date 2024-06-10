; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

include io.asm
include keyboard.asm
include led.asm
include lcd.asm
include rs232.asm
include rtc.asm
include speaker.asm
include fs.asm
include string.asm

; BIOS version
const biosVersionText "XiPU BIOS 2.0.0"

; XiPC logo data
const logoData 0x00, 0x01, 0x80, 0x00, 0x00, \
			   0x7C, 0x79, 0xBF, 0xE0, 0xFA, \		
			   0x7C, 0x79, 0xBF, 0xE0, 0xFA, \
			   0x38, 0x31, 0x9C, 0x73, 0x8E, \
			   0x1C, 0x20, 0x0C, 0x77, 0x06, \
			   0x1E, 0x60, 0x0C, 0x3E, 0x02, \
			   0x0E, 0xC7, 0x8C, 0x3E, 0x02, \
			   0x07, 0x83, 0x8C, 0x7C, 0x00, \
			   0x03, 0x81, 0x8F, 0xEC, 0x00, \
			   0x03, 0x81, 0x8F, 0xEC, 0x00, \
			   0x07, 0xC1, 0x8C, 0x0C, 0x00, \
			   0x0C, 0xE1, 0x8C, 0x0E, 0x00, \
			   0x08, 0xE1, 0x8C, 0x0E, 0x02, \
			   0x18, 0x71, 0x8C, 0x07, 0x04, \
			   0x30, 0x3B, 0xDE, 0x03, 0xFC, \
			   0x7C, 0xFF, 0xFF, 0x00, 0xF8

; XiPC logo data length
const logoDataLength 75

; Messages to display
const loadingSearchText "Searching for os.bin..."
const loadingStartText "Loading OS."
const loadingDoneText "Done"
const loadingErrorNoFileText "Error: The OS file not found"
const loadingErrorEmptyText "Error: The OS file is empty"
const loadingErrorTooBigText "Error: The OS file is too large"

; First label is the entry point
.entry:
	; Reset the Stack Pointer
	rstsp
	; Init IO data bus
	call .xIOInit
	; Reset lcd
	call .xLcdInit
	; Reset speaker
	call .xSpeakerInit
	call .main
	ret

; No arguments
; No return
.main:
	; Turn off the run led
	call .xLedSetRunOff
	; Turn off the error led
	call .xLedSetErrorOff
	clr A
	call .xLcdSetCursorPosX
	mov 2, A
	call .xLcdSetCursorPosY
	call .printLogo
	clr A
	call .xLcdSetCursorPosX
	mov 20, A
	call .xLcdSetCursorPosY
	call .printBiosVersion
	call .playSound
	; Turn on the run led
	call .xLedSetRunOn
	; Load the OS to the memory
	call .loadOs
	; Turn off the run led
	call .xLedSetRunOff
	; Run OS
	call 0x0800
	; Halt execution if it exits OS
	halt

; No arguments
; No return
.printBiosVersion:
	gp *biosVersionText
	call .xStringPrint
	call .xLcdRefresh
	call .xLcdNewLine
	ret

; No arguments
; No return
.printLogo:
	ld *logoDataLength, B
	dec B
	gp *logoData
.printLogo_dataLoop:
	ldp A
	incp
	push B
	mov 7, B
.printLogo_pixelLoop:
	push A
	and 0x80, A
	cmp 0x80, A
	je .printLogo_pixel1
.printLogo_pixel0:
	mov 0x00, A
	call .xLcdPrintChar
	jmp .printLogo_continue
.printLogo_pixel1:
	mov 0xCF, A
	call .xLcdPrintChar
.printLogo_continue:
	pop A
	shl A
	loopz B, .printLogo_pixelLoop
	pop B
	loopz B, .printLogo_dataLoop
	call .xLcdRefresh
	ret

; No arguments
; No return
.playSound:
	; Set fill parameter
	mov 8, A
	call .xSpeakerSetFill
	; Set volume parameter
	mov 8, A
	call .xSpeakerSetVolume
	; Set time parameter
	mov 24, A
	call .xSpeakerSetTime
	; Play e6 tone
	mov 68, A
	call .xSpeakerSetNote
	call .xSpeakerAddNote
	; Play e5 tone
	mov 56, A
	call .xSpeakerSetNote
	call .xSpeakerAddNote
	call .xSpeakerPlay
	ret

; No arguments
; No return
.loadOs:
	enter
	alloc 3
	gp *loadingSearchText
	call .xStringPrint
	call .xLcdRefresh
	call .xLcdNewLine
.loadOs_openFile:
	call .xFsOpenOs
	call .xFsGetStatus
	cmp 0, A
	je .loadOs_errorNoFile
	call .xFSGetSize
	cmp 0, Y
	jne .loadOs_openNotEmpty
	cmp 0, X
	jne .loadOs_openNotEmpty
	jmp .loadOs_errorEmpty
.loadOs_openNotEmpty:
	cmp 0x18, Y ; 6KB = 0x18 * 256
	jg .loadOs_loadFile
	jl .loadOs_errorTooLarge
	cmp 0, X
	jne .loadOs_errorTooLarge
.loadOs_loadFile:
	; save file size on stack
	decp
	stf X, 32
	stf Y, 33
	gp *loadingStartText
	call .xStringPrint
	call .xLcdRefresh
	; save dest address on stack
	mov 0x08, A
	stf A, 34
	ldf 33, Y
.loadOs_loadFileCopyBlockLoop:
	stf Y, 33
	cmp 0, Y
	je .loadOs_loadFileCopyBlockLoopPart
.loadOs_loadFileCopyBlockLoopFull:
	mov 0xFF, B
	jmp .loadOs_loadFileCopyDataLoopInit
.loadOs_loadFileCopyBlockLoopPart:
	ldf 32, B
.loadOs_loadFileCopyDataLoopInit:
	ldf 34, Y
	clr X
.loadOs_loadFileCopyDataLoop:
	call .xFSGetNextDataByte
	stp A
	incp
	loopz B, .loadOs_loadFileCopyDataLoop
	stf Y, 34
	mov '.', A
	call .xLcdPrintChar
	call .xLcdRefresh
	ldf 33, Y
	loopz Y, .loadOs_loadFileCopyBlockLoop
.loadOs_done:
	call .xLcdNewLine
	gp *loadingDoneText
	call .xStringPrint
	call .xLcdRefresh
	call .xLcdNewLine
	call .xLcdNewLine
	leave
	ret

; Show no file found error message
.loadOs_errorNoFile:
	gp *loadingErrorNoFileText
	call .xStringPrint
	call .xLcdRefresh
	halt

; Show empty file error message
.loadOs_errorEmpty:
	gp *loadingErrorEmptyText
	call .xStringPrint
	call .xLcdRefresh
	halt

; Show too large file error message
.loadOs_errorTooLarge:
	gp *loadingErrorTooBigText
	call .xStringPrint
	call .xLcdRefresh
	halt

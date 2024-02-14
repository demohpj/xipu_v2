; Author: Pawel Jablonski
; E-mail: pj@xirx.net
; WWW: xirx.net
; GIT: git.xirx.net
;
; License: You can use this code however you like
; but leave information about the original author.
; Code is free for non-commercial and commercial use.

map ../../../bin/sys/map/bios.map

include string.asm

; Command buffer
var commandBuffer[39]

; No arguments
; No return	
global .terminal:
	; Get command
	call .terminalGetCommand
	; Check command length
	gp *commandBuffer
	call .xStringLength
	; 0 == A
	cmp 0, A
	je .terminal
	; Run app if command is not empty
	call .terminalRunApp
	jmp .terminal

; No arguments
; No return
external .xTerminalReset:
	push A
	; Reset color
	call .xTerminalColorReset
	; Clear screen
	call .xLcdClear
	; Reset cursor position
	clr A
	call .xLcdSetCursorPosX
	call .xLcdSetCursorPosY
	pop A
	ret

; No arguments
; No return
external .xTerminalColorReset:
	push A
	; Change colors to default
	mov 0x0, A
	call .xLcdSetBackground
	mov 0xF, A
	call .xLcdSetForeground
	pop A
	ret

; No arguments
; No return
.terminalGetCommand:
	; Reset color
	call .xTerminalColorReset
	; Do not make a new line if the cursor is at beginning
	call .xLcdGetCursorPosY
	cmp 0, A
	je .terminalGetCommandNoNewLine
	call .xLcdNewLine
	call .xLcdNewLine
.terminalGetCommandNoNewLine:
	; Show command line
	mov '>', A
	call .xLcdPrintChar
	call .xLcdRefresh
	mov 39, A
	gp *commandBuffer
	call .xStringRead
	call .xLcdNewLine
	ret

; Error messages for run app
const appNameLengthError "Error: Bad length of app name"
const appNameUnknownNameError "Error: Unknown app"

; No arguments
; No return
.terminalRunApp:
	enter
	alloc 2
	push A
	push X
	push Y
	; Load command buffer address
	gp *commandBuffer
	; Load first char
	ldp A
	; First char cannot be a space
	cmp ' ', A
	je .terminalRunApp_appNameLengthError
	; Find a space after command
	mov ' ', A
	call .xStringFind
	; If the space is not found then it is no arguments
	cmp 0, A
	je .terminalRunApp_no_arg
	; Move address at space after app name
	addp A
	; Replace a space by end of string char
	stp 0x00
	; Move address at begin of arguments
	incp
	call .xStringLength
	; It is no arguments if string length is equal zero
	cmp 0, A
	je .terminalRunApp_no_arg
	jmp .terminalRunApp_checkAppName
	; Set arguments address for case where no argument was found
.terminalRunApp_no_arg:
	; Load command buffer address
	gp *commandBuffer
	call .xStringLength
	; Move address at end+1 of app name
	inc A
	addp A
	; Store end of string char
	stp 0x00
	; Check app name
.terminalRunApp_checkAppName:
	; Store arguments address to local variables
	stf X, 32
	stf Y, 33
	; Load command buffer address
	gp *commandBuffer
	; Check app name length
	call .xStringLength
	; If app name lenght is equal to zero then show error message
	cmp 0, A
	je .terminalRunApp_appNameLengthError
	; If app name lenght is greather than 8 then show error message
	cmp 8, A
	; 8 < A
	jl .terminalRunApp_appNameLengthError
	; Run load app procedure
	call .terminalLoadApp
	; If load app procedure returned flase then show error message
	cmp 0, A
	je .terminalRunApp_loadFail
	; Make a new line before app output
	call .xLcdNewLine
	; Load args address
	ldf 32, X
	ldf 33, Y
	; Run app
	call 0x2000
	pop Y
	pop X
	pop A
	leave
	ret

; Show length error message
.terminalRunApp_appNameLengthError:
	call .xLcdNewLine
	gp *appNameLengthError
	call .xStringPrint
	pop Y
	pop X
	pop A
	leave
	ret

; Show unknown name of app error message
.terminalRunApp_loadFail:
	pop Y
	pop X
	pop A
	leave
	ret

; Error messages for loading app
const loadingErrorNoFileText "Error: The app file not found"
const loadingErrorEmptyText "Error: The app file is empty"
const loadingErrorTooBigText "Error: The app file is too large"

; No arguments
; Return:
; 	A - success (0 - error, 1 - ok)
.terminalLoadApp:
	enter
	alloc 3
	push B
	push X
	push Y
.terminalLoadApp_setName:
	call .xFsResetName
	gp *commandBuffer
.terminalLoadApp_setNameLoop:
	ldp A
	call .xFSSetNextNameChar
	incp
	cmp 0, A
	jne .terminalLoadApp_setNameLoop
.terminalLoadApp_openFile:
	call .xFsOpenApp
	call .xFsGetStatus
	cmp 0, A
	je .terminalLoadApp_errorNoFile
	call .xFSGetSize
	cmp 0, Y
	jne .terminalLoadApp_openNotEmpty
	cmp 0, X
	jne .terminalLoadApp_openNotEmpty
	jmp .terminalLoadApp_errorEmpty
.terminalLoadApp_openNotEmpty:
	cmp 0xD0, Y ; 52KB = 0xD0 * 256
	jg .terminalLoadApp_loadFile
	jl .terminalLoadApp_errorTooLarge
	cmp 0, X
	jne .terminalLoadApp_errorTooLarge
.terminalLoadApp_loadFile:
	; save file size on stack
	dec X
	stf X, 32
	decc Y
	stf Y, 33
	; save dest address on stack
	mov 0x20, A
	stf A, 34
	ldf 33, Y
.terminalLoadApp_loadFileCopyBlockLoop:
	stf Y, 33
	cmp 0, Y
	je .terminalLoadApp_loadFileCopyBlockLoopPart
.terminalLoadApp_loadFileCopyBlockLoopFull:
	mov 0xFF, B
	jmp .terminalLoadApp_loadFileCopyDataLoopInit
.terminalLoadApp_loadFileCopyBlockLoopPart:
	ldf 32, B
.terminalLoadApp_loadFileCopyDataLoopInit:
	ldf 34, Y
	clr X
.terminalLoadApp_loadFileCopyDataLoop:
	call .xFSGetNextDataByte
	stp A
	incp
	loopz B, .terminalLoadApp_loadFileCopyDataLoop
	stf Y, 34
	ldf 33, Y
	loopz Y, .terminalLoadApp_loadFileCopyBlockLoop
.terminalLoadApp_done:
	pop Y
	pop X
	pop B
	mov 1, A
	leave
	ret

; Show no file found error message
.terminalLoadApp_errorNoFile:
	call .xLcdNewLine
	gp *loadingErrorNoFileText
	call .xStringPrint
	pop Y
	pop X
	pop B
	mov 0, A
	leave
	ret

; Show empty file error message
.terminalLoadApp_errorEmpty:
	gp *loadingErrorEmptyText
	call .xStringPrint
	pop Y
	pop X
	pop B
	mov 0, A
	leave
	ret

; Show too large file error message
.terminalLoadApp_errorTooLarge:
	gp *loadingErrorTooBigText
	call .xStringPrint
	pop Y
	pop X
	pop B
	mov 0, A
	leave
	ret

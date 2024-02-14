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
const appListHeader "App list:"

; Error messages
const appListIsEmpty "Warning: App list is empty"
const appListOpenError "Error: Cannot open the app dir"

; Read buffer to store one sector
var readBuffer[256]

; App quantity in app dir
var appQuantity

; App counter
var appCount

; First label is the entry point
.entry:
	call .main
	ret

; No arguments
; No return
.main:
	push A
	push B
	push X
	push Y
	; List app
	call .xFsListApp
	call .xFsGetStatus
	cmp 0, A
	je .main_errorOpen
	; Get list size
	call .xFSGetSize
	; Do not process empty list
	mov X, A
	mov Y, B
	or A, B
	; 0 == A
	cmp 0, B
	je .main_warningEmpty
	; Save app quantity
	swap A
	and 0x0F, A
	swap Y
	and 0xF0, Y
	or A, Y
	st Y, *appQuantity
	st 0, *appCount
	; Print header
	gp *appListHeader
	call .xStringPrint
	call .xLcdNewLine
	call .xLcdNewLine
	; Run copy loop B+1 times
	; Set starting block
	ld *appQuantity, B
	swap B
	and 0x0F, B
.main_blockLoop:
	push B
	; Set buffer address
	gp *readBuffer
	cmp 0, B
	jne .main_loopFull
	; Read only part data what left to read
	ld *appQuantity, B
	swap B
	and 0xF0, B
	dec B
	jmp .main_loop
.main_loopFull:
	mov 0xFF, B	
.main_loop:
	call .xFSGetNextDataByte
	stp A
	incp
	loopz B, .main_loop
	; Print apps from block
	call .printBlock
	pop B
	loopz B, .main_blockLoop
.main_loopExit:
	; Refresh the screen
	call .xLcdRefresh
	pop Y
	pop X
	pop B
	pop A
	ret

; Show list open error message
.main_errorOpen:
	; Print open error
	gp *appListOpenError
	call .xStringPrint
	; Refresh the screen
	call .xLcdRefresh
	pop Y
	pop X
	pop B
	pop A
	ret

; Show empty list warning message
.main_warningEmpty:
	; Print empty list message
	gp *appListIsEmpty
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
.printBlock:
	push A
	push B
	push X
	push Y
	; Set buffer address
	gp *readBuffer
	mov 15, B
	; Run print loop B+1 times
.printBlock_loop:
	; Print app name
	call .printName
	; Exit loop if all apps was printed
	push B
	xchg *appCount, A
	ld *appQuantity, B
	inc A
	cmp A, B
	pop B
	xchg *appCount, A
	jge .printBlock_loopExit
	; Go to next entry
	addp 16
	loopz B, .printBlock_loop
.printBlock_loopExit:
	pop Y
	pop X
	pop B
	pop A
	ret

; No arguments
; No return
.printName:
	push A
	push B
	; Print a comma if it is not a first name
	ld *appCount, A
	cmp 0, A
	je .printName_noComma
	mov '\,', A
	call .xLcdPrintChar
	mov ' ', A
	call .xLcdPrintChar
.printName_noComma:
	; Check if it is enough space for next name in line
	call .xStringLength
	mov A, B
	call .xLcdGetCursorPosX
	add B, A
	; 40 > A
	cmp 39, A
	jg .printName_noNewLine
	; Print name in new line
	call .xLcdNewLine
.printName_noNewLine:
	; Print name
	call .xStringPrint
	pop B
	pop A
	ret

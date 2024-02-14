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
const fileListHeader "File list:"

; Error messages
const fileListIsEmpty "Warning: File list is empty"
const fileListOpenError "Error: Cannot open the file dir"

; Read buffer to store one sector
var readBuffer[256]

; App quantity in app dir
var fileQuantity

; App counter
var fileCount

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
	; List file
	call .xFsListFile
	call .xFsGetStatus
	cmp 0, A
	je .main_errorOpen
	; Get list size
	call .xFSGetSize
	; Do not process empty list
	mov X, A
	mov Y, B
	or A, B
	; 0 == B
	cmp 0, B
	je .main_warningEmpty
	; Save file quantity
	swap A
	and 0x0F, A
	swap Y
	and 0xF0, Y
	or A, Y
	st Y, *fileQuantity
	st 0, *fileCount
	; Print header
	gp *fileListHeader
	call .xStringPrint
	;call .xLcdNewLine
	call .xLcdNewLine
	; Run copy loop B+1 times
	; Set starting block
	ld *fileQuantity, B
	swap B
	and 0x0F, B
.main_blockLoop:
	push B
	; Set buffer address
	gp *readBuffer
	cmp 0, B
	jne .main_loopFull
	; Read only part data what left to read
	ld *fileQuantity, B
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
	gp *fileListOpenError
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
	gp *fileListIsEmpty
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
	; Print file name
	call .printName
	; Exit loop if all files was printed
	push B
	xchg *fileCount, A
	ld *fileQuantity, B
	inc A
	cmp A, B
	pop B
	xchg *fileCount, A
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
	push X
	push Y
	ld *fileCount, A
	and 0x1, A
	cmp 0, A
	jne .printName_noNewLine
	; Print name in new line
	call .xLcdNewLine
	jmp .printName_noOffset
.printName_noNewLine:
	mov 20, A
	call .xLcdSetCursorPosX
.printName_noOffset:
	; Print name
	call .xStringPrint
	mov ' ', A
	call .xLcdPrintChar
	addp 14
	ldp A
	incp
	ldp B
	mov A, X
	mov B, Y
	call .xStringPrintU16
	pop Y
	pop X
	pop B
	pop A
	ret

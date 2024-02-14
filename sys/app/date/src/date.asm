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

; Date change message
const changedMessage "Date has been changed"

; Error messages
const errorMessageLine0 "Date has wrong format"
const errorMessageLine1 "Please use: YYMMDDhhmmss"

; Prefix for year
const yearPrefix "20"

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
	; Check length of arguments
	call .xStringLength
	; Print current date and time if argument list is empty
	cmp 0, A
	je .main_print
	; Parse arguments
	call .parseDate
	; Write new date nad time if arguments are correct
	cmp 0, A
	jne .main_write
	; Load address to first line of error message
	gp *errorMessageLine0
	; Print first line of error message
	call .xStringPrint
	call .xLcdNewLine
	call .xLcdNewLine
	; Load address to second line of error message
	gp *errorMessageLine1
	; Print second line of error message
	call .xStringPrint
	pop Y
	pop X
	pop A
	ret
	; Write date and time to RTC
.main_write:
	; Write date and time to RTC buffers
	call .writeDate
	; Write date and time from RTC buffers to RTC
	call .xRTCWrite
	; Load address to message
	gp *changedMessage
	; Print message
	call .xStringPrint
	call .xLcdNewLine
	call .xLcdNewLine
	; Print date and time from RTC
.main_print:
	; Read date and time from RTC to RTC buffers
	call .xRTCRead
	; Print current date and time
	call .printDate
	pop Y
	pop X
	pop A
	ret

; Buffer for parse and write
var year
var month
var day
var hour
var minute
var second

; No arguments
; Return:
; 	A - success (0 - error, 1 - ok)
.parseDate:
	push B
	push X
	push Y
	; Check length of arguments
	call .xStringLength
	; Good input should has 12 characters
	cmp 12, A
	jne .parseDate_error
	; Good input should has only digits
	call .parseDigits
	cmp 0, A
	je .parseDate_error
	; Converting two digits to year
	call .convertTwoDigits
	st A, *year
	; Move to next field
	addp 2
	; Converting two digits to month
	call .convertTwoDigits
	; Months are 1 to 12
	cmp 1, A
	; 1 > A
	jg .parseDate_error
	cmp 12, A
	; 12 < A
	jl .parseDate_error
	st A, *month
	; Move to next field
	addp 2
	; Converting two digits to day
	call .convertTwoDigits
	; Days start from 1
	cmp 1, A
	; 1 > A
	jg .parseDate_error
	mov A, B
	push B
	; Calculating quantity of days for selected year and month
	ld *year, A
	ld *month, B
	call .getDaysInMonth
	pop B
	xchg A, B
	; Day number could not be higher than quantity of days in month
	cmp B, A
	; B < A
	jl .parseDate_error
	st A, *day
	; Move to next field
	addp 2
	; Hours are 0 to 23
	call .convertTwoDigits
	cmp 23, A
	; 23 < A
	jl .parseDate_error
	st A, *hour
	; Move to next field
	addp 2
	; Minutes are 0 to 59
	call .convertTwoDigits
	cmp 59, A
	; 59 < A
	jl .parseDate_error
	st A, *minute
	; Move to next field
	addp 2
	; Seconds are 0 to 59
	call .convertTwoDigits
	cmp 59, A
	; 59 < A
	jl .parseDate_error
	st A, *second
	mov 1, A
	pop Y
	pop X
	pop B
	ret
	; Parse ends with error
.parseDate_error:
	mov 0, A
	pop Y
	pop X
	pop B
	ret

; Days in month table
const daysNumber 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31

; Arguments:
;	A - year
;	B - month
; Return:
;	A - days in month
.getDaysInMonth:
	push B
	push X
	push Y
	; Check an extra day for February
	cmp 2, B
	jne .getDaysInMonth_notLeapYear
	; Years divisible by 4 are leap years 
	and 0b11, A
	cmp 0, A
	jne .getDaysInMonth_notLeapYear
	; Set 29 days if it is Februray in leap year
	mov 29, A
	jmp .getDaysInMonth_end
	; Select days quanity of month
.getDaysInMonth_notLeapYear:
	gp *daysNumber
	dec B
	addp B
	ldp A
.getDaysInMonth_end:
	pop Y
	pop X
	pop B
	ret

; Arguments:
; 	X - low address
;	Y - high address
; Return:
; 	A - success (0 - error, 1 - ok)
.parseDigits:
	push B
	push X
	push Y
	; Clear loop counter
	clr B
	; Check every char
.parseDigits_Loop:
	; Load char from buffer
	ldp A
	; It must be '0' or higher
	cmp '0', A
	; '0' > A
	jg .parseDigits_Error
	; It must be '9' or lower
	cmp '9', A
	; '9' < A
	jl .parseDigits_Error
	; Move to next char
	incp
	; Repeat 11+1 times
	loope 11, B, .parseDigits_Loop
	; All chars are digits
	mov 1, A
	pop Y
	pop X
	pop B
	ret
	; One or more chars are not digit
.parseDigits_Error:
	mov 0, A
	pop Y
	pop X
	pop B
	ret

; Converting table number from string to hex
const convertTable 00, 10, 20, 30, 40, 50, 60, 70, 80, 90

; Arguments:
; 	X - low address
;	Y - high address
; Return:
; 	A - number
.convertTwoDigits:
	push B
	push X
	push Y
	; Load first digit
	ldp B
	; Convert from char to number
	sub '0', B
	; Move to next digit
	incp
	; Load second digit
	ldp A
	; Convert from char to number
	sub '0', A
	; Set address to converting table
	gp *convertTable
	; Calc number to take
	addp B
	; Load number from table
	ldp B
	; Add number from table to the second digit
	add B, A
	pop Y
	pop X
	pop B
	ret

; No arguments
; No return
.writeDate:
	push A
	; Load year from buffer
	ld *year, A
	; Write year to RTC buffer
	call .xRTCSetYear
	; Load month from buffer
	ld *month, A
	; Write month to RTC buffer
	call .xRTCSetMonth
	; Load day from buffer
	ld *day, A
	; Write day to RTC buffer
	call .xRTCSetDay
	; Load hour from buffer
	ld *hour, A
	; Write hour to RTC buffer
	call .xRTCSetHour
	; Load minute from buffer
	ld *minute, A
	; Write minute to RTC buffer
	call .xRTCSetMinute
	; Load second from buffer
	ld *second, A
	; Write second to RTC buffer
	call .xRTCSetSecond
	pop A
	ret

; No arguments
; No return
.printDate:
	push A
	push X
	push Y
	; Load address to prefix
	gp *yearPrefix
	; Print prefix
	call .xStringPrint
	; Read year
	call .xRTCGetYear
	; Print year
	call .printTwoDigits
	mov '.', A
	call .xLcdPrintChar
	; Read month
	call .xRTCGetMonth
	; Print month
	call .printTwoDigits
	mov '.', A
	call .xLcdPrintChar
	; Read day
	call .xRTCGetDay
	; Print day
	call .printTwoDigits
	mov ' ', A
	call .xLcdPrintChar
	; Read hour
	call .xRTCGetHour
	; Print hour
	call .printTwoDigits
	mov ':', A
	call .xLcdPrintChar
	; Read minute
	call .xRTCGetMinute
	; Print minute
	call .printTwoDigits
	mov ':', A
	call .xLcdPrintChar
	; Read second
	call .xRTCGetSecond
	; Print second
	call .printTwoDigits
	pop Y
	pop X
	pop A
	ret

; Arguments:
; 	A - number
; No return
.printTwoDigits:
	; 10 <= A
	cmp 10, A
	jle .printTwoDigits_noZero
	push A
	; Print zero if lower than 10
	mov '0', A
	call .xLcdPrintChar
	pop A
.printTwoDigits_noZero:
	; Print number
	call .xStringPrintU8
	ret

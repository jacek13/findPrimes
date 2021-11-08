;-------------------------------------------------------------------------
;.MODEL FLAT, STDCALL

OPTION CASEMAP:NONE
;INCLUDE C:\masm32\include\windows.inc
.CODE

DllEntry PROC hInstDLL:DWORD, reason:DWORD, reserved1:DWORD

mov	eax, 1 	;TRUE
ret

DllEntry ENDP

isPrimeASM PROC
	MOV rbx, rcx	; badany parametr dajemy do Rejestru rbx 64 bitowego
CheckIfIsTwoOrThree:
	CMP rbx, 2		; sprawdzamy czy mamy liczbê pierwsz¹ równ¹ 2
	JE	IsPrimeNumber
	CMP rbx, 3
	JE	IsPrimeNumber
CheckIfNumberIsLessThanOne:
	CMP rbx, 1		; sprawdzamy czy parametr jest mniejszy lub równy 1
	JLE	IsNotPrime
	XOR rdx, rdx	; czyœcimi czêœæ u³amkow¹, potrzebn¹ w dzieleniu
	MOV	rax, rbx	; parametr wprowadzamu do akumulatora
	MOV rcx, 2		; do rejestru rcx wprowadzamy wartoœæ 2 w celu sprawdzenia poszielnoœci
	DIV rcx			; dokonujemy dzielenia
	CMP rdx, 0		; sprawdzamy czy czêœæ u³amkowa jest równa 0
	JE	IsNotPrime
	XOR rdx, rdx	; analogicznie jak powy¿ej, tylko dla wartoœci 3
	MOV rax, rbx
	MOV rcx, 3
	DIV rcx
	CMP rdx, 0
	JE	IsNotPrime
InitIterationValue:
	MOV r8, 5		; zaczynamy badaæ liczbê od wartoœci 5
	JMP CheckNumberInLoop
AddSixToIterationVariable:	; zwiêkszamy wartoœæ zmiennej steruj¹cej o 6
	MOV rax, r8
	ADD rax, 6
	MOV r8, rax
CheckNumberInLoop:	; rozpoczynamy instrukcje warunkowe w pêtli
	MOV rax, r8
	IMUL rax, rax
	CMP rax, rbx
	JG	IsPrimeNumber
	XOR rdx, rdx
	MOV rax, rbx
	DIV r8
	CMP rdx, 0
	JE	IsNotPrime
	MOV r9, r8
	ADD r9, 2
	XOR rdx, rdx
	MOV rax, rbx
	DIV r9
	CMP rdx, 0
	JE	IsNotPrime
	JMP AddSixToIterationVariable
IsPrimeNumber:
	MOV rax, 1
	RET
IsNotPrime:
	XOR rax, rax
	RET
isPrimeASM ENDP

;END DllEntry
END
;-------------------------------------------------------------------------
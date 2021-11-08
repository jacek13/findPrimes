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
	CMP rbx, 2		; sprawdzamy czy mamy liczb� pierwsz� r�wn� 2
	JE	IsPrimeNumber
	CMP rbx, 3
	JE	IsPrimeNumber
CheckIfNumberIsLessThanOne:
	CMP rbx, 1		; sprawdzamy czy parametr jest mniejszy lub r�wny 1
	JLE	IsNotPrime
	XOR rdx, rdx	; czy�cimi cz�� u�amkow�, potrzebn� w dzieleniu
	MOV	rax, rbx	; parametr wprowadzamu do akumulatora
	MOV rcx, 2		; do rejestru rcx wprowadzamy warto�� 2 w celu sprawdzenia poszielno�ci
	DIV rcx			; dokonujemy dzielenia
	CMP rdx, 0		; sprawdzamy czy cz�� u�amkowa jest r�wna 0
	JE	IsNotPrime
	XOR rdx, rdx	; analogicznie jak powy�ej, tylko dla warto�ci 3
	MOV rax, rbx
	MOV rcx, 3
	DIV rcx
	CMP rdx, 0
	JE	IsNotPrime
InitIterationValue:
	MOV r8, 5		; zaczynamy bada� liczb� od warto�ci 5
	JMP CheckNumberInLoop
AddSixToIterationVariable:	; zwi�kszamy warto�� zmiennej steruj�cej o 6
	MOV rax, r8
	ADD rax, 6
	MOV r8, rax
CheckNumberInLoop:	; rozpoczynamy instrukcje warunkowe w p�tli
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
;-------------------------------------------------------------------------
;.MODEL FLAT, STDCALL

OPTION CASEMAP:NONE
;INCLUDE C:\masm32\include\windows.inc
.CODE

DllEntry PROC hInstDLL:DWORD, reason:DWORD, reserved1:DWORD

mov	eax, 1 	;TRUE
ret

DllEntry ENDP

;--------------------------------
;	Prime test x86
;--------------------------------
isPrimeASM86 PROC
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
isPrimeASM86 ENDP
;--------------------------------
;	Prime test x64
;--------------------------------
primeTestASM64 PROC
loadData:					; wczytujemy przekazane liczby do rejestru xmm, 128 bitowego
	MOVD xmm0, rcx			; kopiujemy wartoœæ liczby _a do rejestru xmm0
	MOVD xmm1, rdx			; kopiujemy wartoœæ liczby _b do rejestru xmm1
	MOVLHPS xmm0, xmm0		; kopiujemy m³odsze 64 bity w rejestrze do starszych 64
	PXOR xmm2, xmm2			; szyœcimy rejestr xmm2
	MOVHLPS xmm0, xmm2		; czyœcimy m³odsze 64 bity rejestru xmm0 TODO przenieœæ odrazu wartoœæ z rejestru xmm1
	POR xmm0, xmm1			; dodajemy wartoœæ rejestru xmm1 do xmm0
initRegisters:
	XOR r10, r10			; przygotowujemy rejstr pod ustawianie flag o byciu liczba pierwsza
	XOR r11, r11
	XOR r12, r12	
	NOT	r12	
	XOR r13, r13
	XOR r14, r14					
	INC r14
	MOV r15, r14
	INC r15
firstComparisonWithTwo:		; porownujemy liczby do 2 i do 3
	MOV rbx, 2				; wpisujemy wartosc 2 do rejestru rbx TODO zrobiæ sta³¹ w data segment
	MOVD xmm3, rbx			; wpisujemy wartosc 2 do rejestru xmm3
	PUNPCKLQDQ xmm3, xmm3	; ustawiamy obie 64 bitowe czesci rejestru na wartosc 2
	CMPEQPS xmm3, xmm0		; porownujemy upakowane dane w rejestrze xmm0 z wartoscia 2
	MOVD r8, xmm3			; mlodsze 64 bity rejestru xmm3 przenosimy do rejestru r8
	MOVHLPS xmm3, xmm3		; kopiujemy starsze bity w miejsce mlodszych w rejestrze xmmfirstComparisonWithTwo3
	MOVD r9, xmm3			; kopiujemy starsze bity do rejestru r9
	CMP r9, r12				; porownujemy wynik z poprzedniego porownania ale pojedynczo
	JE firstNumberEqualsTwo		; jesli pierwsza liczba byla rowna 2 to skaczemy
secondComparisonWithTwo:		; analogicznie tylko dla liczby _b				
	CMP r8, r12				
	JE secondNumberEqualsTwo		
firstComparisonWithThree:
	MOV rbx, 3				; postepujemy analogicznie jak poprzednio
	MOVD xmm3, rbx			
	PUNPCKLQDQ xmm3, xmm3	
	CMPEQPS xmm3, xmm0		
	MOVD r8, xmm3			
	MOVHLPS xmm3, xmm3		
	MOVD r9, xmm3			
	CMP r9, r12	
	JE firstNumberEqualsThree
secondComparisonWithThree:
	CMP r8, r12					
	JE secondNumberEqualsThree		
areTheyLessOrEqualOne:
	MOV rbx, 1
	MOVD xmm3, rbx			
	PUNPCKLQDQ xmm3, xmm3
	MOVAPS xmm4, xmm0		; wykorzystamy dodatkowy rejestr aby zachowac kolejnosc porownywania
	CMPLEPS xmm4, xmm3		; sprawdzamy czy _a i _b sa mniejsze rowne 1
	MOVD r8, xmm4			
	MOVHLPS xmm4, xmm4		
	MOVD r9, xmm4			
	CMP r9, r12				; sprawdzamy pierwsze 64 bity	
	JE firstNumberLessOrEqualsOne
areTheyLessOrEqualOneForFirstNumber:
	CMP r8, r12				; sprawdzamy pierwsze 64 bity	
	JE secondNumberLessOrEqualsOne
checkingDivisibilityByTwoForTheFirstNumber:
	MOV rbx, 1						; wpisujemy wartosc 1 aby sprawdzaæ najmniej znaczacy bit w liczbie
	MOVD xmm3, rbx					; wpisujemy wartosc 1 do rejestru xmm3
	PUNPCKLQDQ xmm3, xmm3			; ustawiamy obie 64 bitowe czesci rejestru na wartosc 1
	PAND xmm3, xmm0					; wykonujemy operacje AND na obu 64 bitowych liczbach
	MOVD r8, xmm3					; mlodsze 64 bity rejestru xmm3 przenosimy do rejestru r8
	MOVHLPS xmm3, xmm3				; kopiujemy starsze bity w miejsce mlodszych w rejestrze xmm3
	MOVD r9, xmm3					; kopiujemy starsze bity do rejestru r9
	XOR r11, r11					; przygotowujemy rejestr ktory bedzie przechowywal infomacje o tym ze liczba nie jest pierwsza
	CMP r9, r13						; porownujemy wynik z poprzedniego porownania ale pojedynczo
	JE firstNumberIsDivisibleByTwo	; jesli pierwsza liczba byla podzielna przez 2
checkingDivisibilityByTwoForTheSecondNumber:						
	CMP r8, r13			
	JE secondNumberIsDivisibleByTwo
checkingDivisibilityByThreeForTheFirstNumber:
; TODO no podzielnosc przez 3 to sie mozna bawic w te parzyte i nie parzyste miejsca i ich roznice badac ale po co to komu
; my posluzymy sie narazie analogiczna metoda jak przy normalnym podejsciu 
	MOV r9, rdx		; UWAGA tutaj tracimy kopie z rdx
	XOR rdx, rdx	; analogicznie jak powy¿ej, tylko dla wartoœci 3
	MOV r8, rcx		; UWAGA tutaj tracimy kopie rcx
	MOV rcx, 3
	MOV rax, r8
	DIV rcx
	CMP rdx, 0
	JE	firstNumberIsDivisibleByThree
checkingDivisibilityByThreeForTheSecondNumber:
	XOR rdx, rdx	; analogicznie jak powy¿ej, tylko dla wartoœci 3
	MOV rax, r9
	DIV rcx
	CMP rdx, 0
	JE	secondNumberIsDivisibleByThree
	MOV r12, r8		; rejestry r12 i r13 nie beda juz wykorzystywane na tym etapie
	MOV r13, r9		; zatem przechowaja argumenty funkcji
;---------------TODO DO PRZEROBIENIA----------------
InitIterationValue:
	MOV r8, 5					; zaczynamy badaæ liczbê od wartoœci 5
	JMP CheckNumberInLoop
AddSixToIterationVariable:		; zwiêkszamy wartoœæ zmiennej steruj¹cej o 6
	MOV rax, r8
	ADD rax, 6
	MOV r8, rax
CheckNumberInLoop:				; rozpoczynamy instrukcje warunkowe w pêtli
	MOV rax, r8					; zmienna iteracyjna wpisujemy do rax
	IMUL rax, rax				; rax do potegi 2
	CMP rax, r12				; sprawdzamy czy jest mniejszy od pierwszej liczby
	JG	CheckNumberInLoop2
	XOR rdx, rdx
	MOV rax, r12
	DIV r8
	CMP rdx, 0
	JE	endForFirstNumber
	XOR rdx, rdx
	MOV rax, r13
	DIV r8
	CMP rdx, 0
	JE	endForSecondNumber
	MOV r9, r8
	ADD r9, 2
	XOR rdx, rdx
	MOV rax, r12
	DIV r9
	CMP rdx, 0
	JE	endForFirstNumber
	MOV r9, r8
	ADD r9, 2
	XOR rdx, rdx
	MOV rax, r13
	DIV r9
	CMP rdx, 0
	JE	endForSecondNumber
	JMP AddSixToIterationVariable
doWeContinueForTheFirstNumber:
	MOV rax, 1
	and rax, r11
	CMP rax, 1
	JE endProcedure
	JMP CheckNumberInLoop1
ContinueForTheFirstNumber:
	MOV rax, r8
	ADD rax, 6
	MOV r8, rax
CheckNumberInLoop1:				; rozpoczynamy instrukcje warunkowe w pêtli
	MOV rax, r8					; zmienna iteracyjna wpisujemy do rax
	IMUL rax, rax				; rax do potegi 2
	CMP rax, r12				; sprawdzamy czy jest mniejszy od pierwszej liczby
	JG	firstNumberIsPrime
	XOR rdx, rdx
	MOV rax, r12
	DIV r8
	CMP rdx, 0
	JE	endProcedure
	MOV r9, r8
	ADD r9, 2
	XOR rdx, rdx
	MOV rax, r12
	DIV r9
	CMP rdx, 0
	JE	endProcedure
	JMP ContinueForTheFirstNumber
doWeContinueForTheSecondNumber:
	MOV rax, 2
	and rax, r11
	CMP rax, 2
	JE endProcedure
	JMP CheckNumberInLoop2
ContinueForTheSecondNumber:
	MOV rax, r8
	ADD rax, 6
	MOV r8, rax
CheckNumberInLoop2:				; rozpoczynamy instrukcje warunkowe w pêtli
	MOV rax, r8					; zmienna iteracyjna wpisujemy do rax
	IMUL rax, rax				; rax do potegi 2
	CMP rax, r13				; sprawdzamy czy jest mniejszy od pierwszej liczby
	JG	secondNumberIsPrime
	XOR rdx, rdx
	MOV rax, r13
	DIV r8
	CMP rdx, 0
	JE	endProcedure
	MOV r9, r8
	ADD r9, 2
	XOR rdx, rdx
	MOV rax, r13
	DIV r9
	CMP rdx, 0
	JE	endProcedure
	JMP ContinueForTheSecondNumber
endProcedure:
	MOV rax, r10
	RET
endProcedurePrim:
	XOR rax, rax
	RET
firstNumberEqualsTwo:
	OR r10, r14					; jesli jest rowna 2 to jest liczba pierwsza
	JMP secondComparisonWithTwo	; wracamy do badania 2 liczby
secondNumberEqualsTwo:
	OR r10, r15					; jesli jest rowna 2 to jest liczba pierwsza
	MOV rax, 3
	CMP r10, rax				; sprawdzamy czy poprzednia liczba byla rowna 2
	JE endProcedure				; jesli tak to obie liczby sa rowne 2 wiec konczymy
	JMP secondComparisonWithTwo	; wracamy do badania liczb
firstNumberEqualsThree:
	OR r10, r14						; jesli jest rowna 3 to jest liczba pierwsza
	JMP secondComparisonWithThree	; wracamy do badania 2 liczby
secondNumberEqualsThree:
	OR r10, r15						; jesli jest rowna 3 to jest liczba pierwsza
	MOV rax, 3
	CMP r10, rax					; sprawdzamy czy poprzednia liczba byla rowna 3 lub 2
	JE endProcedure					; jesli tak to obie liczby sa pierwsze wiec konczymy
	JMP secondComparisonWithTwo		; wracamy do badania liczb
firstNumberLessOrEqualsOne:
	OR r11, r14						; jesli pierwsza jest mniejsza rowna 1
	JMP areTheyLessOrEqualOneForFirstNumber	; wracamy do badania 2 liczby
secondNumberLessOrEqualsOne:
	OR r11, r15						; jesli druga jest mniejsza rowna 1
	MOV rax, 3
	CMP r11, rax					; sprawdzamy czy poprzednia liczba byla rowniez podzielna przez 2
	JE endProcedurePrim				; jesli tak to obie liczby nie sa pierwsze
	JMP checkingDivisibilityByTwoForTheFirstNumber	; wracamy do badania liczb
firstNumberIsDivisibleByTwo:
	OR r11, r14						; jesli jest podzielna przez 2
	JMP checkingDivisibilityByTwoForTheSecondNumber	; wracamy do badania 2 liczby
secondNumberIsDivisibleByTwo:
	OR r11, r15						; jesli jest podzielna przez 2
	MOV rax, 3
	CMP r11, rax					; sprawdzamy czy poprzednia liczba byla rowniez podzielna przez 2
	JE endProcedurePrim				; jesli tak to obie liczby nie sa pierwsze
	JMP checkingDivisibilityByThreeForTheFirstNumber	; wracamy do badania liczb
firstNumberIsDivisibleByThree:
	OR r11, r14						; jesli jest podzielna przez 3
	JMP checkingDivisibilityByThreeForTheSecondNumber	; wracamy do badania 2 liczby
secondNumberIsDivisibleByThree:
	OR r11, r15						; jesli jest podzielna przez 3
	MOV rax, 3
	CMP r11, rax					; sprawdzamy czy poprzednia liczba byla rowniez podzielna przez 2
	JE endProcedurePrim				; jesli tak to obie liczby nie sa pierwsze
	JMP InitIterationValue			; wracamy do badania liczb
endForFirstNumber:
	OR r11, r14		
	JMP doWeContinueForTheSecondNumber
endForSecondNumber:
	OR r11, r15
	JMP doWeContinueForTheFirstNumber
firstNumberIsPrime:
	OR r10, r14	
	JMP endProcedure
secondNumberIsPrime:
	OR r10, r15
	JMP endProcedure
primeTestASM64 ENDP

;END DllEntry
END
;-------------------------------------------------------------------------
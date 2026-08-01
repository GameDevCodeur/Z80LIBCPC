
;------------------------------------------------------------------
; Macro: mPLOT
; Description: Transfers byte from (HL) to (DE)
; Input: HL = source address, DE = destination address
; Output: A = value transferred, (DE) = (HL)
; Destroys: A
;------------------------------------------------------------------
MACRO mPLOT		
 	LD A, (HL) 		; 1o 7c	
 	LD (DE), A 		; 1o 7c
ENDM

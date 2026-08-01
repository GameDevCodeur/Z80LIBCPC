
;-----------------------------------------------------------------------------
; mKEY_REQUESTED - Teste une touche 
; Retour: Z=1 si pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_REQUESTED
	LD C, E	
	LD H, 0
	LD L, D	
	LD A, (HL)
    CP C
ENDM

MACRO mKEY_REQUESTED_BIT KBUFFER, BIT
	LD   A, ({KBUFFER})
    BIT  {BIT}, A
ENDM

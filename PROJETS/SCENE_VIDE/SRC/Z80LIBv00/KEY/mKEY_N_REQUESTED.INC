
;-----------------------------------------------------------------------------
; mKEY_N_REQUESTED - Teste la touche N
; Retour: Z=1 si ESPACE pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_N_REQUESTED
	LD   A, (KBD_BUFFER_N)
    CP   KBD_KEY_N
ENDM
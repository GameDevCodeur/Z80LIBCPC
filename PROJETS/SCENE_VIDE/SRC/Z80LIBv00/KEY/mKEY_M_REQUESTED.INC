
;-----------------------------------------------------------------------------
; mKEY_M_REQUESTED - Teste la touche M
; Retour: Z=1 si ESPACE pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_M_REQUESTED
	LD   A, (KBD_BUFFER_M)
    CP   KBD_KEY_M
ENDM
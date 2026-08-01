
;-----------------------------------------------------------------------------
; mKEY_Y_REQUESTED - Teste la touche Y
; Retour: Z=1 si ESPACE pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_Y_REQUESTED
	LD   A, (KBD_BUFFER_Y)
    CP   KBD_KEY_Y
ENDM
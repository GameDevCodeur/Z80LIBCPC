
;-----------------------------------------------------------------------------
; mKEY_UP_REQUESTED - Teste la touche UP
; Retour: Z=1 si UP pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_UP_REQUESTED
	LD   A, (KBD_BUFFER_UP)
    CP   KBD_KEY_UP
ENDM
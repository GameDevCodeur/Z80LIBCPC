
;-----------------------------------------------------------------------------
; mKEY_ENTER_REQUESTED - Teste la touche ENTER
; Retour: Z=1 si ENTER pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_ENTER_REQUESTED
	LD   A, (KBD_BUFFER_ENTER)
    CP   KBD_KEY_ENTER
ENDM
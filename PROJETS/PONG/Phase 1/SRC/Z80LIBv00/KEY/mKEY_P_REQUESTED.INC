
;-----------------------------------------------------------------------------
; mKEY_P_REQUESTED - Teste la touche P
; Retour: Z=1 si ENTER pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_P_REQUESTED
	LD   A, (KBD_BUFFER_P)
    CP   KBD_KEY_P
ENDM
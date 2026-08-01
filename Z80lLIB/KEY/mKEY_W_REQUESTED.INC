
;-----------------------------------------------------------------------------
; mKEY_W_REQUESTED - Teste la touche W
; Retour: Z=1 si ESPACE pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_W_REQUESTED
	LD   A, (KBD_BUFFER_W)
    CP   KBD_KEY_W
ENDM
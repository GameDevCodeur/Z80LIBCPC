
;-----------------------------------------------------------------------------
; mKEY_K_REQUESTED - Teste la touche K
; Retour: Z=1 si ESPACE pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_K_REQUESTED
	LD   A, (KBD_BUFFER_K)
    CP   KBD_KEY_K
ENDM
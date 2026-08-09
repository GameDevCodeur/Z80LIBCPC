
;-----------------------------------------------------------------------------
; mKEY_SPACE_REQUESTED - Teste la touche ESPACE
; Retour: Z=1 si ESPACE pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_SPACE_REQUESTED
	LD   A, (KBD_BUFFER_SPACE)    ; 3 13
    CP   KBD_KEY_SPACE            ; 2 7
ENDM
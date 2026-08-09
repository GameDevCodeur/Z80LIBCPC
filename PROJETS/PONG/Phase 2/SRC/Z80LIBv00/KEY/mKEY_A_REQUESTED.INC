
;-----------------------------------------------------------------------------
; mKEY_A_REQUESTED - Teste la touche A
; Retour: Z=1 si A pressée, Z=0 sinon
; Modifie: AF
;-----------------------------------------------------------------------------

MACRO mKEY_A_REQUESTED_BIT
    LD  A, (KBD_BUFFER_A)
    BIT 5, A
ENDM

MACRO mKEY_A_REQUESTED
    LD  A, (KBD_BUFFER_A)
    CP  KBD_KEY_A
ENDM

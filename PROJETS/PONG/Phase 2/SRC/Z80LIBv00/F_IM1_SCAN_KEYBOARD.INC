; =============================================================================
; F_IM1_SCAN_KEYBOARD - Lecture du clavier via PPI
; =============================================================================
; Description : Lit l'état du clavier via la macro mPPI_READ_KEYBOARD
;               puis saute vers la sortie d'interruption.
; Entrée      : Aucune (VOID)
; Sortie      : Résultat dans registre selon macro
; Modifie     : Selon mPPI_READ_KEYBOARD
; =============================================================================

F_IM1_SCAN_KEYBOARD:
    mPPI_READ_KEYBOARD (VOID)       ; Lecture clavier via PPI
    JP   F_IM1_SKIP                 ; Saut vers restauration contexte + RETI

; -----------------------------------------------------------------------------
F_SCAN_KEYBOARD_END:                ; Label de fin de routine

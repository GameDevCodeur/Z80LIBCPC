; =============================================================================
; GESTIONNAIRE D'INTERRUPTION - RESTAURATION DU CONTEXTE ET SORTIE
; =============================================================================
; Description : Restaure les registres sauvegardés et effectue un retour
;               d'interruption propre.
; Registres   : AF, BC, DE, HL restaurés depuis les registres alternatifs
; Interruptions: Réactivées avant le retour (EI + RETI)
; =============================================================================

; --- Restauration du contexte et sortie ---
F_IM1_SKIP

    POP  IY
    POP  IX
    EX   AF, AF'                    ; Restaure AF
    EXX                             ; Restaure BC, DE, HL

    EI                              ; Réactive les interruptions
    RETI                            ; Retour d'interruption

; SIZE : 9 octets

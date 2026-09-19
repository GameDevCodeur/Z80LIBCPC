; =============================================================================
; FONCTION    : F_ETEINDRE_ENCRES
; =============================================================================
; Description : Force la couleur noire sur les 16 encres de la palette ainsi
;               que sur la bordure de l'écran (17 encres au total).
;               Utilisé pour masquer l'écran pendant les chargements ou la VBL.
; Entrée      : Aucune
; Sortie      : Aucune
; Modifie     : AF, B, D (Les registres d'origine sont écrasés)
; =============================================================================

F_ETEINDRE_ENCRES

    LD   A, 17                  ; 17 itérations (16 pens + border)
    LD   B, GA_PORT_CMD_B       ; Port GA chargé une seule fois
    LD   D, GA_COLOR_BLACK      ; Couleur noire pré-encodée
.B_ENCRES
    DEC  A
    mGA_SET_PEN_INK (VOID)      ; Macro qui sélectionne pen A et met couleur D
    JR   NZ, .B_ENCRES          ; Boucle jusqu'à A=0
    
RET

; SIZE : 14 octets

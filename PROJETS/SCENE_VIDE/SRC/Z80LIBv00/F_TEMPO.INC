; ------------------------------------------------------------
; F_TEMPO - Temporisation paramétrable
; Entrée : A = facteur de temporisation (1 à 255)
; Durée à 4 MHz : A × 0,214 seconde (approximativement)
;
; Exemples :
;   ld a, 5   : call tempo  → ~1,07 secondes
;   ld a, 10  : call tempo  → ~2,14 secondes
;   ld a, 47  : call tempo  → ~10   secondes
; ------------------------------------------------------------

F_TEMPO:
    ld e, a        ; E = compteur
    ld bc, 0       ; B=0 (256 DJNZ), C=0 (256 boucles)
    
.loop:
    djnz $         ; 256 × DJNZ
    dec c          ; Décrémente C
    jr nz, .loop   ; Si C≠0, continue
    
    dec e          ; Décrémente E
    jr nz, .loop   ; Si E≠0, recommence depuis le début
    
    ret
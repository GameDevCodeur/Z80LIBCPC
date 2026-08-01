; -------------------------------------------------------------------------
; CRTC_TABLE_32x24
; TABLE DES VALEURS DES REGISTRES CRTC INVERSE
; -------------------------------------------------------------------------
CRTC_TABLE_32x24
    DB  #00                     ; R13 - Adresse de départ écran poids faible
    DB  #30                     ; R12 - Adresse de départ écran poids fort (#30 -> #C000)
    DB  0                       ; R11 - Ligne de fin du curseur
    DB  0                       ; R10 - Ligne de début du curseur (0 = caché)
    DB  7                       ; R9  - Hauteur d'un caractère (7 = 8 lignes de pixels)
    DB  0                       ; R8  - Mode de balayage (0 = Pas d'entrelacement)
    DB  30                      ; R7  - Position de la synchronisation verticale
    DB  192 / 8                 ; R6  - Hauteur visible (24 lignes de caractères)
    DB  #00                     ; R5  - Ajustement vertical fin
    DB  38                      ; R4  - Hauteur totale de la trame
    DB  #86                     ; R3  - Largeur des pulses de synchro (H=14, V=9)
    DB  42                      ; R2  - Position de la synchronisation horizontale
    DB  32                      ; R1  - Largeur visible (32 caractères = 256 pixels)
    DB  #3F                     ; R0  - Largeur totale de la ligne

; R14/R15 curseur    — non utilisés
; R16/R17 light pen  — lecture seule, non configurables
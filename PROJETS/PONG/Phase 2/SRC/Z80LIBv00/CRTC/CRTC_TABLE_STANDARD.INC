; ─────────────────────────────────────────────
; CRTC_TABLE
; Valeurs registres CRTC — écran standard CPC
; ─────────────────────────────────────────────
CRTC_TABLE:
    DB  63      ; R0  - total caractères/ligne (visible + invisible)
    DB  40      ; R1  - caractères affichés/ligne
    DB  46      ; R2  - position sync horizontale
    DB  142     ; R3  - largeur sync H=14 V=9 (nibbles)
    DB  38      ; R4  - total vertical (lignes caractères)
    DB   0      ; R5  - ajustement vertical fin
    DB  25      ; R6  - lignes caractères affichées
    DB  30      ; R7  - position sync verticale
    DB   0      ; R8  - entrelacement (0 = désactivé)
    DB   7      ; R9  - raster max (8 lignes/car. : 0-7)
    DB   0      ; R10 - curseur début + clignotement (0 = caché)
    DB   0      ; R11 - curseur fin
    DB  48      ; R12 - adresse haute page vidéo (0x30 = 0xC000)
    DB   0      ; R13 - offset bas page vidéo
CRTC_TABLE_END:

; R14/R15 curseur    — non utilisés
; R16/R17 light pen  — lecture seule, non configurables
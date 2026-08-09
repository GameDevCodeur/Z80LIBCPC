
; --------------------------------------------------------
; MACRO   : mXY_TO_VRAM_128x192_M0
; Rôle    : Convertit des coordonnées (Y, X) en adresse VRAM CPC.
;           SPÉCIFIQUE à un écran 128x192 pixels en Mode 0
;           (stride #40 codé en dur). Sur un écran d'une autre
;           résolution, cette macro donnera une adresse fausse —
;           il faudra alors adapter la constante #40.
;           Formule : #C000 + (Y&7)*#800 + (Y>>3)*#40 + X/2
; Entrée  : H = PosY (0-191)
;           A = PosX en PIXELS (0-127, Mode 0)
; Sortie  : HL = adresse VRAM physique
; Détruit : AF, BC
; Préserve: DE
; --------------------------------------------------------
MACRO mXY_TO_VRAM_128x192_M0
    SRL  A                       ; pixel → octet (mode 0 : 2px/octet)
    LD   L, A

    LD   B, H                    ; B = Y temporaire

    ; --- Calcul partie haute : #C0 + (Y&7)*#800 ---
    LD   A, H
    AND  7
    ADD  A, A
    ADD  A, A
    ADD  A, A
    ADD  A, $C0
    LD   H, A

    ; --- Calcul partie basse : (Y>>3)*#40 (stride 128px) ---
    LD   A, B
    AND  -8
    LD   B, 0
    RLA  :  RL B
    RLA  :  RL B
    RLA  :  RL B
    LD   C, A
    ADD  HL, BC                  ; HL = adresse base ligne + offset X
    
ENDM

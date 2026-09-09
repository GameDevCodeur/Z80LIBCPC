
;==============================================================
; F_SCENE_CHARGER — Fonction de chargement
;==============================================================
; Contraintes :
; - SCENE_TABLE doit être alignée sur 256 octets
; - SCENE_TABLE doit tenir sur une seule page (< 256 octets)
; - Les INC L préservent le page-byte (H)
;==============================================================

F_SCENE_CHARGER

    LD   A, IM1_NOT_READY
    LD   (IM1_INDEX), A

    LD   A, B
    CP   SCENE_TABLE_COUNT
    RET  NC                 ; Si B >= SCENE_TABLE_COUNT

    ; Calcul ID*6
    ADD  A, A               ; A = ID*2
    LD   L, A               ; L = ID*2
    ADD  A, A               ; A = ID*4
    ADD  A, L               ; A = ID*6

    ; Construction adresse
    LD   L, A
    LD   H, HI(SCENE_TABLE) ; HL = SCENE_TABLE + offset
    
    LD   E, (HL) : INC L
    LD   D, (HL) : INC L    ; DE = F_SCENE_xxx
    LD   (SCENE_ACTUEL), DE

    LD   E, (HL) : INC L
    LD   D, (HL) : INC L    ; DE = ARRAY_IM1_xxx
    LD   (IM1_CURRENT), DE

    LD   E, (HL) : INC L
    LD   D, (HL)            ; DE = F_SCENE_xxx_INIT

    EX   DE, HL
    JP   (HL)
    
    ; AFBDEHL!
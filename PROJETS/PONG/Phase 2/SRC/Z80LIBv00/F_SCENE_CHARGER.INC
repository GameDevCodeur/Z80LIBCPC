;==============================================================
; F_SCENE_CHARGER — Version page-alignée
;==============================================================
; SCENE_TABLE doit être alignée sur 256 octets.
; Les INC L préservent le page-byte (H), donc interdits si la
; table déborde sur la page suivante (vérifié par l'assembleur
; ou assert SIZE < 256).
;==============================================================

F_SCENE_CHARGER

    LD   A, IM1_NOT_READY
    LD   (IM1_INDEX), A

    LD   A, B
    CP   SCENE_TABLE_COUNT
    RET  NC

    ADD  A, A
    LD   E, A
    ADD  A, A
    ADD  A, E               ; A = ID*6

    LD   L, A
    LD   H, HI(SCENE_TABLE) ; HL = SCENE_TABLE + offset, sans ADD HL,DE

    LD   C, (HL) : INC L
    LD   B, (HL) : INC L    ; BC = F_SCENE_xxx
    LD   E, (HL) : INC L
    LD   D, (HL) : INC L    ; DE = ARRAY_IM1_xxx
    LD   (IM1_CURRENT), DE
    LD   E, (HL) : INC L
    LD   D, (HL)            ; DE = F_SCENE_xxx_INIT

    LD   (SCENE_ACTUEL+1), BC
    EX   DE, HL
    JP   (HL)

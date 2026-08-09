
;-----------------------------------------------------------
; mKEY_ANY_REQUESTED_FAST
;
; Version ultra-rapide : déroulage complet des 10 lignes.
; Détecte si une touche est pressée.
;
; Entrée : KBD_BUFFER_RAM (10 octets, convention Active Low)
; Sortie : Z = aucune touche, NZ = au moins une touche
; Détruit: AF, HL
;-----------------------------------------------------------
MACRO mKEY_ANY_REQUESTED_FAST

    LD   HL, KBD_BUFFER_RAM
    LD   A, #FF

    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL
    AND  (HL) : INC HL

    CP   #FF

ENDM

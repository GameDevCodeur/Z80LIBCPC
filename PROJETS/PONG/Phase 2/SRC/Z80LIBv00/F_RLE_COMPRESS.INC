; ============================================================
;  Compresseur RLE pour Z80
;  Entrée : HL = source, BC = longueur, IX = destination
;  Sortie : IX = après dernier octet écrit, BC = 0
;  Détruit : AF, BC, DE, HL, IX
; ============================================================

F_RLE_COMPRESS:
NEXT_BYTE:
        LD   A, B           ; BC = 0 ?
        OR   C
        RET  Z

        LD   A, (HL)        ; lire l'octet courant
        INC  HL
        DEC  BC

        LD   D, A           ; D = valeur de référence
        LD   E, 1           ; E = compteur (déjà 1 occurrence)

COUNT_LOOP:
        LD   A, B           ; plus rien à lire ?
        OR   C
        JR   Z, WRITE_PAIR

        LD   A, E           ; compteur saturé à 255 ?
        CP   255
        JR   Z, WRITE_PAIR

        LD   A, (HL)        ; prochain octet = même valeur ?
        CP   D
        JR   NZ, WRITE_PAIR

        INC  HL
        DEC  BC
        INC  E
        JR   COUNT_LOOP

WRITE_PAIR:
        LD   (IX+0), E      ; écrire le compteur
        LD   (IX+1), D      ; écrire la valeur
        INC  IX
        INC  IX
        JR   NEXT_BYTE
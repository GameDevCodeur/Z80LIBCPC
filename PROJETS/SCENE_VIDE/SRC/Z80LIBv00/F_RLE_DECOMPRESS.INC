; ============================================================
;  Décompresseur RLE avec sentinelle COUNT=0 pour fin
;  Entrée  : HL = source compressée, DE = destination
;  Sortie  : DE = après dernier octet écrit
;  Détruit : AF, B, HL
; ============================================================

F_RLE_DECOMPRESS:
        LD   A, (HL)        
        OR   A
        RET  Z

        INC  HL

        LD   B, A           ; B = COUNT
        LD   A, (HL)        ; A = BYTE
        INC  HL

DECOMP_LOOP:
        LD   (DE), A        ; 7 cycles
        INC  DE             ; 6 cycles
        DJNZ DECOMP_LOOP

        JR   F_RLE_DECOMPRESS
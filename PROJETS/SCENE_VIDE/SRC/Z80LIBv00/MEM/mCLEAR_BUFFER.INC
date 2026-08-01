;-----------------------------------------------------------
; mCLEAR_BUFFER start_address
;
; Vide (met à 0) un buffer écran de 16384 octets (0x4000),
; en exploitant le débordement naturel des registres 8 bits
; L et H plutôt qu'un compteur BC explicite. Environ 2x plus
; rapide que la version par LDIR (11 cycles/octet contre 21).
;
; Paramètre : start_address -> adresse de début du buffer
;                              (typiquement #C000)
; Entrée  : -
; Sortie  : les 16384 octets à partir de start_address = 0
; Détruit : AF, HL
;-----------------------------------------------------------
MACRO mCLEAR_BUFFER start_address
    LD   HL, {start_address}
    XOR  A
@razScr
    REPEAT 8
        LD (HL), A
        INC L
    REND
    JR   NZ, @razScr
    INC  H
    JR   NZ, @razScr
ENDM

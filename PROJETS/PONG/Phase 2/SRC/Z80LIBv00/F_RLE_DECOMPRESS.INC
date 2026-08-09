; --------------------------------------------------------
; ROUTINE : F_RLE_DECOMPRESS
; Rôle    : Décompresse des données RLE vers DE.
;
; Format des données :
;   COUNT, BYTE
;   COUNT, BYTE
;   ...
;   00                  ; fin des données
;
; Entrée :
;   HL = adresse des données RLE
;   DE = adresse destination
;
; Sortie :
;   HL = après le dernier COUNT = 00
;   DE = après les données décompressées
;
; Détruit :
;   AF, B
;
; Exemple :
;   DEFB 5,#29
;   DEFB 3,#3C
;   DEFB 0
;
; donne :
;   #29,#29,#29,#29,#29,#3C,#3C,#3C
; --------------------------------------------------------

F_RLE_DECOMPRESS

    LD   A,(HL)             ; A = COUNT
    OR   A                  ; COUNT = 0 ?
    RET  Z                  ; Oui -> fin

    INC  HL

    LD   B,A                ; B = COUNT
    LD   A,(HL)             ; A = BYTE à répéter
    INC  HL

DECOMP_LOOP:

    LD   (DE),A             ; écrit BYTE
    INC  DE                 ; destination suivante
    DJNZ DECOMP_LOOP        ; B--, répète jusqu'à B=0

    JR   F_RLE_DECOMPRESS   ; paire suivante

; 14 octets
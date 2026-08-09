; --------------------------------------------------------
; ROUTINE : F_RLE_BIT7_DECOMPRESS
; TAILLE  : 19 octets
; --------------------------------------------------------
; Rôle :
;   Décompresse un flux RLE utilisant le bit 7 comme
;   indicateur de répétition.
;
; FORMAT :
;
;   #00       = marqueur de fin
;
;   #01-#7F   = octet brut
;
;   #81-#FF   = séquence RLE
;               COUNT = bits 6..0
;               VALUE = octet suivant
;
;   #80       = réservé
;
; --------------------------------------------------------
; ENTREES :
;   HL = adresse des données compressées
;   DE = adresse destination
;
; SORTIES :
;   HL = adresse après le marqueur #00
;   DE = adresse après les données décompressées
;
; DETRUIT :
;   AF, B
;
; PRESERVE :
;   C, D, E
;
; OPTIMISATION :
;   Le point d'entrée est directement placé sur
;   .NEXT_RLE_BYTE.
;
;   DJNZ tombe directement sur .NEXT_RLE_BYTE après
;   la dernière répétition.
;
; --------------------------------------------------------

F_RLE_BIT7_DECOMPRESS EQU .NEXT_RLE_BYTE   ; entrée directe

.RLE_SEQUENCE
    AND  #7F                 ; extrait le compteur (0‑127)
    LD   B, A
    LD   A, (HL)             ; valeur à répéter
    INC  HL
    
.RLE_REPEAT_BYTE
    LD   (DE), A
    INC  DE
    DJNZ .RLE_REPEAT_BYTE    ; tombe dans .NEXT_RLE_BYTE quand B=0

.NEXT_RLE_BYTE
    LD   A, (HL)
    INC  HL
    OR   A                   ; Z=1 si fin, S=1 si bit7
    RET  Z
    JR   M, .RLE_SEQUENCE    ; bit7=1 → RLE, sinon octet brut

.COPY_RAW_BYTE
    LD   (DE), A
    INC  DE
    JR   .NEXT_RLE_BYTE


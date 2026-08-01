
; --------------------------------------------------------
; MACRO   : mGET_PIXEL_MASK_M0
; Rôle    : Isole les bits du pixel gauche ou droit d'un
;           octet Mode 0 selon la parité de C (coordonnée X).
; Entrée  : A = octet source, C = coordonnée X (bit0 = parité)
; Sortie  : A = octet masqué (pixel gauche ou droit isolé)
; Détruit : AF
; --------------------------------------------------------
MACRO mGET_PIXEL_MASK_M0
    BIT  0, C
    JR   NZ, @right
    AND  %10101010       ; Isole pixel de gauche
    JR   @done
@right
    AND  %01010101       ; Isole pixel de droite
@done
ENDM
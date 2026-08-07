
;===============================================================
; mDRAWHLINE
;---------------------------------------------------------------
; Description : Trace une ligne horizontale de B octets, tous
;               remplis avec la valeur de A, à partir de DE.
;
; Entrée      : DE = adresse destination (VRAM)
;               A  = valeur/couleur à écrire
;               B  = nombre d'octets (longueur de la ligne)
;
; Sortie      : Ligne tracée en VRAM
;
; Registres modifiés : DE, B, F
;
; Exemple d'utilisation :
;               LD   DE, #C000
;               LD   A, #FF
;               LD   B, 10
;               mDRAWHLINE
;===============================================================
MACRO mDRAWHLINE 
@BCL_HLINE
    LD   (DE), A
    INC  DE
    DJNZ @BCL_HLINE
ENDM

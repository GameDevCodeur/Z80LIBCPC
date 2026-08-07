
;================================================================
;  INSERTION DES DONNEES SCENE TITRE
;================================================================

TITRE_TEMPO_DATA
    DEFB TITRE_TEMPO_3             ; durée 3s = 150 / 50

TITRE_PAL_DATA 
    INCLUDE "TITRE_DATA.PAL"

TITRE_ZX0_DATA
    INCZX0 "TITRE_ZX0_DATA.BIN"

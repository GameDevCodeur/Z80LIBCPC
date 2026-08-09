
;============================================================
; mPSG_SELECT
; Sélectionne un registre du PSG via le port PPI
;
; Entrée  : reg = numéro de registre PSG (0-15)
; Sortie  : -
; Modifie : BC
;============================================================
MACRO mPSG_SELECT reg
    LD   BC, PPI_PORT_A + {reg} ; B = port A, C = registre PSG
    OUT  (C), C                 ; sélectionne le registre
MEND

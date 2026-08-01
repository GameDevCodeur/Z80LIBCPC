
;============================================================
; mPSG_VALIDATE
; Valide la sélection du registre PSG (LATCH → INACTIVE)
;
; Entrée  : -
; Sortie  : -
; Modifie : BC
;============================================================
MACRO mPSG_VALIDATE
    LD   BC, PPI_PORT_C + PSG_SELECT_REG ; B = port C, C = PSG_SELECT_REG
    OUT  (C), C                          ; BDIR/BC2 → LATCH (valide registre)
    OUT  (C), 0                          ; BDIR/BC2 → INACTIVE
MEND

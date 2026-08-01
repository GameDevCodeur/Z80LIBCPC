
;============================================================
; mPPI_RESET
; Remet le PPI 8255 en mode par défaut
;
; Entrée  : -
; Sortie  : -
; Modifie : BC
;============================================================
MACRO mPPI_RESET
    LD   BC, PPI_CTRL + PPI_RESET       ; B = 0xF7, C = mode reset
    OUT  (C), C                         ; envoie mot de contrôle au PPI
MEND


;============================================================
; mPPI_KEYBOARD_MODE_INIT
; Configure le PPI 8255 en mode lecture clavier
;
; Entrée  : -
; Sortie  : -
; Modifie : BC
;============================================================
MACRO mPPI_KEYBOARD_MODE_INIT
    LD   BC, PPI_CTRL + PPI_READ_KBD    ; B = 0xF7, C = mode lecture
    OUT  (C), C                         ; envoie mot de contrôle au PPI
MEND

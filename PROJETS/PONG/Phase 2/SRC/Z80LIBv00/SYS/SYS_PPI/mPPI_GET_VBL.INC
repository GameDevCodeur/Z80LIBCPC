
;============================================================
; mPPI_GET_VBL
;
; Attend la fin du signal VSYNC (VBL) via le port B du PPI.
; Boucle bloquante tant que le bit 0 (VSYNC) du port B est
; à 0 ; sort dès que ce bit passe à 1.
;
; Entrée  : PPI_PORT_B -> adresse du port B du PPI (#F5xx)
; Sortie  : -
; Modifie : AF
;============================================================
MACRO mPPI_GET_VBL
    LD   A, HI(PPI_PORT_B)
@wait_vbl:
    IN   A, (0)
    RRCA
    JR   NC, @wait_vbl
MEND

; SIZE : 7o

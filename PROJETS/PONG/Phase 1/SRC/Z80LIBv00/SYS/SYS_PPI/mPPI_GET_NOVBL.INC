
;============================================================
; mPPI_GET_NOVBL
; Attend du debut de signal VSYNC (VBL) via le port B du PPI
;
; Entrée  : -
; Sortie  : -
; Modifie : AF, B
;============================================================
MACRO mPPI_GET_NOVBL
    LD   B, HI(PPI_PORT_B)  ; 2o  7c  — B = 0xF5
@wait_novbl:
    IN   A, (C)             ; 2o 11c  — lit port B PPI
    RRCA                    ; 1o  4c  — bit 0 (VSYNC) → carry
    JR   C, @wait_novbl     ; 2o 12c  — boucle si VSYNC = 0
MEND 

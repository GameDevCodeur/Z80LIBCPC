
;============================================================
; mPPI_START_VBL - Se synchronise sur le début de trame
; Attend: fin VSYNC → début VSYNC
;
; Entrée  : -
; Sortie  : -
; Modifie : AF, B
;============================================================
MACRO mPPI_START_VBL
    mPPI_GET_NOVBL (VOID)
    mPPI_GET_VBL (VOID)
MEND 

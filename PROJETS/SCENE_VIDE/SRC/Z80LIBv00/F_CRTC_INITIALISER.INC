
INCLUDE "CRTC/CRTC_TABLE.INC"

; -------------------------------------------------------------------------
; ROUTINE : F_CRTC_32x24
; -------------------------------------------------------------------------
; Entrée  : Aucune
; Sortie  : Aucune
; Modifie : AF, BC, HL (selon l'implémentation de la macro)
; -------------------------------------------------------------------------
F_CRTC_INITIALISER

    LD   HL, CRTC_TABLE
    LD    C, 13
    mCRTC_SCR_SETUP (VOID) 

    RET

; SIZE : 3+2+13+1 = 19o


;--------------------------------------------------------
; mSET_PREVIOUSLINE_DE
; Calcule l'adresse de la ligne pixel précédente (écran CPC)
; Entrée  : DE = adresse ligne pixel courante
; Sortie  : DE = adresse ligne pixel précédente
; Modifie : AF
;--------------------------------------------------------
MACRO mSET_PREVIOUSLINE_DE

    LD  A, D : SUB 8 : LD  D, A
    AND #38
    CP  #38
    JR  NZ, @END
    ; --- franchissement : bloc de caractères précédent ---
    LD  A, E : SUB SCR_WIDTH_BYTE : LD E, A
    LD  A, D : SBC A, #C0 : LD D, A
    SET 3, D

@END
MEND

_HAS_mSET_PREVIOUSLINE_DE EQU 1   ; marqueur : cette macro est bien définie

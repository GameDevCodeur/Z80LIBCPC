
;--------------------------------------------------------
; mSET_NEXTLINE_HL
; Calcule l'adresse de la ligne pixel suivante (écran CPC)
;
; Entrée  : HL = adresse ligne pixel courante
; Sortie  : HL = adresse ligne pixel suivante
; Modifie : AF
;--------------------------------------------------------
MACRO mSET_NEXTLINE_HL

    LD  A, H : ADD A, 8 : LD  H, A
    AND #38
    JR  NZ, @END
    ; --- ici, on a débordé : changement de bande ---
    LD  A, SCR_WIDTH_BYTE : ADD L : LD L, A
    LD  A, #C0 : ADC H : LD H, A
    RES 3, H

@END
MEND

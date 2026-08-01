
;--------------------------------------------------------
; mSET_NEXTLINE_DE
; Calcule l'adresse de la ligne pixel suivante (écran CPC)
; Entrée  : HL = adresse ligne pixel courante
; Sortie  : HL = adresse ligne pixel suivante
; Modifie : AF
;--------------------------------------------------------
MACRO mSET_NEXTLINE_DE

    LD  A, D : ADD A, 8 : LD  D, A    
    AND #38
    JR  NZ, @END

    ; --- franchissement : bloc de caractères suivant ---
    LD  A, E : ADD A, SCR_WIDTH_BYTE : LD  E, A    
    LD  A, D : ADC A, #C0 : LD  D, A
    RES 3, D

@END
MEND

_HAS_mSET_NEXTLINE_DE EQU 1   ; marqueur : cette macro est bien définie

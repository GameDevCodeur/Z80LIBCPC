
;--------------------------------------------------------
; mSET_PREVIOUSLINE_HL
; Calcule l'adresse de la ligne pixel précédente (écran CPC)
;
; Entrée  : HL = adresse ligne pixel courante
; Sortie  : HL = adresse ligne pixel précédente
; Modifie : AF
;--------------------------------------------------------
MACRO mSET_PREVIOUSLINE_HL
    LD  A, H
    SUB 8               ; ligne précédente dans le même bloc
    LD  H, A

    AND #38             ; teste si on passe sous le bloc
    CP  #38             ; (retenue = les bits 3-5 sont repassés à 1)
    JR  NZ, @END
    
    LD  A, L            ; changement de bloc :
    SUB SCR_WIDTH_BYTE  ; -#4000 pour repasser au bloc précédent
    LD  L, A

    LD  A, H
    SBC A, #C0          ; ajuste l'octet haut - emprunt
    LD  H, A

    SET 3,D
    
@END
MEND
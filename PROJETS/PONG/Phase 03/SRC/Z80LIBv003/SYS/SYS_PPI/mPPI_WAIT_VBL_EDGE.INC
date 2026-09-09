

; ------------------------------------------------------------
; MACRO   : mPPI_WAIT_VBL_EDGE
; Rôle    : Attend le flanc montant du prochain VBL (VSYNC).
;           Si un VBL est déjà en cours au moment de l'appel,
;           attend d'abord sa fin avant de détecter le flanc
;           suivant, pour ne jamais renvoyer un flanc déjà écoulé.
;
; Entrée  : PPI_PORT_B = adresse du port PPI (octet haut utilisé
;           pour le décodage d'E/S Z80, format IN A,(n))
; Sortie  : (aucune valeur retournée, bloque jusqu'au flanc)
; Détruit : AF
; Dépend  : mPPI_READ_VBL_BIT
; Notes   : Boucle bloquante (polling) - bloque le CPU tant que
;           le flanc n'est pas détecté.
; ------------------------------------------------------------
    MACRO mPPI_WAIT_VBL_EDGE
@wait_low
        mPPI_READ_VBL_BIT
        JR   C, @wait_low          ; attend que VSYNC retombe (fin d'un VBL en cours)
@wait_high
        mPPI_READ_VBL_BIT
        JR   NC, @wait_high        ; attend le vrai flanc montant du prochain VBL
    MEND

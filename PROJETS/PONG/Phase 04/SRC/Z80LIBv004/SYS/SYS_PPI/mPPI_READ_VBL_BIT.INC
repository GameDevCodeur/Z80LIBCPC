

; ------------------------------------------------------------
; MACRO   : mPPI_READ_VBL_BIT
; Rôle    : Lit le port PPI et place le bit VSYNC dans Carry.
;           Bloc élémentaire partagé par mPPI_WAIT_VBL_EDGE.
;
; Sortie  : Carry = bit VSYNC lu (1 = actif, 0 = inactif)
; Détruit : AF
; ------------------------------------------------------------
MACRO mPPI_READ_VBL_BIT
    LD   A, HI(PPI_PORT_B)   ; recharge l'octet haut à chaque lecture
    IN   A, (0)
    RRCA                     ; bit VSYNC -> Carry
MEND


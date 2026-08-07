; --------------------------------------------------------
; MACRO   : mBLIT_LINE_M0
; Rôle    : Copie {NB} octets (1 ligne de motif Mode 0) vers
;           l'écran (DE). Deux modes selon {TRANSPARENT} :
;             0 = copie brute via LDI (rapide, pas de test)
;             1 = saute l'écriture des octets source = 0
;                 (transparence, le fond écran est préservé)
;           Avance ensuite DE à la ligne pixel suivante si
;           demandé, via mSET_NEXTLINE_DE.
; Paramètres :
;   {SRC}         = source à charger dans HL avant la copie
;   {NB}          = nombre d'octets à traiter
;   {AVANCE}      = 1 pour avancer à la ligne suivante, 0 sinon
;   {TRANSPARENT} = 1 pour sauter les octets à 0, 0 pour LDI brut
; Entrée  : DE = adresse écran ligne courante
; Sortie  : DE = adresse écran ligne suivante (si AVANCE=1)
; Détruit : AF, DE, HL
; Dépend  : mSET_NEXTLINE_DE
; --------------------------------------------------------
MACRO mBLIT_LINE_M0 SRC, NB, AVANCE, TRANSPARENT
    LD HL, {SRC}
    IF {TRANSPARENT}
        REPEAT {NB}
            LD   A, (HL)
            INC  HL
            OR   A
            JR   Z, $+3
            LD   (DE), A
            INC  DE
        REND
    ELSE
        REPEAT {NB}
            LDI
        REND
    ENDIF
    IF {AVANCE}
        REPEAT {NB}
            DEC DE
        REND
        mSET_NEXTLINE_DE (VOID)
    ENDIF
MEND

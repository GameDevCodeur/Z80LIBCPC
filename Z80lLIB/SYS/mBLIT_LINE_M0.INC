
; --------------------------------------------------------
; MACRO   : mBLIT_LINE_M0
; Rôle    : Copie {NB} octets (1 ligne de motif Mode 0) via LDI
;           vers l'écran (DE), puis avance DE au début de la
;           ligne pixel suivante si demandé, en gérant
;           correctement le franchissement d'un bloc de 8 lignes
;           de caractères (via mSET_NEXTLINE_DE, pas un simple
;           +#0800 qui serait faux en cas de franchissement
;           de bloc).
; Paramètres :
;   {SRC}    = source à charger dans HL avant les LDI
;              (adresse immédiate ou variable mémoire)
;   {NB}     = nombre d'octets à copier (largeur du sprite en
;              octets écran pour cette ligne)
;   {AVANCE} = 1 pour avancer à la ligne suivante, 0 sinon
;              (dernière ligne d'un sprite : pas d'avance)
; Entrée  : DE = adresse écran ligne courante
; Sortie  : DE = adresse écran ligne suivante (si AVANCE=1)
; Détruit : AF, BC, DE, HL
; Dépend  : mSET_NEXTLINE_DE
; --------------------------------------------------------
MACRO mBLIT_LINE_M0 SRC, NB, AVANCE
    LD HL, {SRC}
    REPEAT {NB}
        LDI
    REND
    IF {AVANCE}
        REPEAT {NB}
            DEC DE
        REND
        mSET_NEXTLINE_DE (VOID)
    ENDIF
MEND

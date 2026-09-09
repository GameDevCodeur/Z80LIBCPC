
; ------------------------------------------------------------
; MACRO   : mCRTC_INIT_REGISTERS
; Rôle    : Configure tous les registres du CRTC (6845) en
;           une boucle, à partir d'une table de valeurs en RAM.
;           Utilise le trick OUT(C),C + OUTI pour n'écrire
;           qu'un minimum d'instructions par registre.
;
; Entrée  : HL = adresse de la table de valeurs CRTC
;                (17 ou 18 octets, un par registre)
;           C  = index du DERNIER registre à configurer
;                (= nombre de registres - 1, car la boucle
;                descend de C jusqu'à 0 inclus)
;           IMPORTANT : la table doit être ordonnée du registre
;           C (le plus haut) vers le registre 0, dans cet ordre,
;           puisque HL avance en incrémentant à chaque OUTI
;           tandis que C décroît en parallèle.
;
; Sortie  : (aucune valeur retournée, config CRTC appliquée)
; Détruit : AF, BC, HL
;
; Notes   : Exploite le comportement documenté de OUTI :
;           B est décrémenté AVANT le OUT, donc charger B=0xBE
;           fait que le OUT réel s'effectue sur le port 0xBD
;           (CRTC_DAT). L'explicite DEC B qui suit ramène B à
;           0xBC (CRTC_REG) pour la sélection du registre suivant.
; ------------------------------------------------------------
MACRO mCRTC_INIT_REGISTERS
    LD   B, HI(CRTC_REG)         ; B = 0xBC -> CRTC_REG

@BC_CRTC
    ; 1. Sélection du registre du CRTC
    OUT  (C), C                  ; sélectionne registre C

    ; 2. Envoi de la valeur de configuration
    LD   B, #BE                  ; /!\ OUTI décrémente B en premier (0xBE -> 0xBD)
    OUTI                         ; DEC B, OUT(C),(HL) sur 0xBD -> CRTC_DAT, INC HL

    ; 3. Passage au registre suivant
    DEC  B                       ; B: 0xBD -> 0xBC (CRTC_REG)
    DEC  C                       ; registre suivant
    JP   P, @BC_CRTC             ; boucle tant que C >= 0 (sign flag clear)
MEND

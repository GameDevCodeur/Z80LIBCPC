
; ============================================================
; ROUTINES DE GESTION D'ÉCRAN POUR AMSTRAD CPC
; ============================================================
;
; CES ROUTINES PERMETTENT DE CONVERTIR DES COORDONNÉES ÉCRAN
; (X EN OCTETS, Y EN LIGNES) EN ADRESSES MÉMOIRE VIDÉO.
;
; LAYOUT MÉMOIRE ÉCRAN CPC (MODE 0) :
; - ORGANISATION NON LINÉAIRE PAR BLOCS DE 8 LIGNES
; - SAUTS DE &4000 ENTRE LES BLOCS
; - DEUX BANQUES POSSIBLES : &C000 ET &8000
;
; FONCTIONS :
; - GET_SCR_ADDRESSE_HL   : CONVERTIT (X,Y) -> ADRESSE ÉCRAN
;
; VARIABLES GLOBALES :
; - SCR_ADDR_TABLE    : POINTEUR VERS LA TABLE ACTIVE (C0 OU 80)
; - SCR_ADDR_TABLE_C0 : TABLE POUR LA BANQUE &C000
; - SCR_ADDR_TABLE_80 : TABLE POUR LA BANQUE &8000
;
; ============================================================

; ------------------------------------------------------------
; F_SRC_ADRESSE_HL
; ------------------------------------------------------------
; CONVERTIT DES COORDONNÉES ÉCRAN (X OCTET, Y LIGNE) EN
; ADRESSE MÉMOIRE VIDÉO EN UTILISANT LES TABLES PRÉ-GÉNÉRÉES.
;
; ENTRÉES   :  H = COORDONNÉE X (EN OCTETS, 0..79 POUR MODE 0)
;              L = COORDONNÉE Y (LIGNE, 0..199 POUR MODE 0)
; SORTIES   : HL = ADRESSE MÉMOIRE ÉCRAN
; REGISTRES AFFECTÉS : AF, BC, DE
; ------------------------------------------------------------
F_SCR_ADRESSE_HL

    PUSH DE                     ; SAUVEGARDE DE

    ; CALCULE L'OFFSET DANS LA TABLE À PARTIR DE Y
    LD A, H                     ; SAUVEGARDE X DANS A
    LD H, 0                     ; PRÉPARE HL POUR MULTIPLICATION
    ADD HL, HL                  ; ×2
    ADD HL, HL                  ; ×4
    ADD HL, HL                  ; ×8
    ADD HL, HL                  ; ×16
    ADD HL, HL                  ; ×32
    ; HL = Y × 32

    ; RÉCUPÈRE L'ADRESSE DE BASE DE LA TABLE
    LD  DE, (SCR_ADDR_TABLE)
    ADD HL, DE                   ; HL POINTE VERS L'ENTRÉE Y

    ; LIT L'ADRESSE DE DÉBUT DE LIGNE
    LD  C, (HL)                  ; OCTET BAS
    INC HL
    LD  B, (HL)                  ; OCTET HAUT
    ; BC = ADRESSE DE DÉBUT DE LA LIGNE Y

    ; CALCULE L'OFFSET X
    LD 	L, A                     ; RESTAURE X
    LD 	H, 0
    ADD HL, HL                   ; ×2
    ADD HL, HL                   ; ×4
    ; HL = X × 4

    ; ADRESSE FINALE
    ADD HL, BC

    POP DE                       ; RESTAURE DE
    RET

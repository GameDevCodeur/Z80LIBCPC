
;-----------------------------------------------------------
; F_SCR_CREER_TABLE
;
; Construit la table relative_adresses (SoA) : pour chaque
; ligne (B itérations), enregistre l'adresse écran courante
; (DE) dans la table à l'index courant (HL), en octet bas
; (page relative_adresses) et octet haut (page +256), puis
; avance DE à la ligne écran suivante (gère le passage entre
; blocs de 8 lignes entrelacées, écran CPC).
;
; Entrée : HL -> index courant dans relative_adresses
;                (octet bas de page, page alignée sur 256)
;          DE -> adresse écran de départ (ex: #C000)
;          B  -> nombre de lignes à traiter
;
; Sortie : relative_adresses / +256 remplies
;          HL avancé de B (index final)
;          DE avancé à la ligne suivant la dernière traitée
;
; Détruit: AFDEHL!
;-----------------------------------------------------------

F_SCR_CREER_TABLE

    LD (HL), E : INC H : LD (HL), D : DEC H : INC L
    
    EX DE, HL

        LD  A, H
        ADD A, 8
        LD  H, A
        JR  NC, .ENDBC26
        LD  A, L
        ADD A, #50
        LD  L, A
        LD  A, H
        ADC A, #C0
        LD  H, A

.ENDBC26

    EX DE, HL
    DJNZ F_SCR_CREER_TABLE

    RET

;LD   HL, relative_adresses      ; index 0 (octet bas de page)
;LD   DE, #C000                  ; adresse écran de départ
;LD   B, 200                     ; nombre de lignes écran (ex: 200 en mode CPC)
;CALL F_SCR_CREER_TABLE

;    ALIGN 256                   ; garantit que octet-haut = octet-bas + 256
;RELATIVE_ADRESSES
;    DEFS 256, 0                 ; page des octets bas (rempli à l'init)
;RELATIVE_ADRESSES_HIGH
;    DEFS 256, 0                 ; page des octets hauts (rempli à l'init)

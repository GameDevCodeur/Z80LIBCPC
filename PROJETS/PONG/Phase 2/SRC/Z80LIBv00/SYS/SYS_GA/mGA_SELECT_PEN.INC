
;-----------------------------------------------------------
; mGA_SELECT_PEN - Sélectionne un stylo dans le Gate Array
;
; Entrée : pen -> numéro du stylo (0 à 15)
; Sortie : Le Gate Array est prêt à recevoir une couleur
; Détruit: A, B (C est ignoré par le GA)
;-----------------------------------------------------------
MACRO mGA_SELECT_PEN pen
    LD   B, HI(GA_PORT)  ; Bits 15=0, 14=1 → sélection du GA
    LD   A, {pen}        ; Numéro du stylo
    OUT  (C), A
ENDM

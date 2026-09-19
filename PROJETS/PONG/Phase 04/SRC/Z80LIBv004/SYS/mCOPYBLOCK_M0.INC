
;-----------------------------------------------------------
; mCOPYBLOCK_M0
;
; Copie un bloc de 8 lignes × 4 pixels (2 octets/ligne),
; Mode 0, de HL (source) vers DE (destination écran).
;
; PRÉREQUIS : DE doit pointer sur le DÉBUT d'un bloc de 8
; lignes écran (bits 5-4-3 de D = 000). Ne gère PAS le
; changement de bande — si ce prérequis n'est pas respecté,
; le comportement est incorrect (pas de détection d'erreur).
;
; Entrée : HL -> données source (8 lignes consécutives)
;          DE -> adresse écran destination (début de bloc)
; Sortie : HL, DE avancés (positions indéfinies après la
;          dernière ligne, ne pas réutiliser directement)
; Détruit: AF
;-----------------------------------------------------------

MACRO mCOPYBLOCK_M0
	REPEAT 7
		LDI : LDI 
		DEC DE : DEC DE
		LD  A,D : ADD A,8 : LD D,A
	REND
	LDI : LDI
ENDM

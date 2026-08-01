
;-----------------------------------------------------------
; mKEY_ANY_REQUESTED
;
; Macro : teste si une touche quelconque de la matrice
; clavier est actuellement appuyée.
;
; Entrée : KB_MATRIX -> table de 10 octets à jour (scan IM1)
; Sortie : Z  = aucune touche appuyée
;          NZ = au moins une touche appuyée
; Détruit: AF, B, HL
;-----------------------------------------------------------
MACRO mKEY_ANY_REQUESTED
	LD   HL, KBD_BUFFER_RAM
	LD   B, 10
	LD   A, #FF
.KB_ANY_LOOP
	AND  (HL)
	INC  HL
	DJNZ .KB_ANY_LOOP
	CP   #FF          ; si A = FF, aucun bit à 0 -> rien appuyé
ENDM
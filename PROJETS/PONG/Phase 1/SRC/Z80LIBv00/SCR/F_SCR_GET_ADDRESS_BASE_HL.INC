
;-----------------------------------------------------------
; F_SCR_GET_ADDRESS_BASE_HL
;
; Retourne l'adresse de base de l'écran actuellement affiché
; (buffer courant). La valeur est auto-modifiable via le
; point de patch .base_screen (mis à jour ailleurs lors du
; swap de double-buffering).
;
; Entrée : -
; Sortie : HL -> adresse de base de l'écran courant
; Détruit: -
;-----------------------------------------------------------
F_SCR_GET_ADDRESS_BASE_HL

	LD HL, #4000 
	.BASE_SCREEN EQU $-2

RET

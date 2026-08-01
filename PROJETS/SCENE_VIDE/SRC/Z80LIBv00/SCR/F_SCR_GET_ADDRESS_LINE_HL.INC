;-----------------------------------------------------------
; F_SCR_GET_ADDRESS_LINE_HL
;
; Retourne l'adresse écran de la ligne demandée, en
; combinant la base de l'écran courant (F_SCR_GET_ADDRESS_BASE)
; avec l'offset relatif de cette ligne, lu dans la table
; RELATIVE_ADRESSES (organisée en SoA : octets bas à
; RELATIVE_ADRESSES, octets hauts 256 octets plus loin).
;
; Entrée : A -> numéro de ligne
;          RELATIVE_ADRESSES -> table d'offsets (SoA, alignée
;                                sur 256 octets)
; Sortie : HL -> adresse écran de la ligne A
;          A  -> inchangé
; Détruit: AF (via l'appel), DE
;-----------------------------------------------------------
F_SCR_GET_ADDRESS_LINE_HL

	LD 	 H, HI(RELATIVE_ADRESSES)
	LD   L, A
	LD   E, (HL)
	INC  H
	LD   D, (HL)
	CALL F_SCR_GET_ADDRESS_BASE
	ADD  HL, DE

RET


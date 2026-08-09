
;-----------------------------------------------------------
; F_ENCODE_COLOR_RIGHT_PIXEL_M0
;
; Convertit un numéro de couleur logique (0-15) vers l'octet
; encodé au format entrelacé du pixel DROIT en Mode 0 CPC.
; Mapping : bit0(A)->bit6(A), bit1(A)->bit2(A),
;           bit2(A)->bit4(A), bit3(A)->bit0(A)
; Vérifié par calcul croisé ET confirmé contre l'implémentation
; BIT/SET d'origine (get_byte_for_color_0_R).
;
; Entrée : A -> numéro de couleur (0-15, bits 0-3 utilisés)
; Sortie : A -> octet encodé
; Détruit: AF, HL
;-----------------------------------------------------------
F_ENCODE_COLOR_RIGHT_PIXEL_M0
	LD   HL, .TABLE_COLOR_RIGHT
	AND  #0F
	ADD  A, L
	LD   L, A
	LD   A, (HL)
	RET
	
	ALIGN 16
.TABLE_COLOR_RIGHT
	DEFB 0x00, 0x40, 0x04, 0x44
	DEFB 0x10, 0x50, 0x14, 0x54
	DEFB 0x01, 0x41, 0x05, 0x45
	DEFB 0x11, 0x51, 0x15, 0x55
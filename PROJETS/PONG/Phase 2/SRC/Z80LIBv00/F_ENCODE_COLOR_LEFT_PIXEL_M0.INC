
;-----------------------------------------------------------
; F_ENCODE_COLOR_LEFT_PIXEL_M0
;
; Convertit un numéro de couleur logique (0-15) vers l'octet
; encodé au format entrelacé du pixel GAUCHE en Mode 0 CPC.
; Mapping : bit0(B)->bit7(C), bit1(B)->bit3(C),
;           bit2(B)->bit5(C), bit3(B)->bit1(C)
;
; Entrée : A -> numéro de couleur (0-15, bits 0-3 utilisés)
; Sortie : A -> octet encodé
; Détruit: AF, HL
;-----------------------------------------------------------
F_ENCODE_COLOR_LEFT_PIXEL_M0
	LD   HL, .TABLE_COLOR_LEFT
	AND  #0F
	ADD  A, L
	LD   L, A
	LD   A, (HL)
	RET

	ALIGN 16
.TABLE_COLOR_LEFT
	DEFB 0x00, 0x80, 0x08, 0x88
	DEFB 0x20, 0xA0, 0x28, 0xA8
	DEFB 0x02, 0x82, 0x0A, 0x8A
	DEFB 0x22, 0xA2, 0x2A, 0xAA

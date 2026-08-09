
;-----------------------------------------------------------
; mCOPYBLOCK_WH_M0 NBLINES, WIDTH
;
; Copie un bloc de NBLINES lignes × WIDTH octets (Mode 0),
; de HL (source) vers DE (destination écran), en gérant le
; passage de bande via mSET_NEXTLINE_DE.
;
; Entrée : HL -> données source (NBLINES lignes consécutives,
;               WIDTH octets chacune)
;          DE -> adresse écran destination
; Détruit: AF, HL, DE
;-----------------------------------------------------------
MACRO mCOPYBLOCK_HW_M0 NBLINES, WIDTH
	REPEAT {NBLINES}-1
		REPEAT {WIDTH}
			LDI
		REND
		REPEAT {WIDTH}
			DEC DE
		REND
		mSET_NEXTLINE_DE (VOID)
	REND
	REPEAT {WIDTH}
		LDI
	REND
ENDM

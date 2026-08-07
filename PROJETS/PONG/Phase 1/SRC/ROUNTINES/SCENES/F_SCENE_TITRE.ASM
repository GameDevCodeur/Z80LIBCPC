
;==============================================================
; SCENE_TITRE - Initialisation
;--------------------------------------------------------------
; Description : Initialisation de l'état "Titre".
;               Configure le mode graphique, décompresse
;               l'écran titre et charge sa palette.
;==============================================================

F_SCENE_TITRE_INITIALISATION
	
    mGA_SET_MODE GA_MODE0_ROMS_OFF

    ; DECOMPRESS ZX0
    LD   HL, TITRE_ZX0_DATA
    LD   DE, TITRE_VRAM
    CALL F_ZX0_DECOMPRESS

    ; CHARGE LA PALETTE    
    LD   HL, TITRE_PAL_DATA
    LD   BC, GA_PORT 
    LD    A, TITRE_PALETTE_SIZE
    mGA_SET_PALETTE (VOID)
    
    RET

;==============================================================
; SCENE_TITRE - Boucle principale
;==============================================================
F_SCENE_TITRE
  
    LD   HL, TITRE_TEMPO_DATA
    DEC  (HL) : RET  NZ

    JP   F_ETEINDRE_ENCRES
    
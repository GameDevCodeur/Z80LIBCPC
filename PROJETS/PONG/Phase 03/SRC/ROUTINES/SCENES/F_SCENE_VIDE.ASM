
; -------------------------------------------------------------------------------
; INITIALISATION DE LA SCÈNE VIDE
; -------------------------------------------------------------------------------
F_SCENE_VIDE_INITIALISATION
   
   ; SET MODE 0
   mGA_SETMODE GA_MODE_0|GA_UROM_DISABLE|GA_LROM_DISABLE|GA_IRQ_RESET

   ; DECOMPRESS ZX0
   LD   HL, TITRE_ZX0_DATA
   LD   DE, TITRE_VRAM
   CALL F_ZX0_DECOMPRESS

   ; CHARGE LA PALETTE    
   LD   HL, TITRE_PAL_DATA
   LD   BC, GA_PORT_CMD 
   LD    A, TITRE_PALETTE_SIZE
   mGA_SET_PALETTE (VOID)

RET    

; -------------------------------------------------------------------------------
; RUNTIME : F_SCENE_VIDE 
; -------------------------------------------------------------------------------
F_SCENE_VIDE
   ;LOGIQUE
   ;AFFICHAGE
RET


; ===============================================================
; SCENE MENU - Initialisation
; ===============================================================

F_SCENE_MENU_INITIALISATION

    ; Eteindre écran
    CALL F_ETEINDRE_ENCRES 

    ; Nettoyer écran
    mCLEAR_BUFFER SCR_VRAM_BASE     ; AFHL!

    ; Afficher logo PONG
    LD   HL, MENU_ZX0_DATA
    LD   DE, MENU_LOGO_VRAM
    CALL F_ZX0_DECOMPRESS

    ; Afficher "PRESS KEY TO START"
    mAFFICHER_BLOCK_CHAR_M0 CHAR_P, MENU_TEXT_VRAM+00
    mAFFICHER_BLOCK_CHAR_M0 CHAR_R, MENU_TEXT_VRAM+02
    mAFFICHER_BLOCK_CHAR_M0 CHAR_E, MENU_TEXT_VRAM+04
    mAFFICHER_BLOCK_CHAR_M0 CHAR_S, MENU_TEXT_VRAM+06
    mAFFICHER_BLOCK_CHAR_M0 CHAR_S, MENU_TEXT_VRAM+08
    mAFFICHER_BLOCK_CHAR_M0 CHAR_K, MENU_TEXT_VRAM+12
    mAFFICHER_BLOCK_CHAR_M0 CHAR_E, MENU_TEXT_VRAM+14
    mAFFICHER_BLOCK_CHAR_M0 CHAR_Y, MENU_TEXT_VRAM+16
    mAFFICHER_BLOCK_CHAR_M0 CHAR_T, MENU_TEXT_VRAM+20
    mAFFICHER_BLOCK_CHAR_M0 CHAR_O, MENU_TEXT_VRAM+22
    mAFFICHER_BLOCK_CHAR_M0 CHAR_S, MENU_TEXT_VRAM+26
    mAFFICHER_BLOCK_CHAR_M0 CHAR_T, MENU_TEXT_VRAM+28
    mAFFICHER_BLOCK_CHAR_M0 CHAR_A, MENU_TEXT_VRAM+30
    mAFFICHER_BLOCK_CHAR_M0 CHAR_R, MENU_TEXT_VRAM+32
    mAFFICHER_BLOCK_CHAR_M0 CHAR_T, MENU_TEXT_VRAM+34

    ; CHARGE LA PALETTE
    LD   HL, MENU_PAL_DATA
    LD   BC, GA_PORT 
    LD    A, MENU_PALETTE_SIZE
    mGA_SET_PALETTE (VOID)

RET    

; ===============================================================
; SCENE MENU - Boucle principale
; ===============================================================

F_SCENE_MENU
   
; -----------------------------------------------------------
; FLASH_TOGGLE — Bascule de couleur sur PEN 6 (Gate Array)
;
; Entrée : MENU_FLASH_COUNTER_DATA (compteur 1 octet)
;          MENU_FLASH_STATE_DATA (état de bascule 1 octet)
; Sortie : PEN 6 alterné entre WHITE et BLACK
; Détruit: AF, HL, B (et C si utilisé pour OUT)
; -----------------------------------------------------------
 FLASH_TOGGLE

    LD   HL, MENU_FLASH_COUNTER_DATA
    DEC  (HL)
    JR   NZ,  FLASH_TOGGLE_         ; Compteur non nul → on saute

    LD   (HL), MENU_FLASH_COUNTER_1 ; Rechargement du compteur (ex: 50)

    LD   HL, MENU_FLASH_STATE_DATA
    RRC  (HL)                       ; Bascule d'état : Carry alterne 0/1

    ; --- Écriture dans le Gate Array ---
    mGA_SELECT_PEN GA_PEN_6         ; Selectionne stylo 6                    

    JR   NC, .set_black             ; Carry=0 → noir
    
    ; Chemin blanc
    LD   A, HW_BRIGHT_WHITE
    JR   .write_color

.set_black:
    LD   A, HW_BLACK

.write_color:
    OUT  (C), A                     ; Écriture de la couleur

FLASH_TOGGLE_
  
   mKEY_ANY_REQUESTED_FAST (VOID) : RET Z
   JP F_ETEINDRE_ENCRES 

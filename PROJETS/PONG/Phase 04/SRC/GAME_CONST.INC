
; ----------------------
; PARAMETRES SCENES        ; Identifiants des scènes
; ----------------------
ID_SCENE_TITRE             EQU   0
ID_SCENE_MENU              EQU   1

; ----------------------
; PARAMETRE COLOR GAME
; ----------------------

; ----------------------
; PARAMETRES TITRE         ; Paramètres de la scène TITRE
; ----------------------
TITRE_VRAM                 EQU   SCR_VRAM_BASE                                                 ; #C000
TITRE_TEMPO                EQU   2*50                                                          ; 2s:N=2*50/50

; -------------------
; PARAMETRES MENU         ; Paramètres de la scène MENU
; -------------------
MENU_LOGO_VRAM             EQU   SCR_VRAM_BASE                                                 ; #C000
MENU_TEXT_VRAM             EQU   SCR_VRAM_BASE+(16*SCR_WIDTH_BYTE)+(7*SCR_PX_PER_BYTE_M0)      ; #C30E
MENU_PALETTE_SIZE          EQU   7                                                             ;     7
MENU_FLASH_COUNTER_1       EQU   1*50                                                          ; 1s:N=1*50/50

; ----------------------
; PARAMETRES GAMEPLAY     ; Paramètres de la scène GAMEPLAY
; ----------------------

; ----------------------
; PARAMETRES GAMEOVER     ; Paramètres de la scène GAMEOVER
; ----------------------



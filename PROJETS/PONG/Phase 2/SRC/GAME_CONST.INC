
; -------------------
; PARAMETRES SCENES        ; Identifiants des scènes
; -------------------
ID_SCENE_TITRE             EQU   0
ID_SCENE_MENU              EQU   1

; -------------------
; PARAMETRES TITRE         ; Paramètres de la scène TITRE
; -------------------
TITRE_VRAM                 EQU   SCR_VRAM_BASE
TITRE_TEMPO_3              EQU   3*50                          ; 3s:N=3*50/50
TITRE_PALETTE_SIZE         EQU   11

; -------------------
; PARAMETRES MENU         ; Paramètres de la scène MENU
; -------------------
MENU_LOGO_VRAM             EQU   SCR_VRAM_BASE
MENU_TEXT_VRAM             EQU   SCR_VRAM_BASE + (16*64)+(7*2) ; #C30E
MENU_PALETTE_SIZE          EQU   6+1
MENU_FLASH_COUNTER_1       EQU   1*50                          ; 1s:N=1*50/50

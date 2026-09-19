
;================================================================
;  INSERTION DES DONNEES SCENE MENU
;================================================================

    INCLUDE "LETTRES_DATA.INC"

MENU_PAL_DATA
    INCLUDE "MENU_DATA.PAL"

MENU_ZX0_DATA
    INCZX0 "MENU_ZX0_DATA.BIN"

;================================================================
;  STRUCTURE DE DONNEES FLASH_ENTITY SCENE MENU
;================================================================
MENU_FLASH_ENTITY
MENU_FLASH_ENTITY.COUNTER      DEFB MENU_FLASH_COUNTER_1
MENU_FLASH_ENTITY.COUNTERVALUE DEFB MENU_FLASH_COUNTER_1
MENU_FLASH_ENTITY.STATE        DEFB 0 
MENU_FLASH_ENTITY.PEN          DEFB GA_PEN_6 
MENU_FLASH_ENTITY.COLORA       DEFB GA_COLOR_BRIGHT_WHITE 
MENU_FLASH_ENTITY.COLORB       DEFB GA_COLOR_BLUE

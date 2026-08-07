
;============================================================
; CONSTANTES SYSTÈME CPC
;============================================================

;------------------------------------------------------------
; IM1 - INTERRUPTION 1
;------------------------------------------------------------
IM1_NOT_READY		EQU -2
IM1_FIRST_VALUE		EQU -1

;------------------------------------------------------------
; CRTC — HD6845
;------------------------------------------------------------
INCLUDE "SYS/SYS_CRTC/CONSTANTES.INC"

;------------------------------------------------------------
; PPI 8255 — Interface Périphériques
;------------------------------------------------------------
PPI_PORT_A          EQU #F400           ; port A
PPI_PORT_B          EQU #F500           ; port B
PPI_PORT_C          EQU #F600           ; port C
PPI_CTRL            EQU #F700           ; port contrôle

PPI_READ_KBD        EQU %10010010       ; A=IN  B=IN  C=OUT — lecture clavier
PPI_RESET           EQU %10000010       ; A=OUT B=IN  C=OUT — mode défaut

KBD_BUFFER_RAM      EQU #0040           ; adresse buffer clavier

KEYCODE_RETURN      EQU 18
KEYCODE_SPACE       EQU 47  
KEYCODE_ESCAPE      EQU 66
KEYCODE_Q           EQU 67
KEYCODE_A           EQU 69

KBD_BUFFER_UP	    EQU KBD_BUFFER_RAM+0
KBD_BUFFER_ENTER    EQU KBD_BUFFER_RAM+2
KBD_BUFFER_SPACE    EQU KBD_BUFFER_RAM+5 
KBD_BUFFER_A    	EQU KBD_BUFFER_RAM+8
KBD_BUFFER_K        EQU KBD_BUFFER_RAM+4
KBD_BUFFER_M        EQU KBD_BUFFER_RAM+4
KBD_BUFFER_N        EQU KBD_BUFFER_RAM+5
KBD_BUFFER_P        EQU KBD_BUFFER_RAM+3
KBD_BUFFER_Q    	EQU KBD_BUFFER_RAM+8
KBD_BUFFER_Y        EQU KBD_BUFFER_RAM+5

KBD_KEY_A           EQU %11011111 		; #DF
KBD_KEY_K           EQU %11011111 		; #DF
KBD_KEY_N           EQU %10111111 		; #BF
KBD_KEY_M           EQU %10111111 		; #BF
KBD_KEY_P           EQU %11110111 		; #F7
KBD_KEY_Q           EQU %11110111 		; #F7
KBD_KEY_Y           EQU %11110111 		; #F7
KBD_KEY_SPACE       EQU %01111111       ; #7F
KBD_KEY_ENTER       EQU %11111011       ; #FB
KBD_KEY_UP		    EQU %11111110       ; #FE

;------------------------------------------------------------
; PSG — Générateur de Son
;------------------------------------------------------------
PSG_REG14_KBD       EQU %1110           ; registre 14 — clavier
PSG_SELECT_REG      EQU %11000000       ; commande sélection registre

;------------------------------------------------------------
; GATE ARRAY — Couleurs Hardware
;------------------------------------------------------------
INCLUDE "SYS/SYS_GA/CONSTANTES.INC"

;------------------------------------------------------------
; FIRMWARE
;------------------------------------------------------------
INCLUDE "SYS/FIRMWARE.INC"

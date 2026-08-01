
; =============================================================================
; PARAMETRES Z80LIBv00
; =============================================================================
INCLUDE "Z80LIBv00\CONSTANTES.INC" 

; =============================================================================
; PARAMETRES Z80LIB PARAMETRABLES
; =============================================================================

; -------------------
; PARAMETRES STACK			; Pile systéme
; -------------------
SP_SIZE             EQU  32*2   	        ; Réservation en N*2 octets (défault: 32*2)

; -------------------
; PARAMETRES CRTC 			; Géométrie de l'écran
; -------------------
CRTC_R1             EQU    40               ; Largeur  visible en caractères
CRTC_R2             EQU    46               ; Position de synchrinonisation horizontale
CRTC_R6             EQU    25               ; Hauteur  visible en lignes de caractères
CRTC_R12            EQU   #30               ; Adresse  de départ écran poids fort (#30 -> #C0)
CRTC_R13            EQU    00               ; Adresse  de départ écran poids faible

; -------------------
; PARAMETRES IM1            ; Gestionnaire IM1 
; -------------------
IM1_IT_MAX      	EQU     6               ; Taille table IM1 d'une scéne (6 = 50hz)

; -------------------
; PARAMETRES ECRAN 			; Géométrie de l'écran
; -------------------
SCR_VRAM_BASE         EQU #C000             ; CRTC (R12-R13)
SCR_WIDTH_BYTE        EQU CRTC_R1*2         ; 64 Bytes (32 Blocs) -   80B (40 Blocs)
SCR_PX_WIDTH_M0       EQU CRTC_R1*4         ; 1..128px (32 Blocs) - 160px (40 Blocs)
SCR_PX_HEIGHT_M0      EQU CRTC_R6*8         ; 1..192px (24 Blocs) - 200px (25 Blocs)
SCR_PX_PER_BYTE_M0    EQU 2                 ;      2px ( 1 Byte ) - Mode 0 
SCR_PX_BLOC_WIDTH_M0  EQU 4
SCR_PX_BLOC_HEIGHT_M0 EQU 8

; ------------------
; ORIGINE MEMOIRE 			; Implentation en mémoire
; ------------------
DEBUT_CODE 			 EQU #4000

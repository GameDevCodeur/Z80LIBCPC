
; =============================================================================
; PARAMETRES DES FONCTIONS
; =============================================================================
ID_ZX0_DECOMPRESS     = 1                  ; 0 - Inactive 
ID_ETEINDRE_ENCRES    = 1                  ; 0 - Inactive 

; =============================================================================
; PARAMETRES Z80LIB
; =============================================================================

; ------------------
; ORIGINE MEMOIRE           ; Implentation en mémoire
; ------------------
DEBUT_CODE            EQU  #4000

; -------------------
; PARAMETRE STACK			; Pile systéme
; -------------------
SP_SIZE               EQU  32*2               ; Réservation en N*2 octets (défault: 64)

; -------------------
; PARAMETRES CRTC 			; Géométrie de l'écran 128x192 (160x200)
; -------------------
CRTC_R1               EQU    32               ; Largeur  visible en caractères (40)
CRTC_R2               EQU    42               ; Position de synchrinonisation horizontale (46)
CRTC_R6               EQU    24               ; Hauteur  visible en lignes de caractères (25)

CRTC_R12              EQU   #30               ; Adresse  de départ écran poids fort (#30 -> #C0)
CRTC_R13              EQU    00               ; Adresse  de départ écran poids faible (00)

; -------------------
; PARAMETRES IM1            ; Gestionnaire IM1 
; -------------------
IM1_IT_MAX            EQU  6                  ; Taille table IM1 d'une scéne (6 = 50hz)

; -------------------
; PARAMETRES ECRAN 			; Géométrie de l'écran
; -------------------
SCR_VRAM_BASE         EQU #C000               ; CRTC (R12-R13)
SCR_WIDTH_BYTE        EQU CRTC_R1*2           ; 64 Bytes (32 Blocs)
SCR_PX_WIDTH_M0       EQU CRTC_R1*4           ; 1..128px (32 Blocs)
SCR_PX_HEIGHT_M0      EQU CRTC_R6*8           ; 1..192px (24 Blocs)
SCR_PX_PER_BYTE_M0    EQU 2                   ;      2px ( 1 Byte ) - Mode 0 
SCR_PX_BLOC_WIDTH_M0  EQU 4
SCR_PX_BLOC_HEIGHT_M0 EQU 8

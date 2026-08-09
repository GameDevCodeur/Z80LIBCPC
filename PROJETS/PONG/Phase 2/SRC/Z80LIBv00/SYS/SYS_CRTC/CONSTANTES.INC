
;------------------------------------------------------------
; CRTC — HD6845
;------------------------------------------------------------

CRTC_REG            EQU #BC00           ; port sélection registre
CRTC_DAT            EQU #BD00           ; port écriture donnée

; Horizontal Total
CRTC_R0_DEFAULT 	EQU 63 				; Largeur totale de ligne (caractères), 
										; fixe le timing HSYNC.
; Horizontal Displayed
CRTC_R1_DEFAULT 	EQU 40 				; Nombre de caractères affichés par ligne.

; Horizontal Sync Position
CRTC_R2_DEFAULT 	EQU 46 				; Position du HSYNC sur la ligne

; Sync Width
CRTC_R3_DEFAULT 	EQU #8E 			; Largeur HSYNC (bits 0-3)
										; VSYNC (bits 4-7)	142 (0x8E) 
										; HSYNC = 14, VSYNC = 8
; Vertical Total
CRTC_R4_DEFAULT 	EQU 38 				; Hauteur totale de trame (lignes de caractères)

; Vertical Total Adjust
CRTC_R5_DEFAULT 	EQU 2 				; Ajustement fin (lignes raster supplémentaires)

; Vertical Displayed
CRTC_R6_DEFAULT 	EQU 25 				; Nombre de lignes de caractères affichées

; Vertical Sync Position
CRTC_R7_DEFAULT 	EQU 30 				; Position du VSYNC

; Interlace & Skew
CRTC_R8_DEFAULT 	EQU 0   			; Mode entrelacé, alignement d'affichage

; Maximum Raster Address
CRTC_R9_DEFAULT 	EQU 7 				; Nombre de lignes raster par caractère - 1

; Cursor Start Raster
CRTC_R10_DEFAULT 	EQU 0 				; Ligne de début du curseur, mode clignotement

; Cursor End Raste
CRTC_R11_DEFAULT 	EQU 0 				; Ligne de fin du curseur

; ------------------------------------------------------------------------------
; Display Start Address
; ------------------------------------------------------------------------------
; R12  R13  -  Address
; #30  #00  -  #C000
; #20  #00  -  #8000
; #10  #00  -  #4000
; #00  #00  -  #0000
; ------------------------------------------------------------------------------
; Exemple : R12 = #30 R13= #00
;
; Étape 1 — Combiner R12 et R13 en valeur MA 14 bits
; MA (14 bits) = (R12  << 8) | R13
;              = (0x30 << 8) | 0x00 = 0x3000
; En binaire (14 bits) : MA = 11 0000 0000 0000
;
; Étape 2 — Extraire la banque mémoire (bits 13-12 de MA = bits 5-4 de R12)
; R12 = 0x30 = 00110000 (binaire) - bits 5-4  = 11
;
; La banque vidéo courante est calculée comme (MA12,MA13) × 0x4000. 
; Les banques vidéo sont 0x0000, 0x4000, 0x8000 ou 0xc000
;
; bits 5-4 = 11 (binaire) = 3 (décimal)
; banque = 3 × 0x4000 = 0xC000
;
; Étape 3 — Offset à l'intérieur de la banque (bits 3-0 de R12 et R13)
; R12 bits 3-0 = 0000   (contribue à MA8-MA11, utilisé seulement pour l'overscan 32K)
; R13          = 0x00   (contribue à MA0-MA7, offset bas standard)
;
; Étape 4 — Application du facteur "2 octets par caractère" (Mode 1)
; Le matériel d'affichage CPC récupère deux octets par caractère CRTC. 
; La longueur d'une ligne CRTC en octets est donc (R1×2)
; Puisque MA (le compteur interne du CRTC) compte en caractères, pas en octets, 
; et que l'offset ici (bits 0-11 de MA) vaut 0, 
; la conversion en octets donne aussi 0 — pas de décalage supplémentaire à ce stade.
; 
; Adresse écran = banque + offset_octets
;               = 0xC000 + 0
;               = 0xC000
; -----------------------------------------------------------------------------
CRTC_R12_DEFAULT 	EQU #30 			; Octet haut de l'adresse de début d'affichage
CRTC_R13_DEFAULT 	EQU 0 				; Octet bas de l'adresse de début d'affichage

; -------------------------------
; Cursor Address
; -------------------------------
CRTC_R14_DEFAULT 	EQU 0 				; Octet haut de la position du curseur texte
CRTC_R15_DEFAULT 	EQU 0 				; Octet bas de la position du curseur texte

; -------------------------------
; Light Pen
; -------------------------------
CRTC_R16_DEFAULT 	EQU 0 				; Lecture seule — position du crayon optique
CRTC_R17_DEFAULT 	EQU 0 				; Lecture seule — position du crayon optique

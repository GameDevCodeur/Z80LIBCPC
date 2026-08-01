;===============================================================
; mCLEAR_BUFFER_LDIR
;---------------------------------------------------------------
; Description : Remplit un buffer mémoire de 16384 octets avec
;               la valeur 0, en utilisant la copie en cascade
;               (LDIR auto-propagée).
;
; Entrée      : HL = adresse de départ du buffer
;               (doit être positionné AVANT l'appel de la macro)
;
; Sortie      : Buffer rempli de 0 sur 16384 octets
;
; Registres modifiés : AFBCDEHL!
;
; Taille du buffer : fixe, 16384 octets (ex: VRAM CPC #C000-#FFFF)
;
; Exemple d'utilisation :
;               LD   HL, #C000
;               mCLEAR_BUFFER
;===============================================================
MACRO mCLEAR_BUFFER_LDIR
    LD      (HL),0              ; 1er octet = 0
    LD      E,L                 ; DE = HL
    LD      D,H
    INC     DE                  ; DE = HL+1 (destination)
    LD      BC,16383            ; 16383 octets à copier
    LDIR                        ; Copie 0 dans tout le buffer
ENDM

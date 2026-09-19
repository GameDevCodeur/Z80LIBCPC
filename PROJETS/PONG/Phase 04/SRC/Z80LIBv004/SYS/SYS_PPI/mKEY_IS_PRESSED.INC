

; ------------------------------------------------------------
; MACRO   : mKEY_IS_PRESSED_BIT
; Rôle    : Teste si une touche du clavier CPC est actuellement
;           pressée, à partir du buffer rempli par
;           mPPI_READ_KEYBOARD.
;
; Paramètre : key = octet encodé (nibble haut = ligne 0-9,
;             nibble bas = bit 0-7 dans la ligne)
;
; Entrée  : KBD_BUFFER_RAM doit avoir été rempli au préalable
;           par mPPI_READ_KEYBOARD (même frame ou dernier scan)
;
; Sortie  : Z = 1 si la touche est pressée
;           Z = 0 sinon
;
; Détruit : AF
; ------------------------------------------------------------
MACRO mKEY_IS_PRESSED_BIT key

    LD   A, (KBD_BUFFER_RAM + (({key} >> 4) & #0F))
    BIT  ({key} & #0F), A

MEND



;===============================================================
; F_AFFICHE_CHAR_M0
;---------------------------------------------------------------
; Description : Affiche un caractère 4x8 pixels (2 octets/ligne)
;               sur écran CPC Mode 0, ligne par ligne.
;
; Entrée      : HL = adresse du caractère source
;               DE = adresse destination en VRAM (début de bloc)
;
; Sortie      : HL, DE avancés (positions indéfinies après la
;               dernière ligne, ne pas réutiliser directement)
;
; Détruit     : AFHLDE!
;===============================================================



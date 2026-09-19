
;============================================================
; mGA_SET_PEN_INK
;
; Sélectionne l'encre du stylo courant sur le Gate Array.
;
; Entrées : A = PEN (numéro de stylo, 0-16 ; 16 = bordure)
;           D = COULEUR HARDWARE
;           B = GA_PORT (octet haut du port Gate Array)
; Sorties : -
; Détruits: (aucun)
;============================================================
MACRO mGA_SET_PEN_INK  
  OUT (C),A       ; Numéro du stylo
  OUT (C),D       ; Couleur du stylo
MEND

; SIZE : 4 octets

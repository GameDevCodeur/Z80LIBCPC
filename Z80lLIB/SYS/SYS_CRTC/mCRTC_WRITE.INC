






;-----------------------------------------------------------
; mCRTC_WRITE register, value
;
; Écrit une valeur dans un registre du CRTC (6845), en deux
; étapes : sélection du registre, puis écriture de la donnée.
;
; Paramètres : register -> numéro du registre CRTC (0-17)
;              value    -> valeur à écrire dans ce registre
; Entrée  : CRTC_REG -> port de sélection du registre CRTC
;           CRTC_DAT -> port d'écriture de la donnée CRTC
; Sortie  : -
; Détruit : BC
;-----------------------------------------------------------
MACRO mCRTC_WRITE register, value
 	LD   BC, CRTC_REG + {register}
    OUT  (C),C
    LD   BC, CRTC_DAT + {value}
    OUT  (C),C
ENDM

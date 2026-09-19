
; ------------------------------------------------------------
; MACRO   : mGA_SET_INK
; Rôle    : Affecte une couleur hardware à un pen donné, en une
;           seule expansion autonome (charge B elle-même —
;           contrairement à mGA_SET_PEN_INK qui suppose B déjà
;           chargé par l'appelant). Adaptée aux appels ponctuels
;           isolés (un seul pen à la fois).
;
; Paramètres :
;   PEN      = numéro de pen (0-15) ou GA_PEN_0 etc., ou &10
;              pour le border ; bits 7-6 doivent être 00
;   HW_COLOR = couleur hardware pré-encodée (bits 7-6 = 01)
;
; Sortie  : (aucune)
; Détruit : AF, BC, D
; Dépend  : mGA_SET_PEN_INK
; Notes   : Pour affecter plusieurs pens à la suite (ex. init
;           palette complète), préférer mGA_SET_PEN_INK dans
;           une boucle avec B chargé une seule fois en amont —
;           mGA_SET_INK rechargerait B inutilement à chaque
;           appel dans ce cas.
; ------------------------------------------------------------
    MACRO mGA_SET_INK PEN, HW_COLOR
        LD    A, {PEN}
        LD    B, GA_PORT_CMD_B
        LD    D, {HW_COLOR}
        mGA_SET_PEN_INK (VOID)
    MEND
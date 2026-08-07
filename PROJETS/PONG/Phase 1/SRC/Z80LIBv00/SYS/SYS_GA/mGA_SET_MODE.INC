
                    
; ============================================================
; MACRO   : mGA_SET_MODE
; Rôle    : Écrit dans le registre de mode d'interruption (IMR)
;           du Gate Array (Amstrad CPC).
;           La donnée est portée par le low byte de l'adresse.
; Entrée  : Paramètre IMR = valeur à écrire.
; Sortie  : Aucune
; Détruit : BC
; Dépend  : GA_PORT (doit être définie, ex: 0x7F00)
; Note    : Utilise l'instruction OUT (C), C spécifique au CPC.
; ============================================================
MACRO mGA_SET_MODE IMR
    LD   B, HI(GA_PORT)     ; High byte du port (ex: 0x7F)
    LD   C, {IMR}           ; Low byte = valeur à écrire
    OUT  (C), C             ; Écrit la valeur dans le GA
ENDM
  
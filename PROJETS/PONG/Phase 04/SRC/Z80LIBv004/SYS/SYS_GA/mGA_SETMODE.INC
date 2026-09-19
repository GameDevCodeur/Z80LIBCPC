; GA_SetMode -- ecrit le mode ecran et la visibilite des ROM en une seule
;               ecriture Gate Array (GA).
;
; Ecrit un octet dans le groupe de commande mode/ROM du GA. Le parametre
; Mode doit etre l'une des constantes GA_MODE_0, GA_MODE_1, GA_MODE_2 (ou
; GA_MODE_3 si necessaire). Les drapeaux ROM et IRQ peuvent etre combines
; par OR directement au point d'appel :
;
;   mGA_SetMode GA_MODE_1                              ; mode 1, ROMs actives, pas de reset IRQ
;   mGA_SetMode GA_MODE_1|GA_UROM_DISABLE              ; mode 1, ROM haute desactivee
;   mGA_SetMode GA_MODE_0|GA_LROM_DISABLE|GA_IRQ_RESET ; mode 0, ROM basse desactivee, reset compteur IRQ
;
; Remarque : l'etat complet mode+ROM est remplace a chaque ecriture -- il
; n'y a pas de lecture-modification-ecriture sur le GA. Toujours fournir
; l'ensemble des drapeaux souhaites en meme temps.
;
; Detruit : B, C
;
; Vitesse : 7 µs = 28 T-states CPC = 7 NOP (constant, sans branchement).
; Detail : ld bc,nn (3µs) + out (c),r (4µs) = 7µs.
;
MACRO mGA_SETMODE CMD
    LD BC,GA_PORT_CMD+{CMD}     ; B=#7F (PORT GA), C=OCTET DE COMMANDE MODE/ROM
    OUT (C),C                   ; ENVOIE LA COMMANDE DE MODE AU GATE ARRAY
ENDM

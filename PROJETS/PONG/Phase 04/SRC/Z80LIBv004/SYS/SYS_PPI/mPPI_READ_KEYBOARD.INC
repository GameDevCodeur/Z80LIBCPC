
; ------------------------------------------------------------
; MACRO   : mPPI_SEND_CMD
; Rôle    : Envoie une commande au PPI (BC = port|valeur).
;           Bloc élémentaire factorisé, réutilisé pour toutes
;           les phases de sélection/validation du PPI.
;
; Paramètre : CMD = valeur combinée port|donnée
; Détruit   : AF, BC
; ------------------------------------------------------------
MACRO mPPI_SEND_CMD CMD

    LD   BC, {CMD}
    OUT  (C), C

MEND

; ------------------------------------------------------------
; MACRO   : mPPI_READ_KEYBOARD
; Rôle    : Scanne les 10 lignes de la matrice clavier CPC
;           via le PPI, et stocke chaque ligne en RAM.
;
; Entrée  : (aucune)
; Sortie  : KBD_BUFFER_RAM+0 .. KBD_BUFFER_RAM+9 = état des
;           10 lignes clavier (bit à 0 = touche pressée)
; Détruit : AF, BC, HL
; Dépend  : mPPI_SEND_CMD
; Notes   : Bascule le PPI en mode lecture clavier (#92) puis
;           restaure le mode par défaut (#82) en fin de scan.
; ------------------------------------------------------------
MACRO mPPI_READ_KEYBOARD
    
    ; --- Adresse de fin du buffer ----
    LD   HL, KBD_BUFFER_RAM+9

    ; ---- Sélection du registre clavier PSG (registre 14) ----
    mPPI_SEND_CMD PPI_PORT_A|#0E  ; 00001110 Reg 14

    ; ---- Validation de la sélection ----
    mPPI_SEND_CMD PPI_PORT_C|#C0  ; 11000000 Selection 
    OUT  (C), 0                   ; 00000000 Validation

    ; ---- Passage en lecture clavier ----
    mPPI_SEND_CMD PPI_CTRL|#92    ; 10010010 Port A en entrée

    ; C = bit accès clavier (bit 6) | ligne clavier 9
    LD   C, PPI_C_KBD_ACCESS|9    ; Ligne 49

.scanLoop

    LD   B, HI(PPI_PORT_C)
    OUT  (C), C                   ; Read data ligne 9

    LD   B, HI(PPI_PORT_A)
    IND                           ; (HL) ← IN(C) ; HL-- ; B--

    DEC  C
    BIT  4, C                     ; test fin des 10 lignes 
    JR   Z, .scanLoop

    ; ---- Restauration mode PPI par défaut ----
    mPPI_SEND_CMD PPI_CTRL|#82    ; 10000010  Port A en sortie

MEND

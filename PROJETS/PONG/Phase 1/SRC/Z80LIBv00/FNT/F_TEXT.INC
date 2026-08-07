
;--------------------------------------------------------
; TXT_DRAW
; Affiche un message à l'écran
;
; Entrée  : IX    = adresse du message
;           (IX+0) = colonne X
;           (IX+1) = ligne Y
;           (IX+2) = masque couleur
;           (IX+3) = premier caractère
;           (IX+4+) = caractères suivants (0 = fin)
; Sortie  : -
; Modifie : AF, BC, DE, HL, IYH, IX
;--------------------------------------------------------
TXT_DRAW

    CALL TXT_SCREEN_ADDRESSE    ; HL = adresse écran (X, Y)
    
    LD   A, (IX + 2)
    LD   IYH, A                 ; masque couleur → IYH
    LD   A, (IX + 3)            ; premier caractère → A

;--------------------------------------------------------
; TXT_DRAW_LOOP
; Affiche une chaîne de caractères à l'écran
;
; Entrée  : IX    = pointeur sur les données caractère
;           HL    = adresse écran
;            A    = 1er caractere
;           (IX+4)= caractère à afficher (0 = fin de chaîne)
; Sortie  : -
; Modifie : AF, BC, DE, HL, IX
;--------------------------------------------------------
TXT_DRAW_LOOP

    PUSH HL
    CALL TXT_DRAW_LETTER        ; affiche le caractère courant
    POP  HL

    LD   A, (IX + 4)            ; lit le prochain caractère
    AND  A                      ; fin de chaîne ?
    RET  Z                      ; oui → retour

    INC  IX                     ; non → caractère suivant
    INC  L                      ; colonne +1
    INC  L                      ; colonne +1 (2 octets/car.)
    JR   TXT_DRAW_LOOP

;--------------------------------------------------------
; TXT_DRAW_LETTER
; Affiche un caractère à l'écran (mode CPC entrelacé)
;
; Entrée  : A   = caractère à afficher (ASCII)
;           HL  = adresse écran
;           IYH = masque de couleur
;           (FONT_TYPE) = adresse de la police (16 octets/car.)
; Sortie  : -
; Modifie : AF, BC, DE, HL
;--------------------------------------------------------
TXT_DRAW_LETTER

    EX   DE, HL                 ; DE = adresse écran

    LD   BC, (FONT_TYPE)        ; BC = base police
    
    SUB  ' '                    ; A  = index caractère (0-based)
    LD   H, 0
    LD   L, A
   
    ADD  HL, HL                 ; x2
    ADD  HL, HL                 ; x4
    ADD  HL, HL                 ; x8
    ADD  HL, HL                 ; x16 = offset dans la police
    ADD  HL, BC                 ; HL = adresse données caractère

    EX   DE, HL                 ; HL = adresse écran, DE = données caractère

REPEAT 8        	            ; 8 lignes de pixels
  
    LD   A, (DE)                ; octet haut (plan 0)
    AND  IYH                    ; applique masque couleur
    LD   B, A
    LD   (HL), B                ; écrit plan 0
    
    LD   C, H                   ; sauvegarde H
    
    LD   A, H
    XOR  #40
    LD   H, A
    
    LD   (HL), B
    
    INC  L
    INC  DE

    LD   A, (DE)                ; octet bas (plan 0)
    AND  IYH    
    LD   (HL), A                ; écrit plan 1 + 1
    
    LD   H, C                   ; restaure H
    LD   (HL), A                ; écrit plan 0 + 1
    
    INC  DE
    DEC  L

    mSET_NEXTLINE (VOID)        ; ligne pixel suivante

REND

    RET

;--------------------------------------------------------
; TXT_SCREEN_ADDRESSE
; Calcule l'adresse mémoire d'une cellule texte (X, Y)
;
; Entrée  : (IX+0) = colonne X
;           (IX+1) = ligne   Y
; Sortie  : HL     = adresse écran
; Modifie : DE, HL, F
;--------------------------------------------------------
TXT_SCREEN_ADDRESSE
    
    LD  C,  (IX + 0)            ; BC = COORDONNEE X
    LD  B,  0                   

    LD  L,  (IX + 1)            ; HL = COORDONNEE Y
    LD  H,  0    

    LD  DE, (SCR_ADDR_TABLE)
    ADD HL, HL                  ; HL = LIGNE * 2
    ADD HL, DE                  ; HL = SCR_ADDR_TABLE[LIGNE]

    LD  A,(HL) : INC HL          
    LD  H,(HL) : LD L,A

    ADD HL, BC           		; HL = ADRESSE FINALE

    RET

FONT_TYPE
    DEFW DA_FONT_MODE0
    
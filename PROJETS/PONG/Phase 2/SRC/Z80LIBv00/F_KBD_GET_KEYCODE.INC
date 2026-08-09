
; --------------------------------------------------------
; ROUTINE : F_KBD_GET_KEYCODE
; Rôle    : Teste l'état d'une touche du clavier.
;
; Entrée  : A = numéro du code de la touche (0 à 79)
;
; Sortie  : Z = 1 si la touche est appuyée
;           Z = 0 si la touche est relâchée
;
; Détruit : AF
;           HL préservé
;
; Dépend  : KBD_BUFFER_RAM
; --------------------------------------------------------

F_KBD_GET_KEYCODE

    PUSH HL

    LD   H,A

    ; Calcul de l'octet dans KBD_BUFFER_RAM
    RRCA
    RRCA
    RRCA
    AND  #0F

    ADD  KBD_BUFFER_RAM % 256
    LD   L,A

    ; Calcul du bit à tester
    LD   A,H
    LD   H,KBD_BUFFER_RAM / 256
    AND  #07

    ; Construction opcode BIT n,(HL)
    ADD  A
    ADD  A
    ADD  A
    ADD  #46

    ; Modification uniquement du 2e octet de BIT
    LD   (KEYTEST_BIT_OPCODE),A

    DEFB   #CB                       ; préfixe BIT
KEYTEST_BIT_OPCODE
    DEFB   #46                       ; Devient BIT n,(HL)

    POP  HL
    RET

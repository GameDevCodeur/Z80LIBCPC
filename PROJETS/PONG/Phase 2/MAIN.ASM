
    ; =============================================================================
    ; CONFIGURATION & FICHE TECHNIQUE : PROJET PONG CPC
    ; =============================================================================
    ; Machine cible    : Amstrad CPC 464 / 664 / 6128
    ; Microprocesseur  : Zilog Z80A @ 4 MHz
    ; Mémoire RAM      : 64 Ko                  (Zone d'exécution : #4000 - #7FFF)
    ; Contrôleur VDC   : CRTC Type 0            (Compatible Types 1, 2, 3, 4)
    ; Mode Graphique   : MODE 0                 (128x192 - 16 Couleurs)
    ; Adresse mémoire  : #4000 
    ; Point d'entrée   : #4000 
    ; Assembleur       : RASM                   (Roudoudou Assembler) v3.2.5
    ; Auteur(s)        : DevZ80CPC
    ; Moteur           : Z80LIB v1.0.0
    ; Version          : v0.1.0
    ; =============================================================================
    ; DESCRIPTION      : Sorti à la fin de l'année 1972 par Atari.
    ;                    Conçu par l'ingénieur Allan Alcorn.                  
    ; =============================================================================

    ; =============================
    ; DIRECTIVES DE RASM
    ; =============================  
    INCLUDE "SRC/RASM/DIRECTIVES.INC"            ; Directives : modifiables   

    ; =============================
    ; CONSTANTES
    ; =============================  
    INCLUDE "SRC/Z80LIB_CONST.INC"               ; Paramètres : modifiables    
    INCLUDE "SRC/GAME_CONST.INC"                 ; Paramètres : modifiables

    ; =============================
    ; Moteur Z80LIB
    ; =============================       
    INCLUDE "SRC/Z80LIBv00/BOOT.INC"             ; /\ NOT modifiable
    
    ; =============================
    ; ROUTINES GAMES
    ; =============================  
    INCLUDE "SRC/GAME_ROUTINES.INC"              ; Routines   : modifiables   

    ; =============================
    ; DATA SCENES GAME
    ; =============================    
    INCLUDE "SRC/DATA/SCENES_IM1.INC"            ; Données    : modifiables    
    INCLUDE "SRC/DATA/SCENES.INC"                ; Données    : modifiables
    
    ; =============================
    ; DATA GAME
    ; =============================
    INCLUDE "SRC/DATA/GAME.INC"                  ; Données    : modifiables
    
    ; =============================
    ; DEBUG INFORMATIONS
    ; =============================  
    FIN_CODE = $
   
    print ">> Adresse de debut   :", {hex}DEBUT_CODE
    print ">> Adresse de fin     :", {hex}FIN_CODE
    print ">> Taille             :", {hex}(FIN_CODE-DEBUT_CODE)
    print ">> Octets disponibles :", {hex}(#7FFF-FIN_CODE)

    assert $ <= #7FFF

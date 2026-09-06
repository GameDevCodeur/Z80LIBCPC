
F_Z80LIB_LANCER

    ; ===========================================
    ;  SET PILE Z80LIB
    ; ===========================================  
    DI                            ; 01o 04c   - Deactiver IM1
    LD   SP, SYS_STACK            ; 03o 10c

    ; ===========================================
    ; Charger SCENE 0
    ; ===========================================
    LD   B, 0
    CALL F_SCENE_CHARGER          ; 42o

    ; ===========================================
    ; Lancer interruption IM1
    ; ===========================================
    CALL F_IM1_START              ; 03o 17c

    ; ===========================================
    ; Drapeau synchronisation 50Hz
    ; ===========================================
    LD HL, frameReady             ; 03o 10c
    
    ; ==========================================
    ; BOUCLE INFINIE
    ; ==========================================

.gameLoop
    LD   A, (HL)                  ; 01o 07c   - Charge la valeur du drapeau
    AND  A                        ; 01o 04c   - Teste si drapeau est égal à 0
    JR   NZ, .gameLoop            ; 02o 12/7c - Si A!0, on boucle en attendant

SCENE_ACTUEL = $+1
    CALL #0000                    ; 03o 17c   - Scene à excuser 
    LD   HL, frameReady           ; 03o 10c
    INC  (HL)                     ; 01o 11c   - Remet le drapeau à 1 (bloquant)
    JR   .gameLoop                ; 02o 12c 

    ; ============================================================
    ; FUNCTIONS Z80LIB
    ; ============================================================
    INCLUDE "F_SCENE_CHARGER.INC" ; 42o
    INCLUDE "IM1/IM1.INC"         ; 62o

    ; ============================================================
    ; Z80LIB DATA SYSTEM VARIABLES
    ; ============================================================

SYS_DATA

    ; ============================================================
    ; FRAME_READY : Flag de synchronisation avec l'interruption
    ; Valeurs     : 0 - Prêt, 1 - En attente interruption
    ; ============================================================

    frameReady DEFB %00000001   ; 01o

    ; ============================================================
    ; RESERVATION DE LA PILE SYSTEME (STACK)
    ; ============================================================

    DEFS SP_SIZE                  ; 64o par defaut 
    SYS_STACK 


 
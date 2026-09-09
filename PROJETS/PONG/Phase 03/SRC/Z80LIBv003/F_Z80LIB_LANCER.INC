
F_Z80LIB_LANCER

    ; ===========================================
    ;  SET PILE Z80LIB
    ; ===========================================  
    DI                            ; 01o 04c   - Deactiver IM1
    LD   SP, SYS_STACK            ; 03o 10c

    ; ===========================================
    ;  SET CRTC Z80LIB
    ; =========================================== 
    mPPI_WAIT_VBL_EDGE (VOID)
    LD   C, 13 : LD HL, crtcTable    
    mCRTC_INIT_REGISTERS (VOID)

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
    INCLUDE "F_SCENE_CHARGER.INC"    ; 42o
    INCLUDE "IM1/IM1.INC"            ; 62o
    INCLUDE "Z80LIB.INC"

    ; ============================================================
    ; Z80LIB DATA SYSTEM VARIABLES
    ; ============================================================

SYS_DATA

    ; ============================================================
    ; TABLE DES VALEURS DES REGISTRES CRTC INVERSE
    ; ============================================================

    crtcTable

    DEFB  CRTC_R13              ; R13 - Adresse de départ écran poids faible
    DEFB  CRTC_R12              ; R12 - Adresse de départ écran poids fort (#30 -> #C000)
    DEFB  0                     ; R11 - Ligne de fin du curseur
    DEFB  0                     ; R10 - Ligne de début du curseur (0 = caché)
    DEFB  7                     ; R9  - Hauteur d'un caractère (7 = 8 lignes de pixels)
    DEFB  0                     ; R8  - Mode de balayage (0 = Pas d'entrelacement)
    DEFB  30                    ; R7  - Position de la synchronisation verticale
    DEFB  CRTC_R6               ; R6  - Hauteur visible (X lignes de caractères)
    DEFB  #00                   ; R5  - Ajustement vertical fin
    DEFB  38                    ; R4  - Hauteur totale de la trame
    DEFB  #86                   ; R3  - Largeur des pulses de synchro (H=14, V=9)
    DEFB  CRTC_R2               ; R2  - Position de la synchronisation horizontale
    DEFB  CRTC_R1               ; R1  - Largeur visible (X caractères)
    DEFB  #3F                   ; R0  - Largeur totale de la ligne

    ; R14/R15 curseur    — non utilisés
    ; R16/R17 light pen  — lecture seule, non configurables

    ; ============================================================
    ; FRAMEREADY  : Flag de synchronisation avec l'interruption
    ; Valeurs     : 0 - Prêt, 1 - En attente interruption
    ; ============================================================

    frameReady 

    DEFB %00000001   ; 01o

    ; ============================================================
    ; RESERVATION DE LA PILE SYSTEME (STACK)
    ; ============================================================

    DEFS SP_SIZE                ; 64o par defaut 
    SYS_STACK 


 
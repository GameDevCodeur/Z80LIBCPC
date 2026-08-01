
    ; ===========================================
    ;  MACROS DE LA Z80LIB v100 OBLIGATOIRES
    ; ===========================================
    INCLUDE "SYS/SYS.INC"

    ; ===========================================
    ;  FONCTIONS DE LA Z80LIB v100 OBLIGATOIRES
    ; ===========================================
    INCLUDE "F_CRTC_INITIALISER.INC"    ; 19 octets
    INCLUDE "F_ATTENTE_VBL.INC"         ; 08 octets
    INCLUDE "F_ETEINDRE_ENCRES.INC"     ; 14 octets
    INCLUDE "F_SCENE_CHARGER.INC"       ; 42 octets 
    INCLUDE "F_IM1.INC"                 ; 62 octets

F_Z80LIB_LANCER
    CALL F_ETEINDRE_ENCRES      ; Mettre les encres en noire
    CALL F_ATTENTE_VBL          ; Attendre synchronisation verticale
    CALL F_CRTC_INITIALISER     ; Configurer le CRTC
    CALL F_IM1_START            ; Lancer interruption IM1
    LD   B, 0
    CALL F_SCENE_CHARGER
    LD HL, FRAME_READY          ; Drapeau synchronisation 50Hz
.boucle_principale
    LD   A, (HL)                ; Charge la valeur du drapeau
    AND  A                      ; Teste si drapeau est égal à 0
    JR   NZ, .boucle_principale ; Si ce n'est pas 0, on boucle en attendant
    SCENE_ACTUEL: CALL #0000
    LD   HL, FRAME_READY
    INC  (HL)                   ; On remet le drapeau à 1 (bloquant)
    JR   F_Z80LIB_LANCER.boucle_principale ; On repart pour un tour de boucle

;=============================================================================
; FRAME_READY : Flag de synchronisation avec l'interruption
; Valeurs     : 0 - Prêt, 1 - En attente interruption
;=============================================================================
FRAME_READY   : DEFB %00000001

; SIZE: 33 + 145 = 178 octets

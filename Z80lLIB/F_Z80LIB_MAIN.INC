; =============================================================================
; CONFIGURATION MINIMALE REQUISE
; =============================================================================
; CPC             : 6128 
; RAM             : 64K + 64K EXT
; CRTC            : TYPE 0 6845SP CPC6128 sortis entre 85 et 87
; Microprocesseur : Z80A 4 MHz
; Format          : RASM v3.2.5 - Atlas
; =============================================================================
     
    ; -----------------------------------------
    ; RESERVATION DE LA PILE SYSTEME (STACK)
    ; -----------------------------------------
    STACK_BOTTOM
        DS SP_SIZE, #00                 ; Réservation de XX octets pour la pile
    STACK_TOP        
    
    DI                      ; 1o
    LD   SP, STACK_TOP      ; 3o
    CALL F_Z80LIB_LANCER    ; 2o

; SIZE : 100 + 244 = 344 octets

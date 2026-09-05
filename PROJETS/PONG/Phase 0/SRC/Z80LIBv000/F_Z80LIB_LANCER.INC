
 F_Z80LIB_LANCER

    ; ===========================================
    ;  PILE Z80LIB
    ; ===========================================  
    DI                      ; 1o 04c
    LD   SP, SYS_STACK      ; 3o 10c
    
    ; ==========================================
    ; BOUCLE INFINIE
    ; ==========================================
    JR   $                  ; 2o 12c 

 RET                        ; 1o 10c

SYS_DATA

    ; =========================================
    ; RESERVATION DE LA PILE SYSTEME (STACK)
    ; =========================================
    DEFS  SP_SIZE           ; 64o octets par defaut 

SYS_STACK 

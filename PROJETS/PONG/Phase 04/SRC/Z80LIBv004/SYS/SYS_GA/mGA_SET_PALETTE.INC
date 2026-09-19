;============================================================
                      MACRO mGA_SET_PALETTE
;------------------------------------------------------------
;  Initialise les couleurs des stylos                 AFBCHL!
;------------------------------------------------------------
; Entrées : 
;     HL=HWPALETTE
;     BC=#7F00
;      A=NBCOULEURS
;------------------------------------------------------------  

      OUT (C), C                 ; 2o - 12c  
      INC  B                     ; 1o - 04c
      OUTI                       ; 2o - 16c B-- PORTB=(HL) HL++
      INC  C                     ; 1o - 04c
      DEC  A                     ; 1o - 04c
      JR   NZ, $-7               ; 2o - 12c-07c

  MEND                           ; 9o - 52c-47c

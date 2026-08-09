

; --------------------------------------------------------
; ROUTINE : F_FLASH_ENTITY
; TAILLE  : 28 octets
; --------------------------------------------------------
; Rôle :
;   Gère le clignotement d'un stylo/couleur.
;
;   Lorsque Counter atteint 0 :
;     - recharge Counter avec CounterValue
;     - inverse State (0 <-> 1)
;     - sélectionne le Pen
;     - sélectionne ColorA ou ColorB selon State
;
; FORMAT STRUCTURE :
;
;   +0 : Counter
;   +1 : CounterValue
;   +2 : State
;   +3 : Pen
;   +4 : ColorA
;   +5 : ColorB
;
; ENTREES :
;   HL = adresse de Counter
;
; SORTIES :
;   Counter décrémenté/rechargé
;   State inversé lorsque Counter atteint 0
;   Pen et couleur envoyés au Gate Array
;
; DETRUIT :
;   AF, B
;   HL = HL + 5 lorsque le flash est exécuté
;
; PRESERVE :
;   DE
;
; CONVENTION GATE ARRAY :
;   Port Gate Array = #7Fxx
;
; --------------------------------------------------------

F_FLASH_ENTITY

    DEC  (HL)                    ; Counter--
    RET  NZ                      ; pas encore arrivé à 0

    ; ----------------------------------------------------
    ; Recharge du compteur
    ; ----------------------------------------------------

    INC  HL
    LD   A,(HL)                  ; A = CounterValue
    DEC  HL
    LD   (HL),A                  ; Counter = CounterValue

    ; ----------------------------------------------------
    ; Toggle State
    ; ----------------------------------------------------

    INC  HL
    INC  HL                      ; HL = State

    LD   A,(HL)
    XOR  1                       ; 0 <-> 1
    LD   (HL),A

    RRA                          ; Carry = nouvel état

    ; ----------------------------------------------------
    ; Sélection du Pen Gate Array
    ; ----------------------------------------------------

    INC  HL                      ; HL = Pen

    LD   B, HI(GA_PORT)
    LD   A,(HL)                  ; C = Pen
    OUT  (C),A                   ; sélection du stylo

    ; ----------------------------------------------------
    ; Sélection de la couleur
    ; ----------------------------------------------------

    INC  HL                      ; HL = ColorA

    LD   A,(HL)                  ; ColorA
    JR   NC,.WRITE               ; State = 0

    INC  HL                      ; HL = ColorB
    LD   A,(HL)                  ; ColorB

.WRITE
    OUT  (C),A                   ; couleur Gate Array
    
    RET

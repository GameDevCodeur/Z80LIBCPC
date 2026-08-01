
;==============================================================
; MACRO KILL_SYSTEM
;--------------------------------------------------------------
; Description : Désactive les interruptions système du CPC
;               en écrasant le vecteur IM1 (#38) avec un RET.
;               #C9 = opcode RET, #FB = opcode EI
;               Après EI, le système ne fait plus rien.
;--------------------------------------------------------------
; Modifie  : HL, (#38)
;==============================================================
MACRO mKILL_SYSTEM   
    LD  HL, #C9FB           ; H=#C9 (RET), L=#FB (EI)
    LD  (#38), HL           ; Ecrase le vecteur INT mode 1
ENDM

;; Hello world
;;
;; Execute using BASIC "CALL &1000" after compilation.

txt_output   equ &bb5a   ;; firmware function to display char on screen

org &1000

ld hl,string
call display_string
ret

string: defb "Hello world",0

;;-----------------------------------------------
;; display 0 terminated string
;; HL = start address of string
display_string:
ld a,(hl)
inc hl
or a
ret z
call txt_output
jr display_string

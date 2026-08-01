;
; RLE Decruncher (v1)
; ---by AsT/iMPACT
; hl=source | de=destination
; structure -> data,flux
; quand 00,00 fin du fichier
;
   org #a100
;
   ld hl,#4000
   ld de,#C000
;
decrunch
   ld c,(hl) ; 1er octet
   inc hl
   ld b,(hl) ; 2Šme octet
   inc hl

   ld a,c ; on teste si les 2 octets sont … z‚ro
   or b
   ret z ; si 00,00 alors fin
;
   ld a,c ; on r‚cupŠre la valeur du 1er octet
;
flux
   ld (de),a ; on le poke b fois
   inc de
   djnz flux
   jr decrunch
;
;
; RLE Cruncher (v2)
; ---by AsT/iMPACT
; hl=source | de=destination | ix=lenght
; structure -> data,flag,flux
;
flag equ #36
;
crunch
   ld hl,#c000
   ld de,#4000
   ld ix,#4000
;
   ld b,0 ; compteur de flux
loop_rle
   ld a,(hl) ; prends 1er octet
   ld (de),a ; octet
   inc de
nxt_byte
   ld i,a ; i=a=1er octet
   ld a,ixl ; si long du fichier=0
   or ixh
   ret z ; alors plus rien a compacter
;
   dec ix
   inc hl
   inc b ; b=b+1
   ld a,b ; si b=0 alors on a 256-1 interation du motif (1er Octet)
   or a
   jr z,ba0
;
   ld a,i ; recupäre valeur de a (1er octet)
;
   ld c,(hl) ; octet 2
   cp c ; compare octet 1 & 2
   jr z,nxt_byte
;
   ld a,b
   dec a
   jr z,pokeone
;
pokeflux
;
   ld a,(flag)
   ld (de),a
   inc de
;
pokebyte
;
   ld a,b
   ld (de),a
   inc de
   jr pokeone
;
; Un octet de trop
;
ba0    dec b ; pour garder le flux
   dec hl
   jr pokeflux
;
pokeone   ld b,0
   jr loop_rle
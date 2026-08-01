;
; RLE Cruncher (v1)
; ---by AsT/iMPACT
; hl=source | de=destination | ix=lenght
; structure -> data,flux
;
   org #a000
;
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
   ld i,a
   ld a,ixl ; si long du fichier=0
   or ixh
   ret z ; alors plus rien a compacter
;
   dec ix
   inc hl
   inc b ; b=b+1
   ld a,b ; si B=0 alors on a 256-1 interation du motif (1er Octet)
   or a ;
   jr z,ba0
;
   ld a,i ;on sauve le 1er octet dans le reg I
;
   ld c,(hl) ; octet 2
   cp c ; compare octet 1 & 2
   jr z,nxt_byte ; si les 2 octets sont ‚gaux alors on mets en place le compteur d'it‚rations
;
pokeflux
;
   ld a,b ; recupŠre le compteur et
   ld (de),a ; poke-le aprŠs l'octet.
   inc de
   ld b,0 ; raz compteur de flux
   jr loop_rle
;
; Un octet de trop
;
ba0    dec b ; pour garder le flux (Seulement quand b=0, b=255 it‚rations)
   dec hl ; on revient un octet avant
   jr pokeflux
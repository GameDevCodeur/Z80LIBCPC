;
; RLE Decruncher (v2)
; ---by AsT/iMPACT
; hl=source | de=destination
; structure -> data,flag,flux
; quand 00,00 fin du fichier
;
   org #a100
;
flag    equ #36
;
   ld hl,#4000 ; startadr
   ld de,#C000 ; destination
   ld a,(flag) ; le flag a ‚t‚ d‚termin‚ par la routine recherche de flag
   ld ixl,a
;
decrunch
   ld c,(hl) ;octet 1
   inc hl
   ld b,(hl) ; flag ou octet 2
   inc hl

   ld a,c ;Est ce que l'octet 1 est l'octet 2 sont … z‚ro ?
   or b ;
   ret z ; si 00,00 alors fin
;
   ld a,b ; est ce que l'octet 2 est le flag ?
   cp ixl ; #fd
   jr nz,noflux ; s'il n'y a pas de r‚p‚tition on poke seulement l'octet (sans r‚p‚tition)
;

   ld b,(hl) ; flux
   inc hl
;
   ld a,c ; on r‚cupŠre l'octet … r‚p‚ter
;
flux
   ld (de),a ;on poke en m‚moire
   inc de
   djnz flux ;on boucle x fois
;
   jr decrunch
;
; Pas de r‚p‚tion ? On poke juste l'octet
;
noflux
   ld a,c ; recupŠre l'octet en cours (1er octet)
   ld(de),a ;on poke
   inc de
    dec hl ; on revient 1 octet avant
;
   jr decrunch
;
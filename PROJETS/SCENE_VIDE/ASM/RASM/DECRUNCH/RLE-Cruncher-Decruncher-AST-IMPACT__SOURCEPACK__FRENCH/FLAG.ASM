;
; Recherche de Flag
; -- By Ast/iMPACT
; HL=Source | BC=Longueur
;
   org #A200

   ld e,0 ; flag de d‚part (0-255)
;
stream
   ld hl,#c000 ; adresse de d‚part
   ld bc,#4000 ;longueur du fichier … compresser
;
bouc
   ld a,(hl) ; Lis octet
   cp e      ; on compare l'octet au flag contenu dans le Reg E
   jr z,found ; l'octet est le mˆme que le flag, on passe au flag suivant (e=e-1)
   inc hl
   dec bc
   ld a,b
   or c
   jr nz,bouc ; on boucle tant que l'octet et le flag ne sont pas ‚gaux durant la longueur d‚termin‚e par BC
diff   ld a,e ; E=octet n'existant pas dans le fichier … compresser. A=E
        ld (#36),a ; adresse o— sera pok‚ ce flag
   ret

found
   dec e ; si l'octet est le mˆme que le flag, on change la valeur du flag
   jr nz,stream ; et on retourne au d‚but rechercher un nouveau flag
   ret
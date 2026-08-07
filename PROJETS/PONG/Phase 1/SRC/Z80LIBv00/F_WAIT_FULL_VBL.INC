
;-----------------------------------------------------------
; F_WAIT_FULL_VBL
;
; Attend un front montant propre du signal VSYNC (VBL) via
; le port PPI B. Attend d'abord que VSYNC retombe à 0 (au
; cas où il serait déjà actif à l'entrée), puis attend qu'il
; repasse à 1 — garantit un timing stable d'un appel à
; l'autre, contrairement à une boucle d'attente simple qui
; pourrait rater la transition si elle est déjà en cours.
;
; Entrée : -
; Sortie : -
; Détruit: AF, B
;-----------------------------------------------------------
F_WAIT_FULL_VBL 
	LD B,#F5
	.NOVBL IN A,(C) : RRA : JR C,.NOVBL
	.VBL   IN A,(C) : RRA : JR NC,.VBL
RET

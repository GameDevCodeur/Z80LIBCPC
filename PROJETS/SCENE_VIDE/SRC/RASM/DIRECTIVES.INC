
;-----------------------------------------------------------
; Statistiques de build, garde-fou de taille et
; Création de la disquette
;
; Directives d'assembleur (Rasm), évaluées à la compilation :
; affiche l'adresse finale, la taille totale du code généré
; depuis DEBUT_CODE, et l'espace restant avant la limite.
; L'assert interrompt la compilation si cette limite
; est dépassée.
;
; Prérequis : DEBUT_CODE doit être défini en tout début du
; code assemblé (avant toute donnée/table), sous peine de
; fausser le calcul de taille.
;-----------------------------------------------------------

	RELEASE_SNAP   = 0
	RELEASE_DSK    = 1
	RELEASE_TAPE   = 0
	RELEASE_AMSDOS = 0
	RELEASE_BINARY = 0

    ;IF RELEASE_SNAP
    ;BUILDSNA
    ;	BANKSET 0
    ;ENDIF

	IF RELEASE_DSK
		SAVE "VIDE.BIN", DEBUT_CODE, FIN_CODE-DEBUT_CODE, DSK, "DSK/VIDE.DSK"
	ENDIF

	IF RELEASE_TAPE
		SAVE "VIDE.BIN", DEBUT_CODE, FIN_CODE-DEBUT_CODE, TAPE, "VIDE.CDT"
	ENDIF

	IF RELEASE_AMSDOS
		SAVE "VIDE.BIN", DEBUT_CODE, FIN_CODE-DEBUT_CODE, AMSDOS
	ENDIF

	IF RELEASE_BINARY
		SAVE "VIDE.BIN", DEBUT_CODE, FIN_CODE-DEBUT_CODE
	ENDIF

	;IF RELEASE_SNAP
	;ELSE
	;	BANK
	;ENDIF

	ORG DEBUT_CODE
	RUN DEBUT_CODE

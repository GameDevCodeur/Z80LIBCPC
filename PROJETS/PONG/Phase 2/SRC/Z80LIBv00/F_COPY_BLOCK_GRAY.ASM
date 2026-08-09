
F_COPY_BLOCK_GRAY

	LDI : LDI : DEC DE : DEC DE   ; copie ligne 1
	SET 3, D                      ; -> adresse ligne 2
	LDI : LDI : DEC DE : DEC DE   ; copie ligne 2
	SET 4, D                      ; -> adresse ligne 3
	LDI : LDI : DEC DE : DEC DE   ; copie ligne 3
	RES 3, D                      ; -> adresse ligne 4
	LDI : LDI : DEC DE : DEC DE   ; copie ligne 4
	SET 5, D                      ; -> adresse ligne 5
	LDI : LDI : DEC DE : DEC DE   ; copie ligne 5
	SET 3, D                      ; -> adresse ligne 6
	LDI : LDI : DEC DE : DEC DE   ; copie ligne 6
	RES 4, D                      ; -> adresse ligne 7
	LDI : LDI : DEC DE : DEC DE   ; copie ligne 7
	RES 3, D                      ; -> adresse ligne 8
	LDI : LDI                     ; copie ligne 8

RET

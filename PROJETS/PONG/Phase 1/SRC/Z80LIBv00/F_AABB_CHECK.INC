
; --------------------------------------------------------
; ROUTINE : F_AABB_CHECK
; Rôle    : Teste le chevauchement de deux rectangles (AABB,
;           Axis-Aligned Bounding Box), en appelant
;           F_AABB_AXIS_OVERLAP sur chaque axe. Générique,
;           réutilisable pour toute paire d'entités (balle/
;           raquette, balle/mur, etc.).
; Entrée  : AABB1_X/W/Y/H, AABB2_X/W/Y/H (à remplir avant appel)
; Sortie  : Carry=1 si chevauchement, Carry=0 sinon
; Détruit : AF, B, DE, HL
; Dépend  : F_AABB_AXIS_OVERLAP
; --------------------------------------------------------
F_AABB_CHECK

    LD   HL, AABB1_X
    LD   DE, AABB2_X
    CALL F_AABB_AXIS_OVERLAP
    RET  NC

    LD   HL, AABB1_Y
    LD   DE, AABB2_Y
    CALL F_AABB_AXIS_OVERLAP
    RET  NC

    SCF
    RET

; --------------------------------------------------------
; ROUTINE : F_AABB_AXIS_OVERLAP
; Rôle    : Teste le chevauchement sur UN axe. Utilise une
;           inégalité LARGE (contact bord à bord = collision),
;           pas stricte, pour éviter de rater le tout premier
;           instant de contact entre deux rectangles.
; Entrée  : HL = &pos1 (pos1 en (HL), taille1 en (HL+1))
;           DE = &pos2 (pos2 en (DE), taille2 en (DE+1))
; Sortie  : Carry=1 si chevauchement (contact inclus), 0 sinon
; Détruit : AF, B
; --------------------------------------------------------
F_AABB_AXIS_OVERLAP
    LD   A, (DE)
    INC  DE
    LD   B, A
    LD   A, (DE)
    ADD  A, B
    DEC  DE
    LD   B, A
    LD   A, (HL)
    CP   B
    JR   NC, .checkStrict1     ; A >= B : soit égal (contact), soit dépassé
    JR   .cond1ok
.checkStrict1
    JR   Z, .cond1ok           ; A == B : contact exact, on accepte
    JR   .noOverlap            ; A > B : vraiment séparés

.cond1ok
    LD   A, (HL)
    INC  HL
    LD   B, A
    LD   A, (HL)
    ADD  A, B
    DEC  HL
    LD   B, A
    LD   A, (DE)
    CP   B
    JR   NC, .checkStrict2
    JR   .overlapConfirmed
.checkStrict2
    JR   Z, .overlapConfirmed
    JR   .noOverlap

.overlapConfirmed
    SCF
    RET
.noOverlap
    OR   A
    RET
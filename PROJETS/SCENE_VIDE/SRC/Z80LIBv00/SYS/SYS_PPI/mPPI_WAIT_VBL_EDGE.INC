

MACRO mPPI_WAIT_VBL_EDGE
    LD   A, HI(PPI_PORT_B)
@wait_low
    IN   A, (0)
    RRCA
    JR   C, @wait_low          ; attend que VSYNC retombe (fin d'un VBL en cours)
@wait_high
    IN   A, (0)
    RRCA
    JR   NC, @wait_high        ; attend le vrai flanc montant du prochain VBL
MEND

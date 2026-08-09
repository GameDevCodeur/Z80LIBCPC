
;============================================================================
; HL = source
; DE = destination
;----------------------------------------------------------------------------
MACRO mBLIT_LINE_M0_HL NB, AVANCE, TRANSPARENT

    IF {TRANSPARENT}

        REPEAT {NB}
            LD   A,(HL)
            INC  HL
            OR   A
            JR   Z,$+3
            LD   (DE),A
            INC  DE
        REND

    ELSE

        REPEAT {NB}
            LDI
        REND

    ENDIF

    IF {AVANCE}

        REPEAT {NB}
            DEC DE
        REND

        mSET_NEXTLINE_DE (VOID)

    ENDIF

MEND

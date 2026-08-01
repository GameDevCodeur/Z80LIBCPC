
;------------------------------------------------------------
; GATE ARRAY — CONSTANTES
;------------------------------------------------------------

GA_INKR             EQU %01000000       ; base commande encre
GA_PORT             EQU #7F00           ; port Gate Array

GA_PEN_0			EQU 0
GA_PEN_1			EQU 1
GA_PEN_2			EQU 2
GA_PEN_3			EQU 3
GA_PEN_4			EQU 4
GA_PEN_5			EQU 5
GA_PEN_6			EQU 6
GA_PEN_7			EQU 7
GA_PEN_8			EQU 8
GA_PEN_9			EQU 9
GA_PEN_10			EQU 10
GA_PEN_11			EQU 11
GA_PEN_12			EQU 12
GA_PEN_13			EQU 13
GA_PEN_14			EQU 14
GA_PEN_15			EQU 15

HW_BLACK            EQU GA_INKR+20      ;  0
HW_BLUE             EQU GA_INKR+04      ;  1
HW_BRIGHT_BLUE      EQU GA_INKR+21      ;  2
HW_RED              EQU GA_INKR+28      ;  3
HW_MAGENTA          EQU GA_INKR+24      ;  4
HW_MAUVE            EQU GA_INKR+29      ;  5
HW_BRIGHT_RED       EQU GA_INKR+12      ;  6
HW_PURPLE           EQU GA_INKR+05      ;  7
HW_BRIGHT_MAGENTA   EQU GA_INKR+13      ;  8
HW_GREEN            EQU GA_INKR+18      ;  9
HW_CYAN             EQU GA_INKR+06      ; 10
HW_SKY_BLUE         EQU GA_INKR+23      ; 11
HW_YELLOW           EQU GA_INKR+30      ; 12
HW_WHITE            EQU GA_INKR+00      ; 13
HW_PASTEL_BLUE      EQU GA_INKR+31      ; 14
HW_ORANGE           EQU GA_INKR+14      ; 15
HW_PINK             EQU GA_INKR+07      ; 16
HW_PASTEL_MAGENTA   EQU GA_INKR+15      ; 17
HW_BRIGHT_GREEN     EQU GA_INKR+18      ; 18
HW_SEA_GREEN        EQU GA_INKR+02      ; 19
HW_BRIGHT_CYAN      EQU GA_INKR+19      ; 20
HW_LIME_GREEN       EQU GA_INKR+26      ; 21
HW_PASTEL_GREEN     EQU GA_INKR+25      ; 22
HW_PASTEL_CYAN      EQU GA_INKR+27      ; 23
HW_BRIGHT_YELLOW    EQU GA_INKR+10      ; 24
HW_PASTEL_YELLOW    EQU GA_INKR+03      ; 25
HW_BRIGHT_WHITE     EQU GA_INKR+11      ; 26

; CMD   IR UR LR  MODE 
; %100  0  0  0   00
GA_MODE0_ROMS_ON  EQU %10000000
GA_MODE1_ROMS_ON  EQU %10000001 
GA_MODE2_ROMS_ON  EQU %10000010
GA_MODE3_ROMS_ON  EQU %10000011
GA_MODE0_ROMS_OFF EQU %10001100
GA_MODE1_ROMS_OFF EQU %10001101 
GA_MODE2_ROMS_OFF EQU %10001110
GA_MODE3_ROMS_OFF EQU %10001111

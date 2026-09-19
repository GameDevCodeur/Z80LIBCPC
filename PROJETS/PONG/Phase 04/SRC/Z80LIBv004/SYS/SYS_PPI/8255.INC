
;------------------------------------------------------------
; PPI 8255 — Interface Périphériques
;------------------------------------------------------------
PPI_PORT_A          EQU #F400           ; port A
PPI_PORT_B          EQU #F500           ; port B
PPI_PORT_C          EQU #F600           ; port C
PPI_CTRL            EQU #F700           ; port contrôle

PPI_C_KBD_ACCESS    EQU #40
KBD_BUFFER_RAM      EQU #0040           ; adresse buffer clavier

; ============================================================
; Row 0
; ============================================================
KEY_CURSOR_UP		EQU #00
KEY_CURSOR_RIGHT	EQU #01
KEY_CURSOR_DOWN		EQU #02
KEY_F9				EQU #03
KEY_F6				EQU #04
KEY_F3				EQU #05
KEY_ENTER_NUM		EQU #06		; NUMERIC KEYPAD ENTER
KEY_FDOT			EQU #07		; NUMERIC KEYPAD DOT

; ============================================================
; Row 1
; ============================================================
KEY_CURSOR_LEFT		EQU #10
KEY_COPY			EQU #11
KEY_F7				EQU #12
KEY_F8				EQU #13
KEY_F5				EQU #14
KEY_F1				EQU #15
KEY_F2				EQU #16
KEY_F0				EQU #17

; ============================================================
; Row 2
; ============================================================
KEY_CLR				EQU #20		; CLR / HOME
KEY_BRACKET_L		EQU #21
KEY_RETURN			EQU #22
KEY_BRACKET_R		EQU #23
KEY_DEL_FWD			EQU #24		; F4 / FORWARD DELETE ON SOME LAYOUTS
KEY_SHIFT			EQU #25
KEY_EQUAL			EQU #26
KEY_CTRL			EQU #27

; ============================================================
; Row 3
; ============================================================
KEY_CARET			EQU #30
KEY_MINUS			EQU #31
KEY_AT				EQU #32
KEY_P				EQU #33
KEY_SEMICOLON		EQU #34
KEY_COLON			EQU #35
KEY_SLASH			EQU #36
KEY_DOT				EQU #37

; ============================================================
; Row 4
; ============================================================
KEY_0				EQU #40
KEY_9				EQU #41
KEY_O				EQU #42
KEY_I				EQU #43
KEY_L				EQU #44
KEY_K				EQU #45
KEY_M				EQU #46
KEY_COMMA			EQU #47

; ============================================================
; Row 5
; ============================================================
KEY_8				EQU #50
KEY_7				EQU #51
KEY_U				EQU #52
KEY_Y				EQU #53
KEY_H				EQU #54
KEY_J				EQU #55
KEY_N				EQU #56
KEY_SPACE			EQU #57

; ============================================================
; Row 6
; ============================================================
KEY_6				EQU #60
KEY_5				EQU #61
KEY_R				EQU #62
KEY_T				EQU #63
KEY_G				EQU #64
KEY_F				EQU #65
KEY_B				EQU #66
KEY_V				EQU #67

; ============================================================
; Row 7
; ============================================================
KEY_4				EQU #70
KEY_3				EQU #71
KEY_E				EQU #72
KEY_W				EQU #73
KEY_S				EQU #74
KEY_D				EQU #75
KEY_C				EQU #76
KEY_X				EQU #77

; ============================================================
; Row 8
; ============================================================
KEY_1				EQU #80
KEY_2				EQU #81
KEY_ESCAPE			EQU #82
KEY_Q				EQU #83
KEY_TAB				EQU #84
KEY_A				EQU #85
KEY_CAPS_LOCK		EQU #86
KEY_Z				EQU #87

; ============================================================
; Row 9
; ============================================================
KEY_DEL				EQU #97
KEY_JOY1_FIRE3		EQU #96
KEY_JOY1_FIRE2		EQU #95
KEY_JOY1_FIRE1		EQU #94
KEY_JOY1_RIGHT		EQU #93
KEY_JOY1_LEFT		EQU #92
KEY_JOY1_DOWN		EQU #91
KEY_JOY1_UP			EQU #90
KEY_JOY2_FIRE		EQU KEY_JOY1_FIRE3

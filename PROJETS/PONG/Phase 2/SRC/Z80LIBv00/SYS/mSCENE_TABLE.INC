; ================================================================
; MACRO : mSCENE_BEGIN
; Rôle  : Démarre la génération des trois tables de scènes.
;         Crée les labels publics SCENE_TABLE_FN, SCENE_TABLE_IM1,
;         SCENE_TABLE_INIT et initialise le compteur.
; ================================================================
MACRO mSCENE_BEGIN
    ALIGN 256
    SCENE_TABLE
    scene_count = 0
ENDM

; ================================================================
; MACRO : mSCENE_ADD
; Rôle  : Ajoute une entrée pour une scène dans les trois tables.
; Entrée : SCENE_NAME (ex: MENU, JEU, ...)
; Prérequis : Les labels suivants doivent être définis AVANT :
;             F_SCENE_<SCENE_NAME>
;             ARRAY_IM1_<SCENE_NAME>
;             F_SCENE_<SCENE_NAME>_INIT
; ================================================================
MACRO mSCENE_ADD SCENE_NAME
    ; Table des routines principales
    DW F_SCENE_{SCENE_NAME}

    ; Table des tableaux IM1
    DW ARRAY_IM1_{SCENE_NAME}

    ; Table des initialisations
    DW F_SCENE_{SCENE_NAME}_INITIALISATION

    scene_count = scene_count + 1
ENDM

; ================================================================
; MACRO : mSCENE_END
; Rôle  : Finalise la génération et définit SCENE_TABLE_COUNT.
; ================================================================
MACRO mSCENE_END
    SCENE_TABLE_COUNT EQU scene_count
    ; Après la définition de SCENE_TABLE
    IF ($ - SCENE_TABLE) >= 256
        ERROR "SCENE_TABLE dépasse 256 octets ! Utiliser une autre version."
    ENDIF
    ASSERT ($ - SCENE_TABLE) < 256
ENDM

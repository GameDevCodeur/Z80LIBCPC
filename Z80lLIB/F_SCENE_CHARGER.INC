
;==============================================================
; F_SCENE_CHARGER
;--------------------------------------------------------------
; Description : Aiguille vers l'initialisation de la scène
;               demandée. Invalide d'abord le vecteur IM1
;               courant (protège contre un dispatch incohérent
;               pendant la transition), accède directement à
;               l'entrée correspondante dans SCENE_TABLE via
;               l'identifiant comme index (ID_SCENE_xxx doivent
;               être séquentiels 0..SCENE_TABLE_COUNT-1), puis
;               charge le pointeur de scène, le vecteur IM1
;               associé, et saute vers le setup.
;--------------------------------------------------------------
; Entrées  : B - Identifiant du nouvel état (ID_SCENE_xxx,
;                séquentiel 0..SCENE_TABLE_COUNT-1)
; Sorties  : RET si identifiant hors limites (IM1_INDEX reste
;            alors invalidé -> le dispatch d'interruption
;            reste figé jusqu'au prochain changement de scène
;            valide ; comportement volontaire, sert de signal
;            de bug si un ID invalide est passé)
;            Sinon saut vers F_SCENE_xxx_INITIALISATION
; Modifie  : AF, DE, HL, (IM1_INDEX), (SCENE_ACTUEL+1),
;            (IM1_CURRENT)
; Table    : SCENE_TABLE, entrées de 6 octets (3 DW consécutifs) :
;            DW F_SCENE_xxx, ARRAY_IM1_xxx, F_SCENE_xxx_INIT
;==============================================================

F_SCENE_CHARGER

    LD   A, IM1_NOT_READY
    LD   (IM1_INDEX), A              ; Invalide le vecteur IM1
    LD   A, B                        ; A = ID_SCENE_xxx (0..N-1 attendu)
    CP   SCENE_TABLE_COUNT
    RET  NC                          ; ID hors limites -> inconnu, sortie

    ADD  A, A                        ; A = ID*2
    LD   E, A                        ; E = ID*2 (sauvegarde)
    ADD  A, A                        ; A = ID*4
    ADD  A, E                        ; A = ID*4 + ID*2 = ID*6
    LD   L, A
    LD   H, 0
    LD   DE, SCENE_TABLE
    ADD  HL, DE                       ; HL = SCENE_TABLE + ID*6

.scene_found
    LD   E, (HL) : INC HL
    LD   D, (HL) : INC HL            ; DE = adresse F_SCENE_xxx
    PUSH DE
    LD   E, (HL) : INC HL
    LD   D, (HL) : INC HL            ; DE = adresse ARRAY_IM1_xxx
    LD   (IM1_CURRENT), DE
    LD   E, (HL) : INC HL
    LD   D, (HL)                     ; DE = adresse F_SCENE_xxx_INITIALISATION
    POP  HL
    LD   (SCENE_ACTUEL+1), HL
    PUSH DE
    RET

; SIZE: 44 octets

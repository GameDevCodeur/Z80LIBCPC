
; --------------------------------------------------------
; ROUTINE : F_IM1_CALLBACK
; Rôle    : Point d'entrée de l'interruption IM1 (~300 Hz,
;           cadencée par le CRTC). Détecte le vrai retour
;           vertical (VBL) via le port PPI plutôt que de
;           compter aveuglément les interruptions, ce qui
;           corrige toute dérive du timing CRTC et cale le
;           jeu sur exactement 50 Hz. Si un VBL est détecté,
;           libère FRAME_READY (débloque BOUCLE_PRINCIPALE).
;           Sinon, avance IM1_INDEX et dispatche vers la
;           sous-tâche IM1 courante via la table IM1_CURRENT
;           (mise à jour par F_SCENE_CHARGER selon la scène
;           active).
; Entrée  : aucune (déclenchée par le matériel)
; Sortie  : FRAME_READY = 0 si VBL détecté
;           IM1_INDEX mis à jour (avancé ou réinitialisé)
;           saut final vers la sous-routine IM1 correspondante
;           (JP (HL)), ou vers F_IM1_SKIP si IM1_INDEX n'est
;           pas encore prêt (IM1_NOT_READY)
; Détruit : rien de visible pour l'appelant — sauvegarde et
;           restaure intégralement AF, BC, DE, HL, IX, IY
;           (registres principaux et alternatifs) avant RETI
; Dépend  : F_IM1_SKIP, IM1_CURRENT, IM1_INDEX, IM1_IT_MAX,
;           IM1_NOT_READY, FRAME_READY
; --------------------------------------------------------
F_IM1_CALLBACK
    ; SAUVEGARDE DU CONTEXTE
    EXX
    EX    AF, AF'
    PUSH  IX
    PUSH  IY

    ; DETECTER SYNCHRO VBL
    LD    A, HI(PPI_PORT_B)
    IN    A, (0)
    RRCA
    JR    NC, .NO_VBL

    ; DEBUT DE FRAME (Fréquence exacte de 50 Hz)
    LD    HL, FRAME_READY
    XOR   A
    LD    (HL), A
    JR    .DISPATCH

.NO_VBL
    LD    A, (IM1_INDEX)
    CP    IM1_NOT_READY
    JR    Z, F_IM1_SKIP          ; pas de VBL et pas encore prêt -> sortie

    INC   A
    CP    IM1_IT_MAX
    JR    C, .DISPATCH
    XOR   A

.DISPATCH
    LD    (IM1_INDEX), A
    ADD   A, A
    LD    C, A
    LD    B, 0
    LD    HL, (IM1_CURRENT)
    ADD   HL, BC
    LD    A, (HL)
    INC   HL
    LD    H, (HL)
    LD    L, A
    JP    (HL)

    
; ------------------------------------------------------------
; DONNÉES POUR LE SYSTÈME D'INTERRUPTIONS VECTORISÉES
; ------------------------------------------------------------
IM1_INDEX:
    DEFB 0                           ; Valeur initiale = -2 ou 0xFE
                                     ; Indique que l'index est "non prêt"
                                     ; Pas encore d'interruption à traiter
IM1_CURRENT:
    DEFW 0                           ; Pointe vers le tableau de vecteurs par défaut

; SIZE: 45 octets

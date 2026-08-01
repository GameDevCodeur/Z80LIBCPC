;============================================================
; mCRTC_SCR_SETUP
; Configure le CRTC à partir d'une table séquentielle
;
; Entrée  : HL = adresse table valeurs
;           C  = nombre de registres à configurer
; Sortie  : -
; Modifie : AF, BC, HL
;============================================================
MACRO mCRTC_SCR_SETUP

    LD   B, #BC                  ; B = 0xBC → CRTC_REG

@BC_CRTC
    
    ; 1. Sélection du registre du CRTC
    OUT  (C), C                  ; sélectionne registre C
    
    ; 2. Envoi de la valeur de configuration    
    LD   B, #BE                  ; /!\ On met #BE car OUTI décrémente B en premier !
    OUTI                         ; DEC B, envoie (HL) sur le port 0xBD → CRTC_DAT, INC HL 

    ; 3. Passage au registre suivant    
    DEC  B                       ; B = 0xBC → CRTC_REG
    DEC  C                       ; registre suivant
    JP   P, @BC_CRTC             ; boucle si C > 0

MEND

; size : 13o

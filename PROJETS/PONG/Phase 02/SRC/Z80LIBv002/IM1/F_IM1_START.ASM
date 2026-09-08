
;-----------------------------------------------------------
; F_IM1_START
;
; Installe la routine F_IM1_CALLBACK comme gestionnaire
; d'interruption, en patchant l'adresse cible du vecteur
; JP situé en #0038 (mode interruption IM1 du CPC).
; Suppose que l'opcode JP (#C3) est déjà présent en #0038
; (mis en place par le firmware au démarrage).
;
; Entrée : aucune
; Sortie : (#0039-#003A) = adresse complète (16 bits) de
;          F_IM1_CALLBACK
; Détruit: AF, HL
;-----------------------------------------------------------
F_IM1_START
    DI                              ; Désactive temporairement les interruptions
    LD   HL, F_IM1_CALLBACK         ; Charge l'adresse de notre routine maison
    LD   (#0039), HL                ; Écrit cette adresse juste après le "JP"
    EI                              ; Réactive globalement les interruptions
    RET
; Total : 9 octets.
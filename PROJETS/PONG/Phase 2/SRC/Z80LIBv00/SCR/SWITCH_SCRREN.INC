
; =============================================================================
; FONCTION    : switch_screens
; =============================================================================
; Description : Alterne les tampons d'affichage (Double Buffering) pour le CPC.
;               - Incrémente un compteur de frames (heartbeat).
;               - Modifie la couleur de la bordure pour le débogage/timing.
;               - Échange les adresses de l'écran visible et caché (0x80 / 0xC0).
;               - Configure les registres 12 et 13 du CRTC 6845.
;               - Redirige le pointeur de la table des lignes pour le dessin.
; Entrée      : Aucune
; Sortie      : Registres modifiés : A, BC, D, E, HL
; =============================================================================
switch_screens

    ; --- 1. GESTION DU TIMING (HEARTBEAT) ---
    ld a, (compteur_frames)         ; Charge la valeur du battement de cœur
    inc a                           ; Incrémente le compteur
    cp 50                           ; Vérifie si on a atteint 50 frames (1 seconde)
    jr nz, sauve_compteur           ; Si non égal, on passe à la sauvegarde
    xor a                           ; Si égal à 50, on réinitialise A à 0

sauve_compteur
    ld (compteur_frames), a         ; Sauvegarde le compteur en mémoire

    ; --- 2. INVERSION DU TAMPON MÉMOIRE (SWITCH_EM) ---
debut_bascule
    ld d, #57                       ; Code couleur Gate Array pour la bordure
    call change_bordure             ; Appelle la routine pour changer la couleur

    ld a, (adr_ecran_visible)       ; Récupère le poids fort de l'écran affiché
    ld (adr_ecran_cache), a         ; Cet écran devient le nouveau tampon caché
    ld e, a                         ; Sauvegarde cette adresse dans E pour plus tard

    xor #40                        ; Alterne rapidement l'adresse (0x80 <-> 0xC0)
    ld (adr_ecran_visible), a       ; Sauvegarde le nouveau poids fort visible

    ; --- 3. CONFIGURATION DU MATÉRIEL (CRTC) ---
    ; On divise l'adresse de la page par 4 (décalage de 2 bits vers la droite)
    ; car le CRTC gère l'adresse différemment de la mémoire CPU.
    srl a                  			         
    srl a                           

    ; Envoi au Registre 12 du CRTC (Adresse de départ - Poids fort)
    ld bc, #bc0c                   ; Sélection du registre 12 via le port 0xBCxx
    out (c), c                      ; Envoie l'index au CRTC
    ld b, #bd                      ; Bascule sur le port de données 0xBDxx
    out (c), a                      ; Envoie la valeur de la page (0x20 ou 0x30)

    ; Envoi au Registre 13 du CRTC (Adresse de départ - Poids faible)
    ld bc, #bc0d                   ; Sélection du registre 13 via le port 0xBCxx
    out (c), c                      ; Envoie l'index au CRTC
    ld b, #bd                      ; Bascule sur le port de données 0xBDxx
    out (c), 0                      ; Le poids faible de l'écran est mis à 0

    ; --- 4. REDIRECTION DE LA TABLE DE TRACÉ ---
    xor a                           
    ld (image_prete), a             ; Réinitialise le flag d'image prête à 0

    ld a, e                         ; Récupère l'adresse de l'écran caché (stockée dans E)
    cp #C0                          ; Est-ce que le tampon caché est en 0xC000 ?
    jr z, ecran_cache_est_c0        ; Si oui, on saute au traitement de la zone 0xC000

    ; Cas où l'écran caché est en 0x8000
    ld hl, table_lignes_80          ; Charge l'adresse de la table pour la zone 0x8000
    ld (pointeur_table_lignes), hl  ; Met à jour le pointeur global pour votre moteur
    ret                             ; Fin de la fonction, retour au programme principal

ecran_cache_est_c0
    ; Cas où l'écran caché est en 0xC000
    ld hl, table_lignes_c0          ; Charge l'adresse de la table pour la zone 0xC000
    ld (pointeur_table_lignes), hl  ; Met à jour le pointeur global pour votre 

    ret                             ; Fin de la fonction, retour au programme principal


; --------------------------------------------------------
; ROUTINE : F_SCENE_TITRE_INITIALISATION
; Rôle    : Initialise l'écran titre : configure le mode
;           graphique, décompresse le visuel en VRAM, puis
;           masque le pen 0 en noir.
;
; Entrée  : TITRE_ZX0_DATA = adresse des données compressées
;           TITRE_VRAM = adresse VRAM destination
;
; Sortie  : écran titre prêt à l'affichage
; Détruit : AFBCDEHL!
; Dépend  : mGA_SETMODE, F_ZX0_DECOMPRESS, mGA_SET_INK
; --------------------------------------------------------
F_SCENE_TITRE_INITIALISATION

    mGA_SETMODE GA_MODE_0|GA_UROM_DISABLE|GA_LROM_DISABLE

    ; ---- Décompression ZX0 en VRAM ----
    LD   HL, TITRE_ZX0_DATA
    LD   DE, TITRE_VRAM
    CALL F_ZX0_DECOMPRESS

    ; ---- Masque le pen 0 en noir ----
    mGA_SET_INK GA_PEN_0, GA_COLOR_BLACK

    RET

; --------------------------------------------------------
; ROUTINE : F_SCENE_TITRE
; Rôle    : Gère le décompte de l'écran titre. Tant que le
;           timer TITRE_TEMPO n'a pas atteint 0, ne fait rien.
;           À 0, éteint les encres et enchaîne sur la suite.
;
; Entrée  : TITRE_TEMPO = compteur de frames restantes
; Sortie  : (aucune valeur ; enchaîne sur F_ETEINDRE_ENCRES à 0)
; Détruit : AFBDHL!
; --------------------------------------------------------
F_SCENE_TITRE
  
    LD   HL, TITRE_TEMPO
    DEC  (HL) : RET  NZ

    JP   F_ETEINDRE_ENCRES ; AFBD!

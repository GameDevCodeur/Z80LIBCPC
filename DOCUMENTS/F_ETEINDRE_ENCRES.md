# 📘 Documentation technique : Routine `F_ETEINDRE_ENCRES`

**Auteur** : Patrick MAES - Analyse personnalisée pour développement Z80 / Amstrad CPC  
**Version** : 1.0  
**Date** : 2026-08-07  
**Assembleur** : RASM (ou compatible)  

---

## 1. Introduction

La routine **`F_ETEINDRE_ENCRES`** (Éteindre les encres) est une fonction utilitaire pour l’Amstrad CPC. Elle force la couleur **noire** sur l’intégralité de la palette graphique, c’est‑à‑dire :

- Les **16 encres logiques** (stylos 0 à 15).
- La **bordure** (stylo 16).

Cette opération est couramment utilisée pour **masquer l’écran** pendant des phases de chargement, des transitions entre écrans, ou pour éviter l’affichage de données parasites durant le temps de traitement d’une trame verticale (VBL).

La routine est très compacte (14 octets) et rapide, car elle utilise une boucle simple et la macro `mGA_SET_PEN_INK` pour écrire dans le Gate Array.

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `F_ETEINDRE_ENCRES` |
| **Paramètres** | Aucun |
| **Entrée** | Aucune |
| **Sortie** | Aucune (l’écran devient noir) |
| **Registres détruits** | `AF`, `B`, `D` |
| **Registres préservés** | `BC` (sauf `B`), `DE` (sauf `D`), `HL`, `IX`, `IY` (non utilisés) |
| **Taille du code** | 14 octets |
| **Dépendances** | `GA_PORT` (doit être défini, ex: `0x7F00`)<br>`HW_BLACK` (doit être défini)<br>Macro `mGA_SET_PEN_INK` (doit être définie) |
| **Compatibilité** | Amstrad CPC / Gate Array uniquement |

---

## 3. Fonctionnement détaillé

### 3.1. Logique de la boucle

La routine exploite le fait que les stylos vont de **0 à 16** (16 pour la bordure).  
Elle utilise le registre `A` comme compteur, initialisé à **17**, et décrémente `A` en début de boucle pour parcourir les valeurs **16, 15, ..., 0**.

```z80
LD   A, 17          ; A = 17
.B_ENCRES:
    DEC  A           ; A = 16 (bordure), puis 15...0
    mGA_SET_PEN_INK  ; Définit l'encre A avec la couleur D (noir)
    JR   NZ, .B_ENCRES ; Continue tant que A != 0
```

**Détail du déroulement :**

| Itération | Valeur initiale de A | `DEC A` | Valeur écrite dans GA | Signification |
| :--- | :--- | :--- | :--- | :--- |
| 1 | 17 | 16 | 16 | Bordure |
| 2 | 16 | 15 | 15 | Encre 15 |
| ... | ... | ... | ... | ... |
| 16 | 2 | 1 | 1 | Encre 1 |
| 17 | 1 | 0 | 0 | Encre 0 |
| 18 | 0 | -1 (0xFF) | (non exécuté) | La boucle s’arrête car Z=1 |

### 3.2. Écriture dans le Gate Array

La routine utilise la macro `mGA_SET_PEN_INK` qui s’attend à trouver :
- `A` = numéro de stylo (ici, décrémenté de 16 à 0).
- `D` = couleur matérielle (ici, `HW_BLACK`).
- `B` = octet haut du port GA (`HI(GA_PORT)`, généralement `0x7F`).

### 3.3. Code assembleur complet

```z80
F_ETEINDRE_ENCRES
    LD   A, 17                      ; 17 itérations (16 encres + bordure)
    LD   B, HI(GA_PORT)             ; B = 0x7F (octet haut du port GA)
    LD   D, HW_BLACK                ; D = (couleur noire)
.B_ENCRES:
    DEC  A                          ; Décrémente : 16 → 15 → ... → 0
    mGA_SET_PEN_INK                 ; Définit l'encre A avec la couleur D
    JR   NZ, .B_ENCRES              ; Boucle tant que A ≠ 0
    RET
```

---

## 4. Dépendances et configuration

Pour que cette routine fonctionne, plusieurs éléments doivent être définis **ailleurs** dans votre code :

| Symbole | Type | Exemple de définition | Rôle |
| :--- | :--- | :--- | :--- |
| `GA_PORT` | Constante | `GA_PORT EQU 0x7F00` | Adresse de base du port Gate Array. |
| `HW_BLACK` | Constante | `HW_BLACK` | Code matériel de la couleur noire. |
| `mGA_SET_PEN_INK` | Macro | Doit être définie comme vu précédemment | Effectue les deux `OUT (C),r` pour écrire dans le GA. |
| `C` | Registre | Doit être `0x00` avant l’appel | Octet bas du port GA. La routine ne le modifie pas, donc un appel préalable `LD C, 0` est nécessaire. |

**Exemple d’initialisation avant l’appel :**

```z80
LD   BC, GA_PORT   ; ou LD B, HI(GA_PORT) / LD C, LO(GA_PORT)
CALL F_ETEINDRE_ENCRES
```

---

## 5. Exemples d’utilisation

### 5.1. Masquer l’écran avant un chargement

```z80
; Initialisation du port GA
LD   BC, GA_PORT   ; BC = 0x7F00 (si GA_PORT est défini)
CALL F_ETEINDRE_ENCRES
; L'écran est maintenant noir.
; Effectuer le chargement des données...
```

### 5.2. Intégration dans une séquence VBL

```z80
WaitVBL:
    ; Attendre le début de la VBL...
    CALL F_ETEINDRE_ENCRES   ; Éteindre l'affichage
    ; Effectuer des modifications rapides de la palette
    ; ...
    CALL F_RESTAURER_ENCRES  ; Remettre les couleurs d'origine
```

---

## 6. Analyse de la taille (14 octets)

Découpage des instructions (en supposant un Z80 standard) :

| Instruction | Taille (octets) |
| :--- | :--- |
| `LD A, 17` | 2 |
| `LD B, HI(GA_PORT)` | 2 (si `HI()` est résolu en constant) |
| `LD D, HW_BLACK` | 2 |
| `.B_ENCRES:` `DEC A` | 1 |
| `mGA_SET_PEN_INK` (2 x `OUT (C),r`) | 4 (2 octets par OUT) |
| `JR NZ, .B_ENCRES` | 2 |
| `RET` | 1 |
| **Total** | **14** |

La routine est donc extrêmement économique en mémoire, ce qui en fait un choix idéal pour les jeux ou démos où chaque octet compte.

---

## 7. Précautions et bonnes pratiques

| Point d’attention | Recommandation |
| :--- | :--- |
| **Registres détruits** | La routine modifie `A`, `BC` et `D`. Si votre code appelant a besoin de ces registres, sauvegardez‑les (ex: `PUSH AF`, `PUSH BC`, `PUSH DE`). |
| **Compatibilité avec les ROMs** | Le Gate Array est accessible en lecture/écriture depuis le mode système. Aucune restriction particulière. |
| **Utilisation pendant la VBL** | Écrire dans le Gate Array n’est pas soumis à la contrainte de la VBL (contrairement à la CRTC), donc vous pouvez appeler cette routine à tout moment. |
| **Ordre des itérations** | L’ordre (bordure puis encres 15 à 0) est sans importance pour l’effet final, mais il est choisi pour optimiser le code (décrémentation). |

---

## 8. Optimisations possibles

La routine est déjà optimale en taille (14 octets). Cependant, si vous devez **éteindre les encres très fréquemment**, vous pouvez envisager :

- **Inliner le code** : si vous appelez la routine dans une boucle serrée, le `CALL`/`RET` peut être évité en copiant le code directement. Mais cela augmenterait la taille du code.
- **Pré‑charger `BC`** : si `BC` est déjà configuré pour le GA ailleurs, vous pouvez économiser les instructions `LD B, ...` en les externalisant.

---

## 9. Conclusion

La routine **`F_ETEINDRE_ENCRES`** est un outil fondamental pour le contrôle de l’affichage sur Amstrad CPC.  
Simple, fiable et ultra‑compacte (14 octets), elle s’intègre parfaitement dans tout projet nécessitant de masquer l’écran rapidement, que ce soit pour des chargements, des transitions ou des effets spéciaux.

Elle tire parti de la macro `mGA_SET_PEN_INK` pour une écriture efficace dans le Gate Array, et utilise une boucle astucieuse pour parcourir les 17 encres avec un minimum d’instructions.

---

## 10. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 1.0 | 2026-08-07 | Patrick MAES | Version initiale. Documentation complète du fonctionnement. |

---

*Références :*  
- Documentation technique de l’Amstrad CPC (schéma du Gate Array).  
- Macros associées : `mGA_SET_PEN_INK`, `mGA_SET_MODE`.

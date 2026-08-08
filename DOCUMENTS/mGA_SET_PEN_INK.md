
# 📘 Documentation technique : Macro `mGA_SET_PEN_INK`

**Auteur** : Analyse personnalisée pour développement Z80 / Amstrad CPC  
**Version** : 1.0  
**Date** : 2026-08-07  
**Assembleur** : RASM (ou compatible)  

---

## 1. Introduction

La macro **`mGA_SET_PEN_INK`** est une interface bas‑niveau pour le **Gate Array** de l’Amstrad CPC. Elle permet de définir la couleur d’un **stylo** (pen) – c’est‑à‑dire d’une des 16 encres logiques ou de la bordure – en écrivant la valeur matérielle correspondante dans le registre d’encre du Gate Array.

Cette macro est conçue pour être utilisée dans des programmes graphiques ou systèmes nécessitant un contrôle précis des couleurs, sans passer par le firmware. Elle est extrêmement compacte (4 octets) et rapide, car elle utilise deux instructions `OUT (C),reg` consécutives, profitant du fait que le port `C` reste constant entre les deux écritures.

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `mGA_SET_PEN_INK` |
| **Paramètres** | Aucun (macro sans paramètre) |
| **Entrées** | `A` = numéro de stylo (0‑15 pour les encres, 16 pour la bordure)<br>`D` = couleur matérielle.<br>`B` = octet haut du port Gate Array (généralement `0x7F`) |
| **Sortie** | Aucune |
| **Registres détruits** | Aucun (les registres sont lus mais pas modifiés) |
| **Taille du code généré** | 4 octets (deux instructions `OUT (C),r`) |
| **Dépendance** | Aucune |
| **Note** | La macro suppose que `B` pointe déjà sur le port Gate Array. |

---

## 3. Fonctionnement détaillé

### 3.1. Contexte matériel (Amstrad CPC)

Le Gate Array est accessible via le port d’E/S situé à l’adresse `0x7F00` (octet haut = `0x7F`, octet bas = `0x00`).  
L’écriture dans le Gate Array se fait en deux étapes :

1. **Sélection du registre** : on écrit le numéro du registre (0‑15 pour les encres, 16 pour la bordure) dans le port.
2. **Écriture de la valeur** : on écrit la couleur matérielle (0‑15) dans le même port.

Le port reste le même pour les deux écritures (`0x7F00`). La première écriture détermine le registre, la seconde écrit la donnée.

### 3.2. Code généré

La macro se contente de deux instructions :

```z80
OUT (C), A      ; Écrit la valeur de A (numéro de stylo) dans le port pointé par B
OUT (C), D      ; Écrit la valeur de D (couleur) dans le même port
```

### 3.3. Exemple d’utilisation typique

```z80
; Configuration du port GA : B = 0x7F
LD  B, HI(GA_PORT)

; Choisir le stylo 5 et lui attribuer la couleur D
LD   A, PEN_5
LD   D, HW_BRIGHT_WHITE
mGA_SET_PEN_INK
```

**Remarque** :  
- Le numéro de stylo `A` peut aller de 0 à 15 pour les encres logiques, et 16 pour la bordure.  
- La couleur `D` est une valeur matérielle correspondant aux couleurs hardware du CPC.
  
---

## 4. Dépendances et configuration

La macro ne dépend d’aucun symbole externe. Elle est auto‑suffisante.

Cependant, pour fonctionner correctement, l’utilisateur doit s’assurer que :
- `B` contient l’octet haut du port Gate Array (généralement `0x7F`).
- `C` contient l’octet bas correspondant à la fonction visée (généralement `0x00` pour l’écriture des encres).
- `A` est compris entre 0 et 16 (inclus) pour le stylo.
- `D` est compris entre 0 et 15 pour la couleur matérielle.

---

## 5. Exemples d’utilisation

### 5.1. Définir plusieurs encres dans une boucle

```z80
; Table des couleurs pour les 16 stylos
ColorTable: DEFB ......

LD   B, HI(GA_PORT)
LD   C, 0
LD   HL, ColorTable
LD   D, 0        ; compteur de stylo
.loop:
    LD   A, D
    LD   E, (HL)  ; couleur
    PUSH DE
    mGA_SET_PEN_INK (VOID)
    POP  DE
    INC  D
    INC  HL
    LD   A, D
    CP   16
    JR   NZ, .loop
```
### 5.2. Changer la bordure
```z80
LD  A, GA_PEN_BORDER
LD  B, HI(GA_PORT)
LD  D, HW_BRIGHT_WHITE
mGA_SET_PEN_INK (VOID)
```
---

## 6. Précautions et bonnes pratiques

| Point d’attention | Recommandation |
| :--- | :--- |
| **Valeur de C** | La macro n’initialise pas `C`. L’utilisateur doit le définir avant l’appel, généralement à `0x00`. Si `C` contient une autre valeur, l’écriture pourrait affecter un registre différent du Gate Array. |
| **Registres modifiés** | La macro ne modifie aucun registre (elle lit `A`, `D`, `B`, `C` mais ne les altère pas). |
| **Ordre des écritures** | La première écriture (`OUT (C),A`) sélectionne le registre, la seconde écrit la donnée. Il est impératif de respecter cet ordre. |
| **Validité des valeurs** | Assurez-vous que `A` est dans [0..16] et `D` dans [0..15]. Des valeurs hors plage peuvent provoquer un comportement indéfini du Gate Array. |
| **Portabilité** | Cette macro est spécifique à l’Amstrad CPC. Elle ne fonctionnera pas sur d’autres machines Z80 sans adaptation. |

---

## 7. Optimisations possibles

La macro est déjà très compacte (4 octets) et rapide (deux instructions OUT). Aucune optimisation significative n’est possible au niveau du code généré.

Cependant, si vous devez définir plusieurs encres à la suite, vous pouvez enchaîner les appels sans recharger `B` et `C` à chaque fois, ce qui économise des instructions.

Exemple :

```z80
; Initialisation une fois
LD   B, HI(GA_PORT)
LD   C, #00

; Définir stylo noir
LD   A, GA_PEN_0
LD   D, HW_BLACK
mGA_SET_PEN_INK (VOID)

; Définir stylo bleu
LD   A, GA_PEN_1
LD   D, HW_BLUE
mGA_SET_PEN_INK
; ...
```
---
### 7.1 Liste des 27 couleurs 

HW_BLACK<BR>
HW_BLUE<BR>
HW_BRIGHT_BLUE<BR>
HW_RED<BR>
HW_MAGENTA<BR>
HW_MAUVE<BR>
HW_BRIGHT_RED<BR>
HW_PURPLE<BR>
HW_BRIGHT_MAGENTA<BR>
HW_GREEN<BR>
HW_CYAN<BR>
HW_SKY_BLUE<BR>
HW_YELLOW<BR>
HW_WHITE<BR>
HW_PASTEL_BLUE<BR>
HW_ORANGE<BR>
HW_PINK<BR>
HW_PASTEL_MAGENTA<BR>
HW_BRIGHT_GREEN<BR>
HW_SEA_GREEN<BR>
HW_BRIGHT_CYAN<BR>
HW_LIME_GREEN<BR>
HW_PASTEL_GREEN<BR>
HW_PASTEL_CYAN<BR>
HW_BRIGHT_YELLOW<BR>
HW_PASTEL_YELLOW<BR>
HW_BRIGHT_WHITE<BR>
---
## 8. Conclusion
La macro **`mGA_SET_PEN_INK`** est une brique élémentaire mais essentielle pour le contrôle des couleurs sur Amstrad CPC. Sa simplicité (deux OUT) la rend extrêmement efficace et facile à intégrer dans des boucles de dessin ou des initialisations d’écran.

Elle est complémentaire à la macro `mGA_SET_MODE` (pour le mode d’interruption) et s’intègre parfaitement dans une bibliothèque de macros dédiée au Gate Array.

---

## 9. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 1.0 | 2026-08-07 | Patrick MAES | Version initiale. Documentation complète du fonctionnement. |

---

*Références :*  
- Documentation technique de l’Amstrad CPC (schéma du Gate Array).  
- Code source original adapté pour RASM.
````

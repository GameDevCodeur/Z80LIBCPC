
# `mGA_SELECT_PEN` — Technical Reference

> **Macro de sélection d'un stylo pour le Gate Array Amstrad CPC**  
> *Version : 1.0 | Statut : Stable | Syntaxe : RASM*

---

## 1. Abstract

`mGA_SELECT_PEN` est une macro assembleur Z80 (syntaxe RASM) conçue pour sélectionner un stylo (0 à 15) dans le **Gate Array** de l'Amstrad CPC. 

Cette opération est le prérequis nécessaire avant d'envoyer une nouvelle valeur de couleur hardware vers ce stylo. La macro est optimisée pour être minimaliste : elle ne charge que le registre `B` (`#7F`) pour sélectionner le Gate Array, exploitant le fait que le registre `C` n'est **pas décodé** par ce composant.

---

## 2. Interface

| Élément | Spécification |
| :--- | :--- |
| **Prototype** | `MACRO mGA_SELECT_PEN pen` |
| **Paramètre** | `pen` : Numéro du stylo à sélectionner (valeur immédiate ou constante, comprise entre `0` et `15`). |
| **Entrées** | Aucune (la macro initialise les registres). |
| **Sortie** | Le Gate Array est configuré pour que le prochain `OUT (C),A` applique la couleur hardware au stylo sélectionné. |
| **Registres détruits** | `A` (écrasé par le numéro du stylo), `B` (écrasé par `#7F`). |
| **Registres préservés** | `C`, `D`, `E`, `H`, `L` (non modifiés). |

---

## 3. Contexte technique : Adressage du Gate Array

### 3.1. Décodage d'adresse

Contrairement au PPI (Port `&F4xx`) ou au FDC, le Gate Array du CPC utilise un **décodage partiel** de l'adresse :

- **Bit 15** doit être à `0`
- **Bit 14** doit être à `1`

Le **reste des bits (incluant le registre `C`)** est **ignoré** par le Gate Array.

L'adresse communément utilisée est donc `&7Fxx`, où `B = &7F` (`0111 1111` en binaire, ce qui satisfait les deux conditions).

### 3.2. Rôle du registre C

Le registre `C` n'étant pas décodé, il peut contenir **n'importe quelle valeur** lors de l'exécution de `OUT (C),A`. Cela permet d'écrire dans le Gate Array sans avoir à initialiser `C`, ce qui économise un chargement d'instruction.

---

## 4. Algorithme

La sélection d'un stylo se fait en deux étapes matérielles :

1. **Placer l'adresse du Gate Array dans `B`** (`#7F`).
2. **Envoyer le numéro du stylo** sur le bus de données via `OUT (C),A`.

Le Gate Array, voyant l'adresse correspondante, accepte la donnée et "gèle" le stylo ciblé pour la prochaine écriture couleur.

### 4.1. Pseudo-code

```pascal
B = #7F
A = pen
OUT (C), A    ; C est un "don't care" (valeur ignorée)
```

---

## 5. Code source

### 5.1. Version standard (RASM)

```asm
;-----------------------------------------------------------
; mGA_SELECT_PEN - Sélectionne un stylo dans le Gate Array
;
; Paramètre :
;   pen : Numéro du stylo (0-15)
;
; Sortie :
;   Gate Array prêt à recevoir une couleur (prochain OUT)
;
; Détruit :
;   A, B
;-----------------------------------------------------------
MACRO mGA_SELECT_PEN pen
    LD   B, #7F        ; Bits 15=0, 14=1 → décodage Gate Array
    LD   A, pen        ; Numéro du stylo à sélectionner
    OUT  (C), A        ; Écriture dans le port (C ignoré)
ENDM
```

### 5.2. Version inlinée (sans macro)

Si vous préférez ne pas utiliser de macro, vous pouvez directement écrire :

```asm
LD   B, #7F
LD   A, 6        ; Sélection du stylo 6
OUT  (C), A
```

---

## 6. Analyse des performances

### 6.1. Décompte cyclique (en T-states)

| Instruction | Coût unitaire | Total |
| :--- | :---: | :---: |
| `LD B, #7F` | 7 | 7 |
| `LD A, pen` | 7 | 7 |
| `OUT (C), A` | 11 | 11 |
| **Total** | - | **25 T-states** |

### 6.2. Latence

À **4 MHz** (CPC standard), l'exécution de cette macro dure :

`25 / 4 000 000 = 6,25 µs`

Ce temps est **négligeable** et permet de changer de stylo extrêmement rapidement.

---

## 7. Exemples d'utilisation

### 7.1. Sélection puis écriture de la couleur

```asm
; Sélectionne le stylo 6
mGA_SELECT_PEN 6

; Définit la couleur du stylo 6 en blanc
LD   A, #FF
OUT  (C), A
```

### 7.2. Sélection par constante symbolique

```asm
GA_PEN_BORDER EQU 0
GA_PEN_INK    EQU 6

mGA_SELECT_PEN GA_PEN_BORDER
LD   A, HW_BLUE
OUT  (C), A
```

### 7.3. Intégration dans une routine de clignotement

```asm
FLASH_TOGGLE:
    ; Bascule de l'état du stylo 6
    RRC  (HL)                    ; Carry alterne 0/1
    JR   NC, .set_black

.set_white:
    mGA_SELECT_PEN 6
    LD   A, HW_BRIGHT_WHITE
    OUT  (C), A
    JR   .done

.set_black:
    mGA_SELECT_PEN 6
    LD   A, HW_BLACK
    OUT  (C), A

.done:
    RET
```

---

## 8. Limitations et bonnes pratiques

| Point | Détail |
| :--- | :--- |
| **Registre `C` non initialisé** | Bien que `C` soit ignoré par le Gate Array, il est recommandé de ne pas l'utiliser comme pointeur de données valide juste après cette macro, car sa valeur reste indéfinie. |
| **Adresse `#7F` obligatoire** | `B` doit **impérativement** être à `#7F`. Un autre registre (`#F4`, `#FB`, etc.) ne sélectionnera pas le Gate Array et pourrait écrire dans le PPI ou ailleurs. |
| **Ordre des opérations** | Il est impératif que le `OUT (C),A` du stylo précède le `OUT (C),A` de la couleur. Si l'ordre est inversé, le Gate Array interprétera la couleur comme un numéro de stylo. |

---

## 9. Différence avec `mGA_SET_PEN_INK`

| Macro | Nombre d'`OUT` | Utilisation |
| :--- | :--- | :--- |
| `mGA_SELECT_PEN` | 1 (sélection) | Flexible : permet d'écrire plusieurs couleurs sur le même stylo sans le resélectionner. |
| `mGA_SET_PEN_INK` | 2 (sélection + couleur) | **Recommandée** en une seule ligne, plus concise, mais re-sélectionne le stylo à chaque appel. |

> 💡 **Choix** : Utilisez `mGA_SELECT_PEN` si vous devez appliquer plusieurs couleurs différentes au même stylo (ex : effacer un texte). Utilisez `mGA_SET_PEN_INK` pour la plupart des cas simples où une seule couleur est appliquée.

---

## 10. Conclusion

`mGA_SELECT_PEN` est une macro **simple, rapide et fiable** pour sélectionner un stylo dans le Gate Array. Elle repose sur une compréhension fine du décodage matériel du CPC, exploitant l'ignorance du registre `C` pour gagner en rapidité et en lisibilité.

Elle est la brique de base de tout programme graphique sur CPC, qu'il s'agisse de jeux, de démos ou d'outils système.

> 🔧 **Recommandation** : Associez cette macro à des couleurs prédéfinies (`HW_WHITE`, `HW_BLACK`, etc.) pour un code clair et maintenable.


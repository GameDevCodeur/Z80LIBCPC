
````markdown
# 📘 Documentation technique : Routine `F_SCENE_CHARGER`

**Auteur** : Analyse personnalisée pour développement Z80 / Amstrad CPC  
**Version** : 1.0  
**Date** : 2026-08-08  
**Assembleur** : RASM (ou compatible)  

---

## 1. Introduction

La routine **`F_SCENE_CHARGER`** est un **chargeur de scène** (scene dispatcher)
pour un programme structuré en états (menus, jeu, écrans de chargement, etc.).

Elle assure la transition entre deux scènes en :

1. **Invalidant** le vecteur d’interruption IM1 couran (pour éviter tout appel intempestif pendant le changement).
2. **Cherchant** dans une table (`SCENE_TABLE`) l’entrée correspondant à l’identifiant fourni.
3. **Mettant à jour** les pointeurs de la scène active (`SCENE_ACTUEL`) et du tableau IM1 associé.
4. **Sautant** directement vers la routine d’initialisation de la nouvelle scène (sans retourner à l’appelant).

Cette routine est conçue pour être utilisée dans un système multi‑scènes, où chaque scène possède sa propre boucle
principale et ses propres gestionnaires d’interruptions.

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `F_SCENE_CHARGER` |
| **Paramètres** | Aucun |
| **Entrées** | `B` = Identifiant de la scène (`ID_SCENE_xxx`), doit être séquentiel de 0 à `SCENE_TABLE_COUNT-1` |
| **Sortie** | Si l’ID est valide : **saut vers `F_SCENE_xxx_INITIALISATION`** (ne retourne pas)<br>Si l’ID est invalide : retour à l’appelant (`RET`) avec `Carry` non modifié |
| **Registres détruits** | `AF`, `DE`, `HL` |
| **Registres préservés** | `BC` (sauf `B` modifié implicitement mais `C` est préservé), `IX`, `IY` |
| **Taille du code** | 44 octets |
| **Dépendances** | `SCENE_TABLE` (table de 6 octets par scène)<br>`SCENE_TABLE_COUNT` (constante)<br>`IM1_NOT_READY` (constante, généralement `0xFF`)<br>`IM1_INDEX`, `IM1_CURRENT`, `SCENE_ACTUEL` (variables mémoire) |

---

## 3. Fonctionnement détaillé

### 3.1. Architecture des scènes

Le programme est découpé en **scènes** (ex: `SCENE_MENU`, `SCENE_JEU`, `SCENE_CHARGEMENT`).  
Pour chaque scène, la table `SCENE_TABLE` contient une entrée de **6 octets** (3 mots de 16 bits) :

```z80
; Structure d'une entrée SCENE_TABLE
; ┌────────────────────────────┐
; │ DW F_SCENE_xxx             │  ; Pointeur vers la routine de la scène (boucle principale)
; │ DW ARRAY_IM1_xxx           │  ; Pointeur vers le tableau de routines IM1 pour cette scène
; │ DW F_SCENE_xxx_INIT        │  ; Pointeur vers la routine d'initialisation de la scène
; └────────────────────────────┘
```

**Ordre** : L’identifiant de la scène (`ID_SCENE_xxx`) doit correspondre à **l’index** dans cette table (0, 1, 2…).

### 3.2. Déroulement pas à pas

#### Étape 1 : Invalidation du vecteur IM1
```z80
LD   A, IM1_NOT_READY
LD   (IM1_INDEX), A
```
- On place la valeur `IM1_NOT_READY` (ex: `0xFF`) dans la variable `IM1_INDEX`.
- Pendant la transition, si une interruption IM1 survient, le dispatcher d’interruption voit cet index invalide<br>et ne fait rien (au lieu d’appeler un pointeur corrompu).

#### Étape 2 : Vérification des limites
```z80
LD   A, B
CP   SCENE_TABLE_COUNT
RET  NC
```
- Si `B >= SCENE_TABLE_COUNT` (flag `C` = 0, donc `NC`), la routine retourne sans rien faire.
- **Comportement volontaire** : cela laisse `IM1_INDEX` à `IM1_NOT_READY`, ce qui fige les interruptions<br>jusqu’au prochain changement valide. C’est un signal de bug si un identifiant invalide est passé par erreur.

#### Étape 3 : Calcul de l’offset dans la table (`ID × 6`)
```z80
ADD  A, A          ; A = ID * 2
LD   E, A          ; E = ID * 2
ADD  A, A          ; A = ID * 4
ADD  A, E          ; A = ID * 4 + ID * 2 = ID * 6
LD   L, A
LD   H, 0
LD   DE, SCENE_TABLE
ADD  HL, DE        ; HL = SCENE_TABLE + ID * 6
```
- Chaque entrée fait 6 octets (`3 × DW`).
- Le calcul utilise seulement le registre `A` et une addition 16 bits finale, ce qui est compact et rapide.

#### Étape 4 : Lecture des 3 pointeurs
```z80
.scene_found
    ; 1er mot : F_SCENE_xxx
    LD   E, (HL) : INC HL
    LD   D, (HL) : INC HL
    PUSH DE

    ; 2e mot : ARRAY_IM1_xxx
    LD   E, (HL) : INC HL
    LD   D, (HL) : INC HL
    LD   (IM1_CURRENT), DE

    ; 3e mot : F_SCENE_xxx_INITIALISATION
    LD   E, (HL) : INC HL
    LD   D, (HL)
```
- Les trois mots sont lus séquentiellement.
- Le premier (`F_SCENE_xxx`) est empilé pour être restauré plus tard dans `SCENE_ACTUEL`.
- Le second (`ARRAY_IM1_xxx`) est directement stocké dans `IM1_CURRENT` (le pointeur que le dispatcher IM1 utilisera).
- Le troisième (`F_SCENE_xxx_INIT`) est conservé dans `DE`.

#### Étape 5 : Mise à jour du pointeur de scène courant
```z80
POP  HL
LD   (SCENE_ACTUEL+1), HL
```
- On récupère `F_SCENE_xxx` dans `HL`.
- On le stocke à l’adresse `SCENE_ACTUEL + 1`.  
  **Convention** : `SCENE_ACTUEL` est probablement une instruction `JP (HL)` ou `CALL` (3 octets). Le `+1` correspond <br>au premier octet de l’adresse (partie basse).  
  Ainsi, modifier `(SCENE_ACTUEL+1)` et `(SCENE_ACTUEL+2)` (implicitement via le mot) change la cible du saut.

#### Étape 6 : Saut vers l’initialisation (tail‑call)
```z80
PUSH DE
RET
```
- On empile l’adresse de `F_SCENE_xxx_INITIALISATION`.
- Le `RET` dépile cette adresse et y saute.  
  **Résultat** : La routine ne retourne pas à l’appelant ; elle passe directement à l’initialisation de la nouvelle scène. <br>C’est une optimisation de type *tail‑call* (économie d’un `JP`).

---

## 4. Dépendances et configuration

| Symbole / Variable | Type | Exemple de définition | Rôle |
| :--- | :--- | :--- | :--- |
| `SCENE_TABLE` | Étiquette | `SCENE_TABLE:` | Début de la table des scènes (6 octets par entrée). |
| `SCENE_TABLE_COUNT` | Constante | `SCENE_TABLE_COUNT EQU 3` | Nombre total de scènes. |
| `IM1_NOT_READY` | Constante | `IM1_NOT_READY EQU $FF` | Valeur marquant un index IM1 invalide. |
| `IM1_INDEX` | Variable (1 octet) | `IM1_INDEX: DB 0` | Index courant du vecteur IM1 (utilisé par le dispatcher). |
| `IM1_CURRENT` | Variable (2 octets) | `IM1_CURRENT: DW 0` | Pointeur vers le tableau IM1 de la scène active. |
| `SCENE_ACTUEL` | Variable (3 octets) | `SCENE_ACTUEL: JP 0` | Instruction de saut vers la routine de la scène active. |

**Exemple de table pour 3 scènes :**

```z80
SCENE_TABLE:
    ; Scène 0 (Menu)
    DW F_SCENE_MENU
    DW ARRAY_IM1_MENU
    DW F_SCENE_MENU_INIT

    ; Scène 1 (Jeu)
    DW F_SCENE_JEU
    DW ARRAY_IM1_JEU
    DW F_SCENE_JEU_INIT

    ; Scène 2 (Chargement)
    DW F_SCENE_CHARGEMENT
    DW ARRAY_IM1_CHARGEMENT
    DW F_SCENE_CHARGEMENT_INIT

SCENE_TABLE_COUNT EQU 3
```

---

## 5. Exemples d’utilisation

### 5.1. Appel depuis une routine système

```z80
; Passer à la scène "Jeu" (ID = 1)
LD   B, 1
CALL F_SCENE_CHARGER
; Ici, on ne revient jamais car F_SCENE_CHARGER saute vers F_SCENE_JEU_INIT.
```

### 5.2. Gestion d’un ID invalide

```z80
LD   B, 5          ; ID inexistant (SCENE_TABLE_COUNT = 3)
CALL F_SCENE_CHARGER
; La routine retourne, IM1_INDEX reste à IM1_NOT_READY.
; Les interruptions IM1 sont désactivées (le dispatcher ne fait rien).
```

### 5.3. Structure du dispatcher IM1

Pour que l’invalidation fonctionne, le dispatcher IM1 doit ressembler à :

```z80
F_IM1_DISPATCH:
    LD   A, (IM1_INDEX)
    CP   IM1_NOT_READY
    RET  Z                 ; Si invalide, on sort
    ; Sinon, on utilise IM1_CURRENT et l'index pour sauter vers la bonne routine
    ; ...
```

---

## 6. Précautions et bonnes pratiques

| Point d’attention | Recommandation |
| :--- | :--- |
| **Non‑retour** | La routine **ne revient pas** à l’appelant si l’ID est valide. Elle saute directement vers l’init de la scène. Si votre code attend un `RET` pour continuer, cela ne fonctionnera pas. |
| **Invalidation IM1** | L’invalidation est volontaire pour éviter les appels d’interruption pendant la transition. Cependant, si la transition est trop longue, les interruptions sont perdues. Assurez‑vous que le temps de l’init est court ou que vous gérez les interruptions différemment. |
| **Registres modifiés** | `AF`, `DE`, `HL` sont écrasés. Si l’appelant a besoin de ces valeurs, il doit les sauvegarder (`PUSH`). |
| **Ordonnancement des ID** | Les identifiants `ID_SCENE_xxx` doivent être **séquentiels** (0, 1, 2…) et correspondre à l’ordre dans `SCENE_TABLE`. Toute lacune provoquera un comportement erroné. |
| **Taille de `SCENE_TABLE_COUNT`** | Doit être une constante connue à l’assemblage. L’instruction `CP SCENE_TABLE_COUNT` fonctionne avec une valeur immédiate. |
| **`SCENE_ACTUEL+1`** | Cette adresse doit pointer sur un mot de 2 octets modifiable (généralement les deux derniers octets d’une instruction `JP`). Si `SCENE_ACTUEL` est une instruction `CALL`, cela fonctionne aussi. |

---

## 7. Analyse de la taille (44 octets)

Découpage des instructions (approximatif) :

| Section | Taille (octets) |
| :--- | :--- |
| Invalidation + vérification limites | 7 |
| Calcul offset (`ID×6`) | 7 |
| Lecture des 3 mots (`LD E,(HL)`, etc.) | 15 |
| Mise à jour `IM1_CURRENT` | 3 |
| POP + stockage dans `SCENE_ACTUEL+1` | 4 |
| PUSH + RET (tail‑call) | 3 |
| Déplacements et étiquettes | ~5 |
| **Total** | **44** |

La routine est très compacte pour un chargeur de scène générique, grâce à une gestion efficace des adresses <br>et à l’utilisation d’un `RET` astucieux pour le saut final.

---

## 8. Conclusion

La routine **`F_SCENE_CHARGER`** est le **cœur du système de changement d’état** d’un jeu ou d’une application modulaire sur Z80.  
Elle offre :

- ✅ Une **validation** des identifiants.
- ✅ Une **protection** contre les interruptions intempestives.
- ✅ Une **transition instantanée** vers la nouvelle scène.
- ✅ Un code **compact** et **rapide** (44 octets).

En combinant cette routine avec une table de scènes bien structurée et un dispatcher IM1 robuste, <br>vous obtenez un squelette de programme flexible et fiable pour l’Amstrad CPC.

---

## 9. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 1.0 | 2026-08-08 | Patrick MAES | Version initiale. Documentation complète du fonctionnement. |

---

*Références :*  
- Code source original adapté pour RASM.  
- Convention de programmation système pour Amstrad CPC.
````

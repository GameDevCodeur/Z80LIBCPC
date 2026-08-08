
# 📘 Documentation technique : `F_SCENE_CHARGER` 

**Auteur** : Patrick MAES - Analyse personnalisée pour développement Z80 / Amstrad CPC  
**Version** : 2.0 – Optimisation mémoire et vitesse  
**Date** : 2026-08-08  
**Assembleur** : RASM (ou compatible)  

---

## 1. Introduction

La routine **`F_SCENE_CHARGER`** est le cœur du mécanisme de changement de scène dans un programme modulaire.  
Cette version **page‑alignée** exploite la structure particulière de la table `SCENE_TABLE` pour éliminer une opération d’addition 16 bits (remplacée par un simple chargement de la page haute) et utilise des incrémentations `INC L` (8 bits) qui sont plus rapides que `INC HL`.  

Le gain est significatif : **environ 15 % de cycles en moins** par rapport à la version classique, pour un coût en taille quasi identique.

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `F_SCENE_CHARGER` |
| **Paramètres** | Aucun |
| **Entrée** | `B` = identifiant de la scène (0 … `SCENE_TABLE_COUNT`‑1) |
| **Sortie** | Si ID valide : **saut** vers `F_SCENE_xxx_INIT` (tail‑call, pas de retour)<br>Si ID invalide : `RET` (Carry non modifié) |
| **Registres détruits** | `AF`, `BC`, `DE`, `HL` |
| **Registres préservés** | `IX`, `IY` (non utilisés) |
| **Taille** | ~40 octets (variable selon les macros) |
| **Prérequis** | `SCENE_TABLE` **doit être alignée sur 256 octets** (page mémoire) et sa taille totale doit être < 256 octets. |
| **Dépendances** | `IM1_NOT_READY`, `IM1_INDEX`, `IM1_CURRENT`, `SCENE_ACTUEL`<br>`SCENE_TABLE` (table 6 octets par entrée)<br>`SCENE_TABLE_COUNT` (constante) |

---

## 3. Principe de la version page‑alignée

### 3.1. Pourquoi aligner `SCENE_TABLE` ?

Une table alignée sur 256 octets signifie que son adresse de base a la forme `0xXX00` (octet bas = 0).  
Lorsque l’on calcule l’adresse d’une entrée, on peut alors :

- Utiliser **uniquement l’octet bas** (`L`) comme décalage.
- Charger l’octet haut (`H`) directement avec la page de la table (`HI(SCENE_TABLE)`).

Cela évite l’instruction `ADD HL, DE` (qui prend 11 cycles) et permet d’utiliser des incrémentations 8 bits (`INC L`) qui ne modifient pas `H` (plus rapides, 4 cycles au lieu de 6 pour `INC HL`).

### 3.2. Structure de `SCENE_TABLE`

Chaque entrée fait **6 octets** (3 mots de 16 bits) :

| Offset | Contenu |
| :--- | :--- |
| +0 | `F_SCENE_xxx`      | (routine principale) |
| +2 | `ARRAY_IM1_xxx`    | (tableau des routines IM1) |
| +4 | `F_SCENE_xxx_INIT` | (routine d’initialisation) |

Les identifiants `ID_SCENE` sont séquentiels (0, 1, 2…) et correspondent à l’ordre dans la table.

### 3.3. Calcul de l’offset

L’adresse d’une entrée est :

```
adresse = SCENE_TABLE + ID × 6
```

Comme `SCENE_TABLE` est alignée sur 256, `adresse` s’écrit :

- **Octet bas** = `ID × 6` (mod 256, mais puisque la table < 256, pas de débordement)
- **Octet haut** = `HI(SCENE_TABLE)`

Ainsi, on peut charger directement :

```z80
LD   L, A       ; A = ID*6
LD   H, HI(SCENE_TABLE)
```

---

## 4. Fonctionnement détaillé

### 4.1. Invalidation IM1

```z80
LD   A, IM1_NOT_READY
LD   (IM1_INDEX), A
```
On place une valeur sentinelle dans la variable d’index du dispatcher IM1.  
Pendant la transition, si une interruption survient, le dispatcher ne fera rien.

### 4.2. Vérification des limites

```z80
LD   A, B
CP   SCENE_TABLE_COUNT
RET  NC
```
Si l’ID est hors limite, on sort (IM1 reste invalidé).

### 4.3. Calcul de l’offset (`ID × 6`)

```z80
ADD  A, A        ; A = ID*2
LD   E, A        ; E = ID*2
ADD  A, A        ; A = ID*4
ADD  A, E        ; A = ID*6
```

On utilise `E` comme registre temporaire. À la fin, `A` contient l’offset en octets.

### 4.4. Chargement de l’adresse de base de la table

```z80
LD   L, A
LD   H, HI(SCENE_TABLE)
```

`HL` pointe désormais directement sur l’entrée de la scène.

### 4.5. Lecture des trois pointeurs (avec `INC L`)

```z80
LD   C, (HL) : INC L
LD   B, (HL) : INC L    ; BC = F_SCENE_xxx

LD   E, (HL) : INC L
LD   D, (HL) : INC L    ; DE = ARRAY_IM1_xxx

LD   (IM1_CURRENT), DE

LD   E, (HL) : INC L
LD   D, (HL)            ; DE = F_SCENE_xxx_INIT
```

**Remarque** : On utilise `INC L` au lieu de `INC HL`, ce qui est plus rapide et ne modifie pas `H`.  
**Condition** : La table ne doit pas dépasser 256 octets (sinon `INC L` provoquerait un wrap‑around, corrompant l’adresse).

### 4.6. Mise à jour du saut vers la routine principale

```z80
LD   (SCENE_ACTUEL+1), BC
```
`SCENE_ACTUEL` est une instruction `JP 0` (3 octets). On modifie les deux derniers octets pour pointer sur `F_SCENE_xxx`.

### 4.7. Saut vers l’initialisation (tail‑call)

```z80
EX   DE, HL        ; HL = adresse de F_SCENE_xxx_INIT
JP   (HL)
```
Au lieu d’un `PUSH DE / RET`, on utilise `EX DE, HL` suivi de `JP (HL)`. C’est équivalent et parfois plus rapide (12 cycles au lieu de `PUSH` + `RET` = 17 cycles).

---

## 5. Code source complet (version page‑alignée)

```z80
; ================================================================
; FONCTION : F_SCENE_CHARGER — Version page‑alignée
; ================================================================
; PRÉREQUIS : SCENE_TABLE doit être alignée sur 256 octets
;             (ex: ALIGN 256 devant la définition).
;             Vérifier que SCENE_TABLE_COUNT * 6 < 256.
; ================================================================

F_SCENE_CHARGER:
    ; ---- Invalidation IM1 ----
    LD   A, IM1_NOT_READY
    LD   (IM1_INDEX), A

    ; ---- Vérification limites ----
    LD   A, B
    CP   SCENE_TABLE_COUNT
    RET  NC

    ; ---- Calcul offset = ID * 6 ----
    ADD  A, A
    LD   E, A
    ADD  A, A
    ADD  A, E

    ; ---- Adresse de l'entrée = SCENE_TABLE + offset ----
    LD   L, A
    LD   H, HI(SCENE_TABLE)

    ; ---- Lire F_SCENE_xxx ----
    LD   C, (HL) : INC L
    LD   B, (HL) : INC L

    ; ---- Lire ARRAY_IM1_xxx ----
    LD   E, (HL) : INC L
    LD   D, (HL) : INC L
    LD   (IM1_CURRENT), DE

    ; ---- Lire F_SCENE_xxx_INIT ----
    LD   E, (HL) : INC L
    LD   D, (HL)

    ; ---- Mettre à jour SCENE_ACTUEL ----
    LD   (SCENE_ACTUEL+1), BC

    ; ---- Sauter vers l'initialisation ----
    EX   DE, HL
    JP   (HL)
```

---

## 6. Dépendances et configuration

### 6.1. Variables système

```z80
IM1_NOT_READY  EQU $FF          ; Sentinelle d'invalidation
IM1_INDEX:     DB 0             ; Index courant pour le dispatcher IM1
IM1_CURRENT:   DW 0             ; Pointeur vers le tableau IM1 de la scène active
SCENE_ACTUEL:  JP  0            ; Instruction JP modifiable dynamiquement
```

### 6.2. Table des scènes

```z80
; ---- Définition de la table (doit être alignée) ----
ALIGN 256
SCENE_TABLE:
    DW F_SCENE_TITRE
    DW ARRAY_IM1_TITRE
    DW F_SCENE_TITRE_INIT

    DW F_SCENE_MENU
    DW ARRAY_IM1_MENU
    DW F_SCENE_MENU_INIT

    DW F_SCENE_JEU
    DW ARRAY_IM1_JEU
    DW F_SCENE_JEU_INIT
    ; ... autant que nécessaire

SCENE_TABLE_COUNT EQU ($ - SCENE_TABLE) / 6
```

**Vérification** : Un `ASSERT` (si RASM le supporte) peut garantir que la table ne dépasse pas une page :

```z80
ASSERT ($ - SCENE_TABLE) < 256
```

---

## 7. Avantages et limites

### ✅ Avantages

- **Vitesse accrue** : environ 15 % de gagné sur la version classique (grâce à `INC L` et à l’élimination de `ADD HL, DE`).
- **Code compact** : pas plus long que la version standard (~40 octets).
- **Prévisibilité** : l’adresse est calculée en une seule instruction (chargement de `H` constant).

### ⚠️ Limites et précautions

- **Alignement obligatoire** : `SCENE_TABLE` doit commencer à une adresse multiple de 256. Dans RASM, on utilise `ALIGN 256` avant la définition.
- **Taille de la table** : Le nombre de scènes × 6 doit être strictement inférieur à 256 (sinon `INC L` fera un débordement de page et corrompra l’adresse). Pour plus de 42 scènes, il faut une autre approche.
- **Modification de `H` interdite** : `INC L` ne modifie pas `H`. Cela impose que la table soit sur une seule page. C’est le cas si l’alignement est respecté et la taille < 256.
- **Compatibilité** : Cette version est spécifique à la structure de `SCENE_TABLE` (6 octets par entrée). Elle ne fonctionne pas avec la version à 3 tables séparées.

---

## 8. Exemples d’utilisation

### 8.1. Appel depuis une routine

```z80
    LD   B, 1          ; Charger la scène MENU (ID = 1)
    CALL F_SCENE_CHARGER
    ; Ici, on ne revient jamais ; on saute vers F_SCENE_MENU_INIT
```

### 8.2. Assertion pour garantir la taille de la table

```z80
; Après la définition de SCENE_TABLE
IF ($ - SCENE_TABLE) >= 256
    ERROR "SCENE_TABLE dépasse 256 octets ! Utiliser une autre version."
ENDIF
```

---

## 9. Comparaison des performances

| Version | Calcul d’adresse | Incrémentations | Saut final | Cycles estimés |
| :--- | :--- | :--- | :--- | :--- |
| Classique (ADD HL,DE) | `LD L,A : LD H,0 : LD DE,SCENE_TABLE : ADD HL,DE` (3 instructions, ~17 cycles) | `INC HL` (3×) | `PUSH DE / RET` | ~70 |
| Page‑alignée (cette version) | `LD L,A : LD H,HI(...)` (1 instruction, ~7 cycles) | `INC L` (3×) | `EX DE,HL / JP (HL)` | ~60 |

Gain d’environ **15 %**.

---

## 10. Conclusion

La version page‑alignée de `F_SCENE_CHARGER` est une optimisation élégante pour les systèmes où la table des scènes peut être placée en mémoire alignée. Elle offre un meilleur temps d’exécution tout en restant compacte et lisible.

Elle s’intègre parfaitement dans un projet structuré en scènes et constitue un excellent choix pour les jeux ou démos exigeants en performance.

---

## 11. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 2.0 | 2026-08-08 | Analyse technique | Version page‑alignée, documentation détaillée. |
| 1.0 | 2026-08-07 | Original | Version classique (table unique). |

---

*Références :*  
- Documentation technique Z80 (cycles d’exécution).  
- Amstrad CPC system programming guides.

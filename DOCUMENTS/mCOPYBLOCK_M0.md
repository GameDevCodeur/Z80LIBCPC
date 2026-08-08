
# Analyse détaillée : `mCOPYBLOCK_M0`

## 1. Objectif général

La macro copie un bloc graphique de **8 lignes verticales** sur **4 pixels de large** (soit 2 octets par ligne) en **Mode 0** (4 bits par pixel, 2 pixels par octet).  
La source (`HL`) est organisée de manière **contiguë** en mémoire (16 octets consécutifs), tandis que la destination (`DE`) est une zone de l'écran physique de l'Amstrad CPC, dont l'organisation mémoire est volontairement entrelacée pour faciliter le balayage écran.

---

## 2. Contexte mémoire écran (Mode 0 CPC)

Sur CPC, l'adresse d'un pixel à l'écran se calcule ainsi :

```
Adresse = &C000 + (Y % 8) * 2048 + (Y // 8) * 80 + (X // 2)
```

- `Y % 8` correspond au numéro de la ligne **au sein d'un bloc de 8 lignes** (de 0 à 7).
- Les lignes d'un même bloc (0 à 7) sont espacées de **2048 octets** (`0x0800`).

Si l'on décompose l'adresse 16 bits en `D` (octet haut) et `E` (octet bas), on obtient :
- `D = &C0 + (Y % 8) * 8` (les bits 5, 4 et 3 codent la ligne dans le bloc).
- `E` contient le décalage horizontal (`X // 2`) ainsi que le numéro du bloc de caractères (`(Y // 8) * 80`).

**Correspondance entre la ligne `Y` et l'octet haut `D` :**

| Ligne `Y` (dans le bloc) | `D` (hexadécimal) | `D` (binaire) | Bits 5-4-3 |
|---------------------------|-------------------|---------------|------------|
| 0                         | `&C0`             | `1100 0000`   | `000`      |
| 1                         | `&C8`             | `1100 1000`   | `001`      |
| 2                         | `&D0`             | `1101 0000`   | `010`      |
| 3                         | `&D8`             | `1101 1000`   | `011`      |
| 4                         | `&E0`             | `1110 0000`   | `100`      |
| 5                         | `&E8`             | `1110 1000`   | `101`      |
| 6                         | `&F0`             | `1111 0000`   | `110`      |
| 7                         | `&F8`             | `1111 1000`   | `111`      |

---

## 3. Prérequis essentiels (⚠️ Critique)

> **Contrainte** : Les bits 5, 4 et 3 de l'octet `D` doivent impérativement être à `000`.

Cela signifie que `DE` doit pointer sur la **première ligne (`Y % 8 = 0`)** d'un bloc de 8 lignes.  
Exemples d'adresses valides : `&C000`, `&C050`, `&C0A0`...  
Exemple d'adresse **invalide** : `&C800` (car `D = &C8` possède les bits 5-4-3 = `001`).

> ⚠️ **Si ce prérequis n'est pas respecté**, l'incrémentation de `D` par 8 ne pointera pas sur la ligne suivante attendue, provoquant un décalage horizontal catastrophique ou une sortie du bloc mémoire dédié à l'écran.

---

## 4. Déroulement pas à pas

La macro est déroulée (unroll) pour optimiser la vitesse : 7 itérations explicites, puis une dernière ligne.

### Itérations 1 à 7 (répétées 7 fois)

```asm
LDI : LDI        ; Copie 2 octets (4 pixels) de (HL) vers (DE), incrémente HL et DE
DEC DE : DEC DE  ; Remet DE au début de la ligne copiée (on recule de 2)
LD  A,D          ; Charge l'octet haut dans A
ADD A,8          ; Ajoute 8 (équivaut à sauter à la ligne suivante du bloc)
LD  D,A          ; Sauvegarde dans D, DE pointe désormais sur la ligne suivante
```

### Dernière itération (8e ligne)

```asm
LDI : LDI        ; Copie la dernière ligne
```

> **Explication de `ADD A,8`** : Comme les lignes sont espacées de `0x0800` en mémoire, ajouter `8` à l'octet haut permet de passer de `&C0` à `&C8`, puis à `&D0`, etc., tout en conservant l'octet bas `E` intact (même colonne X).

---

## 5. Évolution de `DE` au cours de l'exécution (Exemple avec `DE = &C050`)

| Étape | Action                     | Adresse `DE` après action |
|-------|----------------------------|---------------------------|
| Départ| Initial                    | `&C050` (ligne 0)         |
| Tour 1| Copie ligne 0 + saut       | `&C850` (ligne 1)         |
| Tour 2| Copie ligne 1 + saut       | `&D050` (ligne 2)         |
| Tour 3| Copie ligne 2 + saut       | `&D850` (ligne 3)         |
| Tour 4| Copie ligne 3 + saut       | `&E050` (ligne 4)         |
| Tour 5| Copie ligne 4 + saut       | `&E850` (ligne 5)         |
| Tour 6| Copie ligne 5 + saut       | `&F050` (ligne 6)         |
| Tour 7| Copie ligne 6 + saut       | `&F850` (ligne 7)         |
| Final | Copie ligne 7 (sans saut)  | `&F852` (fin du bloc)     |

---

## 6. État des registres en sortie

| Registre | État après exécution |
|----------|-----------------------|
| **`HL`** | Avancé de 16 octets. Pointe après la zone source. |
| **`DE`** | Avancé de 2 octets par rapport à la 8ᵉ ligne copiée. Pointe sur le pixel suivant immédiatement à droite du bloc copié (pas de gestion du retour à la ligne). |
| **`A`**  | Contient la valeur de `D` de la dernière itération (modifiée). |
| **`F`**  | Les indicateurs (`C`, `Z`, etc.) sont modifiés par les instructions `LDI` et `ADD`. |

---

## 7. Performances (Temps d'exécution)

Calcul des cycles processeur (en T-states) :

- `LDI` = 16 T-states (× 2 = 32 pour deux `LDI`).
- `DEC DE` = 6 T-states (× 2 = 12 pour deux `DEC`).
- `LD A,D` = 4 T-states.
- `ADD A,8` = 7 T-states.
- `LD D,A` = 4 T-states.

**Total par itération (7 fois)** :  
`(32 + 12 + 15) = 59` T-states.

**Dernière ligne** :  
`2 × 16 = 32` T-states.

**Total général** :  
`(59 × 7) + 32 = 413 + 32 = 445` T-states.

À une fréquence de **4 MHz** (CPC standard), l'exécution dure environ **111 microsecondes** (`445 / 4 000 000`), ce qui est extrêmement rapide pour copier 16 octets avec un accès entrelacé.

---

## 8. Limitations importantes

| Limitation | Détail |
|------------|--------|
| **Absence de gestion des bandes (changement de bloc)** | La macro ne peut pas passer d'un bloc de 8 lignes au suivant (il faudrait ajouter 80 à `E` et remettre `D` à `&C0`). Elle est conçue strictement pour une colonne de 8 lignes à l'intérieur d'un même bloc de caractères. |
| **Alignement obligatoire** | `DE` doit avoir **obligatoirement** les bits 5-4-3 de `D` à 0. Aucune vérification ni correction n'est effectuée. |
| **Absence de gestion du chevauchement** | Si la source (`HL`) et la destination (`DE`) se chevauchent, les `LDI` peuvent écraser des données source non encore lues. |
| **Absence de gestion des bords de page mémoire** | Si on atteignait l'adresse `&F8xx` et qu'on ajoutait `8` à `D`, cela provoquerait un overflow (`&00xx`) sans alerte. Heureusement, le prérequis s'arrête à la ligne 7 (`&F8`), ce qui évite ce cas. |

---

## 9. Organisation attendue de la source (`HL`)

La mémoire source doit être **16 octets consécutifs**, organisés ligne par ligne de haut en bas :

| Adresse source | Contenu          |
|----------------|------------------|
| `HL + 0`       | Ligne 0, Octet 0 |
| `HL + 1`       | Ligne 0, Octet 1 |
| `HL + 2`       | Ligne 1, Octet 0 |
| `HL + 3`       | Ligne 1, Octet 1 |
| ...            | ...              |
| `HL + 14`      | Ligne 7, Octet 0 |
| `HL + 15`      | Ligne 7, Octet 1 |

C'est une représentation **linéaire** classique, opposée à la disposition physique entrelacée de l'écran CPC.

---

## 10. Conclusion

`mCOPYBLOCK_M0` est une macro **hautement optimisée** pour copier un petit bloc de `4×8` pixels en Mode 0. Elle exploite avec élégance la structure entrelacée du CPC en ne modifiant que l'octet haut de l'adresse destination. 

Elle est parfaitement adaptée au **tilemapping**, au **blitting de sprites** de petite taille, ou aux effets de **scroll vertical** limités à l'intérieur d'un même bloc de caractères.

> 🔧 **Recommandation** : À utiliser avec une extrême rigueur sur les adresses. Son efficacité repose entièrement sur le respect strict du prérequis d'alignement (`bits 5-4-3 de D = 000`). En cas de non-respect, les résultats sont imprévisibles et peuvent corrompre l'affichage.

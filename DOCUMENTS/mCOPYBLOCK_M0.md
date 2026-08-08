
# `mCOPYBLOCK_M0` — Technical Reference

> **Routine de copie graphique ultra-optimisée pour l’écran Amstrad CPC (Mode 0)**  
> *Version : 1.0 | Statut : Stable*

---

## 1. Abstract

`mCOPYBLOCK_M0` est une macro assembleur Z80 conçue pour transférer un bloc élémentaire de **8 lignes × 4 pixels** (largeur de 2 octets) depuis un buffer source linéaire (`HL`) vers la mémoire vidéo entrelacée du CPC (`DE`) en **Mode 0** (4 bits par pixel).  
Son exécution est entièrement déroulée (`unrolled`) pour atteindre une latence minimale de **~111 µs** à 4 MHz.

---

## 2. Interface

| Élément | Spécification |
| :--- | :--- |
| **Prototype** | `MACRO mCOPYBLOCK_M0` |
| **Entrées** | `HL` : Pointeur sur 16 octets sources (8 lignes × 2 octets, contigus).<br>`DE` : Adresse écran de destination (doit pointer sur le début d'un bloc de 8 lignes). |
| **Sorties** | `HL` : Avancé de 16 octets (pointe après la source).<br>`DE` : Avancé de 2 octets sur la dernière ligne (pointe après les 4 pixels copiés). |
| **Registres détruits** | `AF` (contenu modifié). |

---

## 3. Prérequis et contraintes

### 3.1. Alignement critique (⚠️ Condition sine qua non)

Les **bits 5, 4 et 3** de l'octet supérieur de l'adresse `DE` (registre `D`) **doivent impérativement être à `000`**.  
Cela signifie que `DE` doit pointer sur la **première ligne** (`Y % 8 = 0`) d'un bloc vertical de 8 lignes.

| ✅ Adresses valides | ❌ Adresses invalides |
| :--- | :--- |
| `&C000`, `&C050`, `&C0A0`, `&C0FA` | `&C800` (D = &C8 → bits = 001) |
| `&D000`, `&D050`... | `&D800` (D = &D8 → bits = 011) |

> **Conséquence en cas de non-respect** : L'incrémentation de `D` par `8` ne pointera pas sur la ligne physique suivante. Les données seront écrites dans une colonne horizontale différente, provoquant un affichage corrompu (effet « déchirure ») sans aucune alerte.

### 3.2. Gestion des bandes

La macro ne gère **pas** le passage d'une bande de 8 lignes à la suivante (changement de bloc de caractères). Elle est strictement confinée à un seul bloc vertical.

---

## 4. Mémoire vidéo CPC (Contexte technique)

En Mode 0, l’espace écran (`&C000` à `&FFFF`) est organisé pour faciliter le balayage raster. L’adresse d’un pixel est définie par :

```
Adresse = &C000 + (Y % 8) * 2048 + (Y // 8) * 80 + (X // 2)
```

L'incrémentation de **8** sur l'octet haut (`D`) permet de passer d'une ligne à la suivante au sein du même bloc (espacement de `0x0800` octets).

**Table de correspondance :**

| Ligne (Y % 8) | Octet haut (`D`) | Binaire `D` | Bits 5-4-3 |
| :---: | :---: | :---: | :---: |
| 0 | `&C0` | `1100 0000` | `000` |
| 1 | `&C8` | `1100 1000` | `001` |
| 2 | `&D0` | `1101 0000` | `010` |
| 3 | `&D8` | `1101 1000` | `011` |
| 4 | `&E0` | `1110 0000` | `100` |
| 5 | `&E8` | `1110 1000` | `101` |
| 6 | `&F0` | `1111 0000` | `110` |
| 7 | `&F8` | `1111 1000` | `111` |

---

## 5. Algorithme et flux d'exécution

La macro est déroulée : 7 itérations de copie + saut, suivies d'une copie terminale.

### 5.1. Pseudo-code

```pascal
Répéter 7 fois :
    Copier 2 octets de (HL) vers (DE)  [LDI x2]
    DE = DE - 2                         [Retour au début de la ligne]
    D  = D + 8                          [Saut à la ligne suivante]
Fin Répéter
Copier les 2 derniers octets            [Dernière ligne]
```

### 5.2. Traçage (Exemple avec `DE = &C050`)

| Étape | Opération | `DE` après exécution |
| :---: | :--- | :--- |
| Init | - | `&C050` (Ligne 0) |
| 1 | Copie L0 + `D += 8` | `&C850` (Ligne 1) |
| 2 | Copie L1 + `D += 8` | `&D050` (Ligne 2) |
| 3 | Copie L2 + `D += 8` | `&D850` (Ligne 3) |
| 4 | Copie L3 + `D += 8` | `&E050` (Ligne 4) |
| 5 | Copie L4 + `D += 8` | `&E850` (Ligne 5) |
| 6 | Copie L5 + `D += 8` | `&F050` (Ligne 6) |
| 7 | Copie L6 + `D += 8` | `&F850` (Ligne 7) |
| 8 | Copie L7 *(sans saut)* | `&F852` *(position finale)* |

---

## 6. Mesures de performance

### 6.1. Décompte cyclique (en T-states)

| Instruction | Coût unitaire | Occurrences | Total |
| :--- | :---: | :---: | :---: |
| `LDI` | 16 | 16 | 256 |
| `DEC DE` | 6 | 14 | 84 |
| `LD A, D` | 4 | 7 | 28 |
| `ADD A, 8` | 7 | 7 | 49 |
| `LD D, A` | 4 | 7 | 28 |
| **Total** | - | - | **445 T-states** |

### 6.2. Latence

- **Temps d'exécution** : `445 / 4 000 000 ≈ 111.25 µs` (à 4 MHz).
- **Débit** : 16 octets copiés en ~111 µs, soit **~144 Ko/s** pour ce type de transfert entrelacé.

---

## 7. Limitations et cas de bord

| Problème | Impact | Solution / Remarque |
| :--- | :--- | :--- |
| **Changement de bande (Bloc > 7)** | Impossible de passer à la bande suivante (il faudrait ajouter 80 à `E` et remettre `D` à `C0`). | La macro est conçue pour le tilemapping interne aux blocs. Utiliser un wrapper pour gérer les bandes. |
| **Chevauchement Source / Dest** | Si `HL` et `DE` se chevauchent, les `LDI` peuvent écraser des données source non lues. | S'assurer que les zones mémoire sont disjointes. |
| **Dépassement de page (`D` > `&F8`)** | Si une 9ᵉ itération était ajoutée, `D` passerait à `&00` (wrap). | Le prérequis d'arrêt à la ligne 7 évite ce cas. |

---

## 8. Exemple d'utilisation

```asm
; Copie un tile 4x8 depuis TILE_BUFFER vers l'écran à la colonne X=10, ligne Y=0
ld   hl, TILE_BUFFER          ; Source
ld   de, &C050                ; Dest (Ex: X=10/2=5, Y=0)
mCOPYBLOCK_M0                  ; Exécution
```

---

## 9. Journal des modifications

| Version | Date | Auteur | Description |
| :---: | :--- | :--- | :--- |
| 1.0 | 2026-08-08 | Analyse système | Spécification initiale et benchmarks. |

---

## 10. Licence d'utilisation

Ce document accompagne une macro destinée à un usage embarqué. L'utilisateur est seul responsable du respect des prérequis matériels (alignement mémoire) lors de l'intégration dans son projet.

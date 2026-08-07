
# 📘 Documentation technique : Macro `mGA_SET_MODE`

**Auteur** : Analyse personnalisée pour développement Z80 / Amstrad CPC  
**Version** : 1.0  
**Date** : 2026-08-07  
**Assembleur** : RASM (ou compatible avec les opérateurs `HI()` et la substitution `{}`)

---

## 1. Introduction

La macro **`mGA_SET_MODE`** est une interface bas‑niveau dédiée au **Gate Array** de l’Amstrad CPC.  
Elle permet de modifier le **registre de mode d’interruption (IMR – Interrupt Mode Register)** en une seule instruction, en tirant parti des particularités du mapping mémoire‑port du CPC.

Le Gate Array est le chip central qui gère la vidéo, les couleurs, et le contrôle des interruptions. L’écriture dans le registre IMR détermine le comportement de la ligne d’interruption `IRQ` (généralement utilisée par le firmware pour le ticking de la ROM, les lecture clavier, etc.).

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `mGA_SET_MODE` |
| **Paramètre** | `IMR` – valeur 8 bits à écrire dans le registre de mode d’interruption |
| **Sortie** | Aucune |
| **Registres détruits** | `BC` (modifiés intégralement) |
| **Registres préservés** | `AF`, `DE`, `HL`, `IX`, `IY` (non utilisés) |
| **Port matériel cible** | Gate Array (adresse base `0x7F00` en CPC) |
| **Dépendance** | L’étiquette `GA_PORT` doit être définie (ex: `GA_PORT EQU 0x7F00`) |

---

## 3. Fonctionnement détaillé

Le code assembleur généré par la macro est le suivant :

```z80
LD   B, HI(GA_PORT)   ; B = 0x7F (octet haut du port Gate Array)
LD   C, {IMR}         ; C = valeur passée en paramètre (ex: 0x80)
OUT  (C), C           ; Écrit la valeur C dans le port pointé par BC
```

### 3.1. Décorticage instruction par instruction

#### 📍 `LD B, HI(GA_PORT)`
- **Rôle** : Charger la partie haute de l’adresse du port dans le registre `B`.
- **Opérateur `HI()`** : C’est un opérateur spécifique à RASM qui extrait l’octet haut d’une valeur 16 bits.
  - Si `GA_PORT` est défini à `0x7F00`, `HI(GA_PORT)` retourne `0x7F`.
- **Pourquoi `0x7F` ?**  
  Sur Amstrad CPC, l’espace d’adressage I/O est décodé de telle sorte que le Gate Array est accessible lorsque l’octet haut de l’adresse est `0x7F` (bits `A15..A8`).  
  L’octet bas (`A7..A0`) transporte la commande ou la donnée.

#### 📍 `LD C, {IMR}`
- **Rôle** : Placer la valeur du paramètre dans le registre `C`.
- **Substitution `{}`** : En RASM, l’utilisation de `{param}` permet d’injecter la valeur d’un paramètre de macro tel quel, qu’il s’agisse d’une constante immédiate (ex: `0x80`) ou d’une étiquette (ex: `IRQ_STANDARD`).
- **Double rôle de `C`** :
  1. Il contient la **donnée** à écrire.
  2. Il sert d’**octet bas** de l’adresse du port.

#### 📍 `OUT (C), C`
- **Rôle** : Écrire la valeur de `C` (la donnée) dans le port d’adresse `BC` (où `B` = octet haut, `C` = octet bas).
- **Spécificité CPC** :  
  Le Gate Array lit l’octet bas (`C`) comme la valeur à écrire dans ses registres internes.  
  Ainsi, `OUT (C), C` est une instruction **parfaitement adaptée** au CPC pour écrire dans le Gate Array en une seule instruction, sans nécessiter de registre `A` supplémentaire.
- **Effet matériel** : L’écriture dans le registre IMR modifie le mode d’interruption :
  - Bit 7 (valeur `0x80`) = Activation des interruptions standard.
  - Bit 0 (valeur `0x01`) = Mode spécial (souvent inutilisé).
  - Valeur `0x00` = Désactivation des interruptions IRQ.

---

## 4. Algorithme / Séquence logique

```
┌─────────────────────────────┐
│  Début de la macro          │
│  Paramètre : IMR            │
└───────────────┬─────────────┘
                ▼
┌─────────────────────────────────────────┐
│ 1. B = HI(GA_PORT)                     │
│    → Octet haut (0x7F) pour le port GA │
└───────────────┬─────────────────────────┘
                ▼
┌─────────────────────────────────────────┐
│ 2. C = {IMR}                           │
│    → La valeur à écrire dans le GA     │
└───────────────┬─────────────────────────┘
                ▼
┌─────────────────────────────────────────┐
│ 3. OUT (C), C                          │
│    → Écrit C (donnée) dans le port     │
│       dont l'adresse est BC            │
└───────────────┬─────────────────────────┘
                ▼
┌─────────────────────────────┐
│  Fin de la macro            │
│  Carry et flags inchangés   │
└─────────────────────────────┘
```

---

## 5. Dépendances et configuration

Pour que la macro s’assemble correctement, vous **devez** définir `GA_PORT` **avant** toute utilisation :

```z80
; Déclaration obligatoire
GA_PORT EQU 0x7F00
```

> **Note** : Si `GA_PORT` n’est pas défini, l’assembleur générera une erreur de symbole inconnu lors du passage par l’opérateur `HI()`.

---

## 6. Exemples d’utilisation

### 🔹 Activer les interruptions standard (mode le plus courant)
```z80
mGA_SET_MODE 0x80
```

### 🔹 Désactiver les interruptions (mode pollé)
```z80
mGA_SET_MODE 0x00
```

### 🔹 Utiliser une constante nommée
```z80
IMR_STANDARD EQU 0x80
mGA_SET_MODE IMR_STANDARD
```

### 🔹 Intégration dans une routine système
```z80
; Sauvegarde du contexte avant modification
PUSH BC
mGA_SET_MODE 0x80
POP  BC
; ... suite du code
```

---

## 7. Précautions et bonnes pratiques

| Point d’attention | Recommandation |
| :--- | :--- |
| **Destruction de `BC`** | La macro modifie `B` et `C`. Si `BC` contient des données utiles (ex: pointeur, compteur), sauvegardez‑le avec `PUSH BC` avant l’appel. |
| **Valeurs valides pour IMR** | Sur CPC, seuls les bits `0` et `7` sont significatifs. Évitez d’écrire des valeurs inattendues (ex: `0x55`) qui pourraient perturber le comportement de la puce. |
| **Opérateur `HI()`** | Assurez‑vous que votre version de RASM supporte cette syntaxe. Si ce n’est pas le cas, utilisez `GA_PORT >> 8` ou `HIGH GA_PORT` (selon la variante). |
| **Alignement mémoire** | Le paramètre `IMR` peut être une valeur immédiate ou une adresse mémoire (ex: `(HL)`). Dans ce cas, utilisez `{IMR}` pour injecter l’expression brute. |

---

## 8. Variantes possibles

### 8.1. Si `HI()` n’est pas reconnu
```z80
MACRO mGA_SET_MODE IMR
    LD   B, GA_PORT / 256   ; ou >> 8
    LD   C, {IMR}
    OUT  (C), C
ENDM
```

### 8.2. Si l’on souhaite préserver `BC`
On peut ajouter une sauvegarde/restauration directement dans la macro, mais cela alourdit le code :
```z80
MACRO mGA_SET_MODE IMR
    PUSH BC
    LD   B, HI(GA_PORT)
    LD   C, {IMR}
    OUT  (C), C
    POP  BC
ENDM
```

---

## 9. Conclusion

La macro **`mGA_SET_MODE`** est un exemple parfait d’écriture efficace pour l’Amstrad CPC sous RASM :
- Elle utilise un paramètre pour rester générique.
- Elle exploite l’instruction `OUT (C), C`, spécifique et très compacte.
- Elle est minimaliste (3 instructions) et s’intègre facilement dans tout code système.

**Avec elle, le contrôle des interruptions du Gate Array devient une formalité.**

---

## 10. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 1.0 | 2026-08-07 | Analyse technique | Version initiale. Documentation complète du fonctionnement. |

---

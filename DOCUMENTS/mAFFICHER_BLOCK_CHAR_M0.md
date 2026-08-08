
# 📘 Documentation technique : Macro `mAFFICHER_BLOCK_CHAR_M0`

**Auteur** : Patrick MAES Analyse personnalisée – Z80 / Amstrad CPC  
**Version** : 1.0  
**Date** : 2026-08-08  
**Assembleur** : RASM (ou compatible)  

---

## 1. Présentation

La macro **`mAFFICHER_BLOCK_CHAR_M0`** est une surcouche de `mCOPYBLOCK_M0`, spécialisée pour l’affichage d’un caractère défini par un bloc de 16 octets (8 lignes × 2 octets) en Mode 0.  
Elle prend en paramètre l’adresse du bloc source et l’adresse écran de destination, puis appelle `mCOPYBLOCK_M0` pour effectuer la copie.

Cette macro simplifie l’appel à la routine de copie en ne nécessitant qu’une seule ligne de code pour charger les registres et effectuer la copie.

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `mAFFICHER_BLOCK_CHAR_M0` |
| **Type** | Macro (expansion inline) |
| **Paramètres** | `CHAR` : adresse/label des données source du caractère (16 octets)<br>`VRAM` : adresse écran de destination (début de bande) |
| **Entrée** | (via paramètres) |
| **Sortie** | Aucune (le caractère est copié à l’écran) |
| **Registres modifiés** | `AF`, `HL`, `DE` (car utilise `mCOPYBLOCK_M0`) |
| **Taille générée** | ~41 octets (deux `LD` + expansion de `mCOPYBLOCK_M0`) |
| **Prérequis** | • Les paramètres doivent être des expressions valides (labels, constantes)<br>• `VRAM` doit pointer sur une adresse de début de bande (bits 5‑4‑3 de `D` = 0)<br>• La source doit contenir 16 octets valides |
| **Dépendances** | Macro `mCOPYBLOCK_M0` (doit être définie avant) |
| **Compatibilité** | Amstrad CPC, Mode 0 uniquement |

---

## 3. Fonctionnement interne

La macro génère exactement trois instructions :

```z80
LD   HL, {CHAR}      ; charge l'adresse source
LD   DE, {VRAM}      ; charge l'adresse destination
mCOPYBLOCK_M0        ; copie le bloc
```

`{CHAR}` et `{VRAM}` sont substitués par les expressions données en paramètres.  
Le reste du travail est délégué à `mCOPYBLOCK_M0`, qui effectue la copie ligne par ligne.

---

## 4. Code source

```z80
MACRO mAFFICHER_BLOCK_CHAR_M0 CHAR, VRAM
    LD   HL, {CHAR}
    LD   DE, {VRAM}
    mCOPYBLOCK_M0
ENDM
```

---

## 5. Exemple d’utilisation

### 5.1. Affichage simple

```z80
; Afficher le caractère 'A' en haut à gauche
mAFFICHER_BLOCK_CHAR_M0 TABLE_CARACTERES + 32*16, $C000
```

### 5.2. Affichage dans une boucle

```z80
; Afficher une chaîne de caractères
LD   HL, CHAINE_TEXTE
LD   DE, $C000       ; début écran
.boucle:
    LD   A, (HL)
    CP   0
    RET  Z
    PUSH HL
    PUSH DE
    ; Calcul de l'adresse source du caractère
    LD   B, A
    LD   A, 16
    MUL  B            ; A = code_ascii * 16 (si MUL disponible)
    LD   HL, TABLE_CARACTERES
    ADD  A, L
    LD   L, A
    ADC  A, H
    SUB  L
    LD   H, A
    mAFFICHER_BLOCK_CHAR_M0 HL, DE
    POP  DE
    POP  HL
    INC  HL
    LD   A, E
    ADD  A, 2
    LD   E, A
    JR   NC, .boucle
    LD   A, D
    ADD  A, 8
    LD   D, A
    JR   .boucle
```

---

## 6. Optimisations possibles

- **Intégration des calculs d’adresse** : On pourrait ajouter des paramètres `X` et `Y` pour calculer automatiquement `VRAM` à partir des coordonnées, mais cela alourdirait la macro.
- **Conservation des registres** : Si `HL` ou `DE` doivent être préservés, on peut ajouter `PUSH`/`POP` autour de l’appel, mais cela ralentirait l’exécution.

---

## 7. Prérequis et limites

| Point d’attention | Recommandation |
| :--- | :--- |
| **Alignement bande** | `VRAM` doit pointer sur une adresse de début de bande (`D & 0x38 == 0`). |
| **Mode vidéo** | Conçue pour le Mode 0. Pour d’autres modes, il faut adapter `mCOPYBLOCK_M0`. |
| **Source** | La source doit fournir 16 octets valides (8 lignes × 2 octets). |
| **Paramètres** | `CHAR` et `VRAM` doivent être des expressions que RASM peut résoudre (labels, constantes ou registres). |

---

## 8. Conclusion

La macro `mAFFICHER_BLOCK_CHAR_M0` est une **abstraction pratique** pour afficher des caractères graphiques en Mode 0. Elle réduit le code répétitif et améliore la lisibilité du programme tout en conservant les performances de `mCOPYBLOCK_M0`.

Elle s’intègre parfaitement dans des boucles d’affichage de texte ou des routines de rendu de sprites.

---

## 9. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 1.0 | 2026-08-08 | Analyse technique | Création de la documentation. |

---

*Références :*  
- Amstrad CPC System Programming Guide.  
- Macro `mCOPYBLOCK_M0` – documentation associée.


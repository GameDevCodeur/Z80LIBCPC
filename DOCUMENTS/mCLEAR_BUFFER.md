
# `mCLEAR_BUFFER` — Technical Reference

> **Routine de mise à zéro ultra-rapide pour les buffers vidéo Z80**  
> *Version : 1.0 | Statut : Stable*

---

## 1. Abstract

`mCLEAR_BUFFER` est une macro assembleur Z80 conçue pour effacer (mettre à zéro) un buffer mémoire d'une taille fixe de **16 384 octets (`0x4000`)**, typiquement la mémoire écran d'un Amstrad CPC (de `&C000` à `&FFFF`).

Contrairement aux implémentations classiques utilisant un compteur 16 bits (`BC`) et `LDIR`, cette macro exploite **les débordements naturels (`overflow`) des registres 8 bits `L` et `H`** pour itérer sur les 64 pages de 256 octets, éliminant ainsi la gestion lourde d'un compteur explicite. Le gain de performance est d'environ **×1.7** par rapport à `LDIR`.

---

## 2. Interface

| Élément | Spécification |
| :--- | :--- |
| **Prototype** | `MACRO mCLEAR_BUFFER start_address` |
| **Paramètre** | `start_address` : Adresse de départ du buffer à vider. **Doit impérativement avoir son octet haut égal à `&C0`** pour garantir le nettoyage exact des 16 384 premiers octets de la VRAM. Exemple typique : `#C000`. |
| **Entrées** | Aucun registre d'entrée requis. |
| **Sorties** | Tous les octets compris entre `start_address` et `start_address + 0x3FFF` sont mis à `0x00`. |
| **Registres détruits** | `AF` (le registre `A` est mis à zéro) et `HL` (incrémenté jusqu'à `0x0000` après la dernière itération). |

> ⚠️ **Contrainte d'alignement critique** : Pour que la macro efface strictement `0x4000` octets, l'octet haut de `start_address` (`H`) **doit** être `0xC0`. Si une autre valeur est fournie (ex : `0xD0`), la taille effacée sera de `(0x100 - H) * 256` octets.

---

## 3. Principe de fonctionnement & Algorithme

La macro repose sur une double bouche utilisant les débordements de `L` et `H` :

1. **Boucle interne (page de 256 octets)** :  
   Un bloc `REPEAT 8` est déroulé pour écrire 8 octets à la suite, tout en incrémentant `L`. Le `JR NZ` qui suit vérifie si `L` a atteint `0x00` (après 256 incrémentations). Si ce n'est pas le cas, la boucle reprend.

2. **Boucle externe (changement de page)** :  
   Lorsque `L` revient à `0x00` (fin de la page courante), `H` est incrémenté. Si `H` n'est pas à `0x00` (i.e. on n'a pas dépassé la page `0xFF`), la boucle externe reprend pour vider la page suivante.

3. **Condition d'arrêt** :  
   Le processus s'arrête lorsque `H` passe de `0xFF` à `0x00` (débordement). Pour `H = 0xC0`, cela balaye exactement les pages `0xC0` à `0xFF` (soit 64 pages), totalisant `64 × 256 = 16 384` octets.

### Pseudo-code

```pascal
HL = start_address
A = 0

Boucle_Page :
    Répéter 8 fois :
        [HL] = 0
        HL = HL + 1
    Fin Répéter
    
    Si L != 0 : Aller à Boucle_Page
    H = H + 1
    Si H != 0 : Aller à Boucle_Page
Fin Macro
```

---

## 4. Analyse du flux (cas typique : `start_address = &C000`)

| Registre | État initial | Itérations internes | Comportement |
| :--- | :--- | :--- | :--- |
| **`L`** | `0x00` | Incrémenté 8 fois par bloc `REPEAT`, donc 32 blocs pour atteindre `0x00`. | Le `JR NZ` est pris 31 fois, sauté la 32ᵉ fois. |
| **`H`** | `0xC0` | Incrémenté après chaque page complète. | Boucle de `0xC0` à `0xFF` (64 fois), puis `INC H` donne `0x00` et termine. |

**Adresses écrites** :
- Page 0 : `&C000` → `&C0FF` (256 octets)
- Page 1 : `&C100` → `&C1FF`
- ...
- Page 63 : `&FF00` → `&FFFF`

---

## 5. Mesures de performances

### 5.1. Décompte cyclique (en T-states)

| Instruction | Coût unitaire | Occurrences (par page) | Total (par page) |
| :--- | :---: | :---: | :---: |
| `LD (HL), A` | 7 | 256 | 1 792 |
| `INC L` | 4 | 256 | 1 024 |
| `JR NZ, @razScr` (pris) | 12 | 31 | 372 |
| `JR NZ, @razScr` (non pris) | 7 | 1 | 7 |
| **Total par page** | - | - | **3 195 T-states** |
| **Total pour 64 pages** | - | - | **204 480 T-states** |

### 5.2. Comparaison avec `LDIR`

- **Version `LDIR` classique** : `LDIR` coûte **21 T-states par octet**.  
  `16 384 × 21 = 344 064 T-states`.
- **Version macro (`mCLEAR_BUFFER`)** : **204 480 T-states**.

> 🚀 **Rapport d'accélération** : `344 064 / 204 480 ≈ 1.68`.  
> La macro est donc environ **1,7 fois plus rapide** que `LDIR`, ce qui confirme l'affirmation de l'en-tête.

---

## 6. Conditions d'utilisation et contraintes

| Contrainte | Détail |
| :--- | :--- |
| **Alignement mémoire** | L'octet haut de l'adresse (`H`) doit être strictement `0xC0` pour une utilisation sur écran CPC (zone `&C000`). |
| **Dépassement de buffer** | Si `start_address` est inférieur à `&C000` (ex: `&B000`), la macro effacera jusqu'à `&FFFF` (20 480 octets), débordant sur la mémoire système. |
| **Non-réentrance** | La macro modifie `HL` sans le sauvegarder. Si `HL` doit être préservé, un `PUSH HL` / `POP HL` est requis autour de l'appel. |

---

## 7. Limitations et cas de bord

### 7.1. Non-généricité du paramètre `start_address`

Contrairement à ce que son nom laisse suggérer, `start_address` n'est pas une adresse totalement générique pour vider `n` octets.  
La taille effacée est dictée par la valeur de `H` :

> **Taille effacée** = `(0x100 - H_initial) × 256` octets.

Pour que cette taille soit **exactement** `0x4000` (16 384), il est impératif que `H_initial = 0xC0`.

| Si `start_address` = | `H` | Taille effacée | Résultat |
| :--- | :--- | :--- | :--- |
| `&C000` | `0xC0` | `0x4000` (16k) | ✅ Correct |
| `&C050` | `0xC0` | `0x4000` (16k) | ✅ Correct (débute à `&C050` et termine à `&FFFF`, mais les premiers `0x50` octets de la page 0 ne sont pas écrits. L'écran est bien vidé du point de départ jusqu'à la fin de la VRAM). |
| `&D000` | `0xD0` | `0x3000` (12k) | ❌ Incorrect (manque les 4 derniers kilo-octets) |

> 💡 **Recommandation** : Bien que la macro supporte des offsets dans la page `0xC0`, il est plus sûr de toujours fournir une adresse alignée sur 256 octets (`&C000`, `&C100`, etc.) pour un comportement prévisible.

---

## 8. Optimisation du déroulage de boucle

Le `REPEAT 8` a été choisi comme un bon compromis entre la taille du code et la réduction des cycles de branchement.

- **Sans déroulage** (une seule écriture par itération) : La boucle interne `LD (HL),A` / `INC L` / `JR NZ` serait exécutée 256 fois par page, générant 256 branchements, soit `256 × (12 ou 7)` T-states supplémentaires.
- **Avec `REPEAT 8`** : Le nombre de branchements est divisé par 8 (32 branchements par page), réduisant significativement la pénalité des sauts conditionnels.

> 📦 **Taille du code** : La macro génère `8 × 2 + 2 = 18` octets par page ? Non, le code assembleur est fixe. Le `REPEAT 8` déroule localement, générant 8 paires `LD (HL),A` / `INC L` (soit 16 octets de code), plus le `JR` (2 octets) et le `INC H` / `JR` (4 octets). La taille totale du code généré est d'environ **22 octets**, ce qui est remarquablement compact.

---

## 9. Exemple d'utilisation

```asm
; Efface l'écran complet (Mode 0/1/2)
mCLEAR_BUFFER #C000

; Efface seulement la page vidéo &C1xx (256 octets) ? 
; Non, la macro est conçue pour effacer jusqu'à &FFFF.
; Pour effacer une page spécifique, il faut adapter manuellement.
```

---

## 10. Conclusion

`mCLEAR_BUFFER` est une macro élégante et extrêmement rapide pour vider la VRAM standard du CPC. Son astuce de comptage reposant sur les débordements des registres 8 bits en fait une pièce de code particulièrement efficace et compacte.

Elle est parfaitement adaptée aux boucles de jeu nécessitant un effacement rapide de l'écran, à condition de respecter la contrainte d'adresse de départ (`H = &C0`) pour garantir la taille exacte de `16 384` octets.

> 🔧 **Note de conception** : L'absence de compteur `BC` libère le registre `BC` pour d'autres usages, ce qui est un avantage supplémentaire dans les contextes où les registres sont rares.

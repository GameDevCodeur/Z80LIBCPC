
# `mKEY_ANY_REQUESTED` — Technical Reference

> **Routine de détection de frappe clavier ultra-rapide**  
> *Version : 1.0 | Statut : Stable*

---

## 1. Abstract

`mKEY_ANY_REQUESTED` est une macro assembleur Z80 conçue pour déterminer de manière extrêmement efficace si **au moins une touche** de la matrice clavier de l'Amstrad CPC est actuellement enfoncée.

Contrairement à une routine qui vérifierait chaque ligne ou colonne individuellement, cette macro utilise une **accumulation par ET logique (`AND`)** sur les 10 octets de la matrice, permettant de tester la totalité des 80 touches physiques en un seul passage. Elle repose sur l'hypothèse que le buffer `KBD_BUFFER_RAM` a été préalablement rempli par un scan clavier (typiquement via l'ISR IM1).

---

## 2. Interface

| Élément | Spécification |
| :--- | :--- |
| **Prototype** | `MACRO mKEY_ANY_REQUESTED` |
| **Prérequis** | `KBD_BUFFER_RAM` doit être une zone mémoire de **10 octets** contenant l'état scanné du clavier, avec une convention **Active Low** (bit = `0` si la touche est pressée, bit = `1` si la touche est relâchée). |
| **Entrées** | Aucune (lecture directe de la RAM). |
| **Sortie (Flag Z)** | `Z = 1` (ZF actif) : Aucune touche enfoncée (`A == #FF`).<br>`Z = 0` (ZF inactif) : Au moins une touche enfoncée (`A != #FF`). |
| **Registres détruits** | `AF` (le contenu de `A` est écrasé), `B` (décrémenté de 10 à 0), `HL` (incrémenté de 10). |

---

## 3. Contexte : Matrice clavier CPC

Le clavier CPC est organisé en **10 lignes (rows)** et **8 colonnes (columns)**, totalisant 80 emplacements physiques.

- **Lignes** : Sélectionnées par le port `&F6F0` à `&F6FF` (adresse basse).
- **Colonnes** : Lues sur le port `&F6F0` (octet où chaque bit correspond à une colonne).

La convention de lecture est la suivante :

| Bit | Signification |
| :--- | :--- |
| `0` | **Touche enfoncée** (contact fermé) |
| `1` | **Touche relâchée** (contact ouvert) |

Ainsi, une ligne inactive ou non scannée retournera théoriquement `#FF`. Un buffer rempli de `#FF` sur 10 lignes signifie qu'aucune touche n'est appuyée.

---

## 4. Algorithme et flux d'exécution

### 4.1. Pseudo-code

```pascal
HL = adresse du buffer (KBD_BUFFER_RAM)
B = 10
A = #FF                   ; Initialisation à tous les bits à 1

Répéter 10 fois :
    A = A ET (contenu pointé par HL)   ; AND logique
    HL = HL + 1
    B = B - 1
    Si B != 0 : continuer

; Si une touche est pressée, au moins un bit de A est à 0.
; Si aucune touche n'est pressée, tous les bits de A restent à 1.
Comparer A avec #FF pour définir le flag Z.
```

### 4.2. Explication du mécanisme `AND`

L'accumulation par `AND` est cruciale :

- **Initialisation** : `A = #FF` (`1111 1111`).
- **Itération 1** : `A = #FF AND Ligne0`. Si une touche est pressée dans la ligne 0, le bit correspondant dans `A` devient `0`.
- **Itération 2** : `A = A AND Ligne1`. Le(s) bit(s) déjà à `0` dans `A` restent à `0` (car `0 AND X = 0`).
- **Au final** : `A` contient un `0` sur chaque colonne où au moins une touche est pressée (sur n'importe quelle ligne).

> 💡 **Si une touche quelconque est pressée**, le bit correspondant dans `A` est forcé à `0` → `A != #FF` → Flag `NZ`.

> 💡 **Si aucune touche n'est pressée**, chaque ligne vaut `#FF` → `A` reste `#FF` → Flag `Z`.

---

## 5. Mesures de performances

### 5.1. Décompte cyclique (en T-states)

| Instruction | Coût unitaire | Occurrences | Total |
| :--- | :---: | :---: | :---: |
| `LD HL, KBD_BUFFER_RAM` | 10 | 1 | 10 |
| `LD B, 10` | 7 | 1 | 7 |
| `LD A, #FF` | 7 | 1 | 7 |
| `AND (HL)` | 7 | 10 | 70 |
| `INC HL` | 6 | 10 | 60 |
| `DJNZ .KB_ANY_LOOP` (pris) | 13 | 9 | 117 |
| `DJNZ .KB_ANY_LOOP` (non pris) | 8 | 1 | 8 |
| `CP #FF` | 7 | 1 | 7 |
| **Total** | - | - | **286 T-states** |

### 5.2. Latence

À une fréquence de **4 MHz**, l'exécution dure environ **71,5 microsecondes** (`286 / 4 000 000`).

Cette rapidité permet d'appeler cette macro à chaque frame (50 Hz) sans impacter significativement les performances globales, ou même de l'utiliser dans des boucles d'attente très réactives.

---

## 6. Conditions d'utilisation et contraintes

| Contrainte | Détail |
| :--- | :--- |
| **Buffer à jour** | La macro suppose que `KBD_BUFFER_RAM` est rafraîchi régulièrement (par l'ISR IM1 du firmware ou une routine personnalisée). Sans ce rafraîchissement, le résultat est indéterminé. |
| **Convention Active Low** | La logique `AND` + `CP #FF` n'est valable que si une touche pressée est représentée par un `0` bit. C'est le standard sur CPC, mais à vérifier si vous utilisez un scan personnalisé inversé. |
| **Taille du buffer** | Le buffer **doit** faire exactement 10 octets. La macro ne vérifie pas les bornes ; si le buffer est plus petit, elle lira des données mémoire adjacentes, faussant le résultat. |

---

## 7. Cas particuliers

### 7.1. Appui sur deux touches (ou plus)

La macro reste parfaitement fonctionnelle. Si la touche A est pressée sur la ligne 0 (bit 0 à 0) et la touche B sur la ligne 5 (bit 3 à 0), `A` accumulera les `0` sur les bits 0 et 3. Le résultat sera `A != #FF` → `NZ`.

### 7.2. Conflit / Ghosting

Certains claviers peuvent présenter des conflits de matrice (ghosting) où une combinaison de touches produit des `0` inattendus. Dans ce cas, la macro retournera `NZ` (touche pressée) même si physiquement seules certaines touches sont enfoncées, ce qui est un comportement acceptable pour une détection "any key".

---

## 8. Exemple d'utilisation

```asm
; Attendre qu'une touche soit appuyée (boucle active)
.wait_loop
    mKEY_ANY_REQUESTED        ; Teste le buffer
    JR   Z, .wait_loop        ; Si Z (aucune touche), on attend

; --- Ici, au moins une touche est pressée ---
    CALL traiter_la_touche
```

```asm
; Exemple avec gestion du buffer externalisé
    CALL scan_keyboard         ; Routine personnalisée qui remplit KBD_BUFFER_RAM
    mKEY_ANY_REQUESTED
    JR   NZ, key_pressed_handler
```

---

## 9. Comparaison avec d'autres méthodes

| Méthode | Taille code | Temps d'exécution | Précision |
| :--- | :--- | :--- | :--- |
| **`mKEY_ANY_REQUESTED` (AND)** | ~15 octets | **286 T-states** | Détecte toute touche |
| **Boucle sur 10 lignes avec OR** | ~20 octets | ~350 T-states | Similaire |
| **Test direct firmware (KL KEY)** | ~30 octets | Lourd (appel API) | Similaire mais plus lent |
| **LDIR sur buffer** | ~15 octets | 10 × 21 = 210 T-states + comparaison | Nécessite de comparer avec `#FF` |

La méthode `LDIR` serait légèrement plus rapide en théorie (`10 × 21 = 210` + `CP` = 217 T-states), mais elle consomme `BC` et nécessite que le buffer soit contigu. La macro actuelle est un excellent compromis entre vitesse et préservation des registres (elle ne touche pas à `C`/`DE`).

---

## 10. Conclusion

`mKEY_ANY_REQUESTED` est une macro simple, robuste et extrêmement efficace pour détecter une pression sur le clavier CPC. Son utilisation du **ET logique cumulatif** est une astuce de génie qui réduit le test à une seule comparaison finale.

Elle est idéale pour :
- Les écrans de titre (appuyez sur une touche pour continuer).
- Les menus principaux.
- Les boucles de pause / mise en veille.
- Les raccourcis globaux dans les jeux.

> 🔧 **Recommandation** : Associez cette macro à une routine de scan clavier en interruption (IM1) pour obtenir une détection réactive et sans latence, sans consommer de temps processeur dans des boucles de scrutation matérielle.

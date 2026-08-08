
# `mKEY_ANY_REQUESTED_FAST` — Technical Reference

> **Routine de détection de frappe clavier ultra-optimisée (version déroulée)**  
> *Version : 2.0 | Statut : Stable*

---

## 1. Abstract

`mKEY_ANY_REQUESTED_FAST` est une macro assembleur Z80 conçue pour déterminer si **au moins une touche** de la matrice clavier Amstrad CPC est actuellement enfoncée, avec une latence minimale.

Cette version exploite un **déroulage complet de boucle (`unroll`)** sur les 10 lignes du buffer clavier, éliminant ainsi toute gestion de compteur (`B`) et de branchement conditionnel (`DJNZ`). Le résultat est une exécution **~46 % plus rapide** que la version itérative classique, tout en préservant les registres `BC` et `DE` pour d'autres usages critiques.

Elle est idéale pour les boucles d’attente réactives, les menus interactifs et les jeux nécessitant une réponse quasi-instantanée à la pression d’une touche.

---

## 2. Interface

| Élément | Spécification |
| :--- | :--- |
| **Prototype** | `MACRO mKEY_ANY_REQUESTED_FAST` |
| **Prérequis** | `KBD_BUFFER_RAM` : zone mémoire de **10 octets** contenant l’état scanné du clavier, avec convention **Active Low** (bit = `0` si la touche est pressée, `1` si relâchée). La mise à jour de ce buffer doit être assurée par une routine externe (ex : ISR IM1 ou scan manuel). |
| **Entrées** | Aucune (lecture directe du buffer). |
| **Sortie** | **Flag Z** :<br>• `Z = 1` (ZF actif) → aucune touche enfoncée (`A == #FF`).<br>• `Z = 0` (ZF inactif) → au moins une touche enfoncée (`A != #FF`). |
| **Registres détruits** | `AF` (le registre `A` est écrasé) et `HL` (incrémenté de 10 à partir de `KBD_BUFFER_RAM`). |
| **Registres préservés** | `BC`, `DE` (et tous les autres non modifiés). |

---

## 3. Algorithme détaillé

### 3.1. Principe du masque cumulatif par ET logique

La routine utilise une accumulation par **ET bit à bit** (`AND`) sur les 10 octets du buffer. L'initialisation à `#FF` permet de conserver un `1` sur chaque bit tant qu'aucune touche n'est pressée. Dès qu'un bit passe à `0` (touche enfoncée), il reste à `0` pour le reste du parcours.

**Équation logique** :  
`A = #FF AND Ligne0 AND Ligne1 AND ... AND Ligne9`

Si une touche est pressée sur une ligne quelconque, le bit correspondant dans `A` devient `0` ; la valeur finale n'est donc pas `#FF`.

### 3.2. Déroulage complet

La macro déroule les 10 itérations de test sans utiliser de boucle `DJNZ`. Chaque itération est explicite :

```asm
AND  (HL) : INC HL
```

Cette séquence effectue :
- `AND (HL)` : applique le ET logique entre `A` et l'octet pointé par `HL`, résultat dans `A`.
- `INC HL` : pointe sur l'octet suivant.

Les 10 blocs sont identiques, ce qui supprime les 9 branchements conditionnels (`DJNZ`) et la gestion du compteur `B`.

### 3.3. Test final

Après le parcours, `A` contient le masque résultant. L'instruction `CP #FF` compare `A` avec `#FF` et positionne le flag `Z` :
- Si `A = #FF` → toutes les lignes étaient à `#FF` → Z = 1.
- Sinon → Z = 0.

---

## 4. Analyse des performances

### 4.1. Décompte cyclique détaillé (en T-states)

| Instruction | Coût unitaire | Occurrences | Total |
| :--- | :---: | :---: | :---: |
| `LD HL, KBD_BUFFER_RAM` | 10 | 1 | 10 |
| `LD A, #FF` | 7 | 1 | 7 |
| `AND (HL)` | 7 | 10 | 70 |
| `INC HL` | 6 | 10 | 60 |
| `CP #FF` | 7 | 1 | 7 |
| **Total** | - | - | **154 T-states** |

### 4.2. Latence

À la fréquence standard de l'Amstrad CPC (4 MHz) :

- **Temps d'exécution** : `154 / 4 000 000 = 38,5 µs`.
- **En contexte d'interruption (50 Hz)** : cela représente environ **0,19 %** du temps disponible par trame (20 ms), laissant une marge confortable pour d'autres traitements.

### 4.3. Comparaison avec la version itérative (originale)

| Version | T-states | Temps (4 MHz) | Gain |
| :--- | :---: | :---: | :---: |
| **Itérative (DJNZ)** | 286 | 71,5 µs | Référence |
| **Déroulée (V2)** | **154** | **38,5 µs** | **+46 %** |

---

## 5. Avantages et inconvénients

### 5.1. Avantages

| Point | Détail |
| :--- | :--- |
| **Rapidité extrême** | Pas de gestion de boucle, pas de saut conditionnel → temps d'exécution minimal. |
| **Prédictibilité** | Timing fixe (pas de variation liée à la prise ou non de sauts). |
| **Préservation de `BC` et `DE`** | Ces registres restent disponibles pour d'autres usages (critique en assembleur). |
| **Simplicité d'utilisation** | Une ligne de code suffit, aucun paramètre à fournir. |
| **Compact** | Code source très lisible malgré le déroulage. |

### 5.2. Inconvénients

| Point | Détail |
| :--- | :--- |
| **Taille du code généré** | Environ 35 octets (contre ~15 pour la version bouclée). Le surcoût reste négligeable sur 64 Ko. |
| **Non-générique** | Fonctionne uniquement pour un buffer de 10 octets fixe. Si le nombre de lignes change, la macro doit être réécrite. |
| **Dépendance au buffer** | Nécessite que `KBD_BUFFER_RAM` soit correctement initialisé et mis à jour. |

---

## 6. Exemple d'utilisation

### 6.1. Boucle d'attente simple

```asm
; Attendre qu'une touche soit pressée (utilisation classique)
wait_key:
    mKEY_ANY_REQUESTED_FAST
    JR   Z, wait_key      ; Si Z, aucune touche → on boucle
    ; --- Ici, une touche est pressée ---
    CALL traiter_touche
```

### 6.2. Utilisation dans un gestionnaire d'événements

```asm
; Appel périodique (par exemple à chaque VBL)
scan_and_handle:
    CALL maj_buffer_clavier   ; Routine externe qui remplit KBD_BUFFER_RAM
    mKEY_ANY_REQUESTED_FAST
    JR   NZ, key_pressed
    RET
key_pressed:
    ; ... traitement ...
```

---

## 7. Notes d'implémentation et recommandations

- **Nom du buffer** : La macro utilise `KBD_BUFFER_RAM`. Assurez-vous que ce symbole est défini (par exemple `EQU #C000` ou une zone réservée en RAM).
- **Précaution** : Si vous utilisez un scan clavier en interruption (IM1), le buffer est mis à jour automatiquement par le firmware. Vérifiez toutefois que la routine de scan n'utilise pas `HL` ou `A` de manière conflictuelle (elle les sauvegarde normalement).
- **Extension** : Si vous travaillez avec un clavier matriciel différent (ex : 8 lignes), adaptez le nombre de blocs `AND (HL) : INC HL` en conséquence.

---

## 8. Conclusion

`mKEY_ANY_REQUESTED_FAST` est une macro de détection clavier **ultra-rapide, prévisible et économique en registres**. Son déroulage complet en fait la solution de choix pour les applications temps réel où chaque microseconde compte, sans compromettre la lisibilité du code.

Elle s'intègre parfaitement dans une chaîne de développement Z80 pour Amstrad CPC, que ce soit pour des jeux, des démos ou des outils système.

> 💡 **Astuce** : Combinez cette macro avec un buffer mis à jour par interruption pour bénéficier d'une détection « zéro latence » sans scrutation matérielle active.

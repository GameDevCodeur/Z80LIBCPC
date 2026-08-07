
# 📘 Documentation technique : Fonction `F_ZX0_Decompress`

**Auteur** : Analyse personnalisée pour développement Z80 / Amstrad CPC  
**Version** : 1.0  
**Date** : 2026-08-07  
**Assembleur** : RASM (ou compatible)  
**Code source** : Speed-optimized ZX0 decompressor by spke (187 bytes)

---

## 1. Introduction

La fonction **`F_ZX0_Decompress`** est un décompresseur **ultra‑rapide** pour le format de compression **ZX0**, conçu pour les systèmes Z80 (Amstrad CPC, ZX Spectrum, etc.).  
Elle est basée sur le travail d’Einar Saukas et a été optimisée pour la vitesse par spke, tout en restant très compacte (187 octets).  
Elle est environ **5 % plus rapide** que le décompresseur "Turbo" (128 octets) et atteint des performances proches de la version "Mega" (412 octets).

La macro génère du code **inlinable** (si vous l’appelez plusieurs fois, le code est dupliqué) ou peut être utilisée une seule fois en tant que routine. Dans votre code, vous l’appelez via la routine `F_ZX0_DECOMPRESS` qui l’invoque.

---

## 2. Spécifications techniques

| Élément | Détail |
| :--- | :--- |
| **Nom** | `DecompressZX0` |
| **Paramètre** | Aucun (macro sans paramètre) |
| **Entrée** | `HL` = adresse de début des données compressées<br>`DE` = adresse de début de la zone mémoire de destination |
| **Sortie** | Aucune (les données décompressées sont écrites en mémoire) |
| **Registres détruits** | `AF`, `BC`, `DE`, `HL`, `IX` |
| **Registres préservés** | Aucun (tous sont modifiés) |
| **Dépendance** | Aucune (auto‑suffisante) |
| **Taille du code généré** | Environ 187 octets (selon l’expansion de la macro) |
| **Format pris en charge** | ZX0 v2 (support pour les nouvelles versions) |

---

## 3. Principe du format ZX0

Le format ZX0 est un schéma de compression sans perte basé sur le **LZ77** (dictionnaire glissant). Il utilise deux types de blocs :

- **Littéraux** : séquence d’octets non compressés, copiés directement.
- **Matches** : séquence d’octets déjà vus, copiée à partir d’un décalage (offset) et d’une longueur.

Le flux binaire est encodé avec un code **Elias gamma** (nombres entiers encodés en binaire avec des longueurs variables) et utilise des bits de contrôle pour distinguer les types de blocs.

### 3.1. Structure du flux

```
+------------------------------------------------------------------+
| 1 bit : type de bloc                                             |
|   - 0 = Run of literals (suivi d'une longueur Elias gamma)       |
|   - 1 = Match (suivi d'un offset et d'une longueur)              |
+------------------------------------------------------------------+
| Pour un match :                                                  |
|   - Si le bit de contrôle est 0 : le prochain bit indique        |
|     l'offset (0 = réutilisation du dernier offset, 1 = nouvel    |
|     offset encodé sur 7+8 bits).                                 |
|   - Longueur : Elias gamma (avec un décalage pour les petites    |
|     longueurs).                                                  |
+------------------------------------------------------------------+
| Fin de données : offset = 0 (valeur spéciale)                    |
+------------------------------------------------------------------+
```

Le décompresseur lit le flux bit par bit, en utilisant un accumulateur `A` comme réservoir de bits (8 bits). Quand il est vide, il recharge l’octet suivant de `HL`.

---

## 4. Fonctionnement détaillé du code

La macro est organisée en plusieurs sections, chacune correspondant à une étape du décodeur.

### 4.1. Initialisation

```z80
ld      ix, @CopyMatch1
ld      bc, $ffff
ld      (@PrevOffset+1), bc      ; offset par défaut = -1
inc     bc
ld      a, $80
jr      @RunOfLiterals
```

- `IX` est utilisé comme adresse de retour astucieuse (voir plus bas).
- `BC` est initialisé à `$FFFF` pour le décalage par défaut (`-1`) et stocké dans une instruction auto‑modifiée `ld hl, $ffff` (label `@PrevOffset`).  
  Cette technique évite de charger l’offset depuis une variable séparée.
- `A` est mis à `$80` pour simuler un bit de contrôle déjà consommé (un premier bit à 0) et sauter directement à `@RunOfLiterals` pour démarrer la décompression.

### 4.2. Lecture des bits (Elias gamma)

La macro utilise des **boucles inline** (non appelées) pour lire les codes Elias gamma de manière efficace. Elle gère les débordements du réservoir en rechargeant `A` depuis `(HL)`.

```z80
add     a, a                    ; décalage d'un bit
rl      c                       ; construction de la valeur dans BC (pour les longueurs)
...
call    z, @ReloadReadGamma      ; si A devient 0, recharger
```

Deux routines auxiliaires sont définies :
- `@ReloadReadGamma` : recharge `A` depuis `(HL)` et incrémente `HL`, puis continue la lecture.
- `@ReadGammaAligned` : suppose que `A` vient d’être rechargé et continue la lecture.

### 4.3. Gestion des « Runs of Literals »

```z80
@RunOfLiterals:
        inc     c                 ; C = 1 (longueur minimale)
        add     a, a
        jr      nc, @LongerRun
        jr      nz, @CopyLiteral  ; si le bit est 1 et A non nul → court littéral (1 octet)
        ...
```

- Le premier bit indique si la longueur est courte (0) ou longue (1).  
- La longueur est lue via Elias gamma, puis on copie `BC` octets de `(HL)` vers `(DE)` en utilisant `ldir`.

### 4.4. Gestion des Matches

Les matches peuvent être de trois types :
- **Match court** (longueur = 2) : optimisé avec un code direct `ld bc, 2`.
- **Match avec réutilisation du dernier offset** : le bit de contrôle est 0 pour l’offset, on utilise l’offset mémorisé.
- **Match avec nouvel offset** : on lit 7 bits de l’octet bas et 8 bits de l’octet haut (stocké dans `BC` après inversion de bits).

```z80
@ShorterOffsets:
        ld      b, $ff            ; high byte toujours $FF
        ld      c, (hl)
        inc     hl
        rr      c                 ; prépare l'offset
        ld      (@PrevOffset+1), bc
        jr      nc, @LongerMatch  ; si le dernier bit est 0 → longueur > 2
```

L’offset est ensuite utilisé pour calculer l’adresse source : `HL = DE - offset`.  
La copie se fait avec `ldir` après quelques `ldi` pour gérer les chevauchements (car les zones peuvent se chevaucher).

### 4.5. Optimisations spécifiques

- **Auto‑modification** : l’instruction `ld hl, $ffff` (label `@PrevOffset+1`) est modifiée en cours d’exécution pour stocker le dernier offset utilisé. Cela évite une variable en RAM.
- **Push IX comme astuce** : dans la section `@LongerRepMatch`, un `jp nz, @CopyMatch1` est précédé de `push ix` ; `IX` contient l’adresse de `@CopyMatch1`, donc `ret` après un appel factice permet de sauter sans utiliser `jp`.
- **Boucles déroulées** : pour la lecture des codes gamma, des séquences de `add a,a` et `rl c` sont répétées pour minimiser les branchements.
- **Gestion du cas `A=0`** : quand le réservoir est vide, on recharge via `call z, @ReloadReadGamma` qui est optimisé pour être appelé uniquement quand nécessaire.

### 4.6. Fin de décompression

La fin est détectée quand un offset nul est rencontré dans les données (`ret z` après `inc c` dans `@ProcessOffset`). Le décompresseur s’arrête alors.

---

## 5. Dépendances et configuration

Aucune configuration particulière n’est nécessaire. La macro est auto‑suffisante.

Cependant, elle utilise des **labels locaux** (préfixés par `@`) qui doivent être uniques par expansion (c’est le cas dans RASM). Si vous appelez la macro plusieurs fois, chaque expansion génère ses propres labels, évitant les conflits.

---

## 6. Exemples d’utilisation

### 6.1. Appel direct (depuis votre code)

```z80
; Données compressées situées à ZX0_DATA
; Destination en RAM à DEST_ADDR
ld   hl, ZX0_DATA
ld   de, DEST_ADDR
DecompressZX0
; Ici, les données sont décompressées.
```

### 6.2. Via une routine wrapper

```z80
F_ZX0_DECOMPRESS:
    DecompressZX0
    ret
```

Cette routine peut être appelée avec `HL` et `DE` pré‑chargés.

---

## 7. Précautions et bonnes pratiques

| Point d’attention | Recommandation |
| :--- | :--- |
| **Registres détruits** | Tous les registres (`AF`, `BC`, `DE`, `HL`, `IX`) sont modifiés. Sauvegardez‑les si nécessaire. |
| **Taille du code** | La macro génère environ 187 octets. Si vous l’appelez plusieurs fois, la taille du code augmente (duplication). Pour économiser de la place, définissez la macro une seule fois comme routine et appelez‑la via `CALL`. |
| **Compatibilité** | Le code est prévu pour le format ZX0 v2. Assurez‑vous que vos données compressées utilisent ce format. |
| **Auto‑modification** | La macro modifie son propre code (l’instruction `ld hl, $ffff`). Cela nécessite que le code soit en RAM (pas en ROM). |
| **Dépassement de mémoire** | Assurez‑vous que la zone de destination est suffisamment grande pour recevoir les données décompressées. |
| **Chevauchement source/destination** | Le code gère les chevauchements (car il copie avec `ldir` dans l’ordre croissant). Cependant, l’offset peut être inférieur à la longueur, mais `ldir` les gère correctement tant que l’offset est supérieur à la taille de la copie (cas des séquences répétées). Le décompresseur ZX0 gère cela. |

---

## 8. Analyse des performances

Le décompresseur est optimisé pour la **vitesse** :
- Utilisation de `ldir` pour les copies, qui est rapide sur Z80.
- Lecture de bits avec boucles déroulées pour réduire les branchements.
- Gestion des cas courts (longueur=2) en évitant la lecture de longueur.
- Appels conditionnels avec `call z` qui ne sont exécutés que lorsque nécessaire.

Comparé au décompresseur "Turbo" (128 octets), il est **5 % plus rapide** et quasi aussi rapide que la version "Mega" (412 octets) tout en étant beaucoup plus compact.

---

## 9. Variantes et évolutions possibles

- **Version 16 bits** : non nécessaire car le décompresseur fonctionne en 8 bits.
- **Optimisation mémoire** : on peut stocker le code en ROM et la table des offsets en RAM pour économiser de la RAM, mais cela ralentirait.
- **Gestion des erreurs** : aucune vérification n’est faite ; les données doivent être valides.

---

## 10. Conclusion

La macro **`DecompressZX0`** est un décompresseur exceptionnel pour le format ZX0, alliant rapidité et compacité. Son code astucieux et optimisé en fait un choix idéal pour les jeux et démos sur Z80, où la vitesse de décompression est cruciale.

Elle s’intègre facilement dans tout projet et ne nécessite aucune bibliothèque externe.

---

## 11. Historique des versions

| Version | Date | Auteur | Changements |
| :--- | :--- | :--- | :--- |
| 1.0 | 2026-08-07 | Patrick MAES | Documentation complète du fonctionnement. |

---

*Références :*  
- ZX0 original par Einar Saukas : https://github.com/einar-saukas/ZX0  
- Code source adapté par spke (187 bytes) et uniabis pour le support v2.
````


# Analyse technique — `mBLIT_LINE_HL`

## 1. Présentation

### Macro

```asm
mBLIT_LINE_HL NB, AVANCE, TRANSPARENT
```

### Fonction

`mBLIT_LINE_HL` réalise le transfert d'une ligne de données graphiques depuis une source mémoire vers la VRAM du **CPC**, avec prise en charge de deux modes :

* **Mode opaque** : tous les octets sont copiés.
* **Mode transparent** : les octets `#00` ne sont pas écrits.

La macro peut également assurer automatiquement le passage à la **ligne VRAM suivante** grâce au paramètre `AVANCE`.

---

# 2. Paramètres

| Paramètre     | Type      | Description                                      |
| ------------- | --------- | ------------------------------------------------ |
| `NB`          | Constante | Nombre d'octets à transférer                     |
| `AVANCE`      | `0/1`     | Passage ou non à la ligne VRAM suivante          |
| `TRANSPARENT` | `0/1`     | Active ou non le traitement du `#00` transparent |

### `NB`

Définit la largeur de la ligne en octets.

Exemple :

```asm
mBLIT_LINE_HL 4,1,0
```

transfère exactement **4 octets**.

---

### `AVANCE`

| Valeur | Comportement                                                      |
| -----: | ----------------------------------------------------------------- |
|    `0` | `DE` reste après la zone copiée                                   |
|    `1` | Retour au début de la ligne puis passage à la ligne VRAM suivante |

Lorsque `AVANCE=1`, la macro utilise :

```asm
mREWIND_DE NB
mSET_NEXTLINE_DE
```

---

### `TRANSPARENT`

| Valeur | Mode        |
| -----: | ----------- |
|    `0` | Opaque      |
|    `1` | Transparent |

En mode transparent :

```text
#00 = transparent
#01-#FF = pixel écrit
```

---

# 3. Registres

## Entrées

```text
HL = adresse source
DE = adresse destination VRAM
```

## Sorties

### `HL`

Toujours avancé de `NB` octets :

```text
HL_final = HL_initial + NB
```

### `DE`

Si :

```asm
AVANCE = 0
```

alors :

```text
DE_final = DE_initial + NB
```

Si :

```asm
AVANCE = 1
```

alors :

```text
DE_final = début de la ligne VRAM suivante
```

---

# 4. Mode opaque

Lorsque :

```asm
TRANSPARENT = 0
```

la macro utilise `LDI`.

```asm
REPEAT {NB}
    LDI
REND
```

## Fonctionnement de `LDI`

Chaque `LDI` réalise :

```text
(HL) → (DE)
HL   → HL + 1
DE   → DE + 1
BC   → BC - 1
```

C'est donc la solution privilégiée pour un transfert direct.

### Avantages

* Très compact.
* Rapide.
* Aucun test de pixel.
* Particulièrement adapté au rendu CPC.

### Inconvénient

`BC` est modifié.

La macro ne doit donc pas être utilisée dans un contexte où `BC` doit être conservé sans sauvegarde préalable.

---

# 5. Mode transparent

Lorsque :

```asm
TRANSPARENT = 1
```

chaque octet est testé avant écriture.

```asm
LD   A,(HL)
INC  HL

OR   A
JR   Z,@skip

LD   (DE),A

@skip:
INC  DE
```

## Fonctionnement

```text
Source       Action
--------------------------------
#00          aucune écriture
#01          écriture
#02          écriture
...
#FF          écriture
```

Le pointeur `DE` avance toujours.

C'est essentiel : même lorsqu'un pixel est transparent, il occupe une position dans la ligne.

---

# 6. Comparaison des deux modes

| Opération               |      Opaque |         Transparent |
| ----------------------- | ----------: | ------------------: |
| Lecture source          |       `LDI` |         `LD A,(HL)` |
| Test `#00`              |         Non |                 Oui |
| Écriture conditionnelle |         Non |                 Oui |
| `HL++`                  |         Oui |                 Oui |
| `DE++`                  |         Oui |                 Oui |
| `BC--`                  |         Oui |                 Non |
| Vitesse                 | Très rapide |          Plus lente |
| Usage                   | Fond opaque | Sprites avec masque |

---

# 7. Passage à la ligne suivante

Lorsque :

```asm
AVANCE = 1
```

après le transfert :

```text
DE
│
│ NB octets copiés
▼
[ FIN DE LIGNE ]
```

`DE` doit revenir au début de la ligne avant d'appliquer le déplacement vertical CPC.

La macro utilise :

```asm
mREWIND_DE NB
mSET_NEXTLINE_DE
```

Conceptuellement :

```text
DE = DE + NB
        │
        ▼
   mREWIND_DE
        │
        ▼
DE = début ligne
        │
        ▼
mSET_NEXTLINE_DE
        │
        ▼
DE = ligne VRAM suivante
```

---

# 8. Pourquoi `mREWIND_DE` est optimisé

Pour les petites valeurs de `NB`, plusieurs `DEC DE` sont plus simples :

```asm
DEC DE
DEC DE
DEC DE
DEC DE
```

Pour une largeur plus importante, le calcul est réalisé avec `ADD HL,BC`.

```asm
LD   BC,-NB
EX   DE,HL
ADD  HL,BC
EX   DE,HL
```

La macro choisit automatiquement la meilleure méthode selon `NB`.

---

# 9. Flux général

```text
              ┌──────────────────────┐
              │ mBLIT_LINE_HL        │
              │ NB, AVANCE, TRANS... │
              └──────────┬───────────┘
                         │
                 TRANSPARENT ?
                    /          \
                  NON           OUI
                   │             │
                   ▼             ▼
                 LDI       LD A,(HL)
                   │             │
                   │          OR A
                   │          /   \
                   │        #00   ≠ #00
                   │         │       │
                   │         │       ▼
                   │         │    LD (DE),A
                   │         │       │
                   │         └───┬───┘
                   │             │
                   └──────┬──────┘
                          │
                         DE++
                          │
                          ▼
                    AVANCE = 1 ?
                      /       \
                    NON       OUI
                     │          │
                     ▼          ▼
                    FIN    mREWIND_DE
                                │
                                ▼
                         mSET_NEXTLINE_DE
                                │
                                ▼
                               FIN
```

---

# 10. Exemple — sprite opaque

```asm
mBLIT_LINE_HL 4,1,0
```

Pour :

```text
HL = SOURCE
DE = VRAM
```

la macro effectue :

```asm
LDI
LDI
LDI
LDI
```

Puis :

```asm
mREWIND_DE 4
mSET_NEXTLINE_DE
```

Le résultat est :

```text
HL = SOURCE + 4
DE = ligne VRAM suivante
```

---

# 11. Exemple — sprite transparent

```asm
mBLIT_LINE_HL 4,1,1
```

Pour une source :

```text
#12 #00 #34 #00
```

le résultat est :

```text
VRAM :
#12 [inchangé] #34 [inchangé]
```

Mais `DE` avance bien de quatre positions.

---

# 12. Cas `AVANCE=0`

Exemple :

```asm
mBLIT_LINE_HL 4,0,0
```

Le transfert est effectué :

```text
Source :  A B C D
           ↓ ↓ ↓ ↓
VRAM   :  A B C D
```

et :

```text
HL = HL_initial + 4
DE = DE_initial + 4
```

Aucun déplacement vertical n'est effectué.

---

# 13. Intérêt architectural

Cette macro permet de centraliser trois responsabilités :

1. **Transfert horizontal**
2. **Gestion de la transparence**
3. **Progression verticale dans la VRAM CPC**

Le code appelant reste donc très compact :

```asm
mBLIT_LINE_HL BALL_WIDTH,1,1
```

ou :

```asm
mBLIT_LINE_HL SPRITE_WIDTH,1,0
```

La décision d'optimisation est prise **à l'assemblage**, et non à l'exécution.

Il n'y a donc aucun test runtime du type :

```asm
CP ...
JR ...
```

pour déterminer le mode.

---

# 14. Bilan

### Points forts

* **Macro entièrement spécialisée à l'assemblage.**
* Deux chemins distincts opaque / transparent.
* `LDI` utilisé pour le transfert opaque rapide.
* Aucun test de transparence en mode opaque.
* Gestion automatique du déplacement vertical.
* `mREWIND_DE` optimisé selon `NB`.
* `HL` et `DE` avancent naturellement pendant le transfert.
* Très adaptée au rendu de sprites CPC Mode 0.

### Coûts

Le mode transparent est nécessairement plus coûteux car chaque octet nécessite :

```asm
LD A,(HL)
INC HL
OR A
JR Z,...
```

alors que le mode opaque peut utiliser directement :

```asm
LDI
```

### Architecture recommandée

```text
mBLIT_LINE_HL
       │
       ├── OPAQUE
       │     └── LDI × NB
       │
       └── TRANSPARENT
             └── test #00 × NB
       
       │
       ▼
   AVANCE ?
       │
       ▼
mREWIND_DE
       │
       ▼
mSET_NEXTLINE_DE
```

**Conclusion :** `mBLIT_LINE_HL` constitue une bonne primitive bas niveau pour un moteur de sprites CPC. 
La spécialisation par macro permet d'obtenir du code final compact et rapide, tout en conservant une interface d'appel très simple.

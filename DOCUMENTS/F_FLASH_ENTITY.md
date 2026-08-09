
markdown_content = """# F_FLASH_ENTITY

## Routine de gestion du clignotement Gate Array (Amstrad CPC)

---

## Métadonnées

| Attribut | Valeur |
|----------|--------|
| **Architecture** | Zilog Z80 (@ 4 MHz) |
| **Plateforme** | Amstrad CPC (série 464/664/6128) |
| **Registre cible** | Gate Array (#7Fxx) |
| **Taille** | **28 octets** |
| **Cycles (chemin critique)** | ~160 T-states (flash exécuté) |
| **Cycles (sortie rapide)** | 31 T-states |
| **Pile** | Aucune utilisation |

---

## Description

`F_FLASH_ENTITY` gère le clignotement périodique d'un stylo (pen) sur Amstrad CPC. Elle implémente un **compteur décroissant** avec recharge automatique et **bascule d'état binaire** (0/1) pilotant l'alternance entre deux couleurs (ColorA / ColorB).

Le mécanisme repose sur une structure de données de 6 octets en RAM. La routine est conçue pour être appelée à chaque frame (VBL) ou à une fréquence fixe par le moteur de jeu.

---

## Structure de données (`FLASH_ENTITY`)

```text
Offset   Nom            Type      Description
─────────────────────────────────────────────────────────────
+0       Counter        uint8_t   Compteur courant (décrémenté)
+1       CounterValue   uint8_t   Valeur de recharge (période)
+2       State          uint8_t   État courant (0 = ColorA, 1 = ColorB)
+3       Pen            uint8_t   Stylo Gate Array (#40 | n°_stylo)
+4       ColorA         uint8_t   Couleur état 0 (format Gate Array)
+5       ColorB         uint8_t   Couleur état 1 (format Gate Array)
─────────────────────────────────────────────────────────────
Taille totale : 6 octets
```

### Convention Gate Array

Le Gate Array du CPC est sélectionné lorsque **A15=0, A14=1, A10=1**. Le port physique est donc `#7Fxx` (tout `xx` est valide). La routine exploite cette propriété en fixant `B = #7F` et en utilisant `OUT (C), reg` pour éviter de recharger le port à chaque écriture.

| Commande | Format | Exemple |
|----------|--------|---------|
| Sélection stylo | `#4n` | `#42` = stylo 2 |
| Couleur | `#nn` | `#1A` = couleur 26 (rouge vif) |

---

## Interface

### Entrées

| Registre | Description |
|----------|-------------|
| `HL` | Adresse de `Counter` (offset +0 de la structure) |
| `B` | Doit valoir `#7F` (port Gate Array) à l'appel |

### Sorties

| Registre | Description |
|----------|-------------|
| `Counter` | Décrémenté, ou rechargé si arrivé à 0 |
| `State` | Inversé (0↔1) lorsque le compteur atteint 0 |
| `Pen` | Sélectionné sur le Gate Array |
| `ColorA/B` | Écrite sur le Gate Array selon `State` |
| `HL` | `HL + 5` si le flash a été exécuté, inchangé sinon |

### Registres détruits

```
AF, B
```

### Registres préservés

```
DE, IX, IY
```

---

## Code source commenté

```asm
F_FLASH_ENTITY

    ; ─── Test compteur ───────────────────────────────────
    DEC  (HL)                    ; Counter--
    RET  NZ                      ; Pas encore 0 : sortie immédiate

    ; ─── Recharge compteur ─────────────────────────────
    INC  HL
    LD   A, (HL)                 ; A = CounterValue
    DEC  HL
    LD   (HL), A                 ; Counter = CounterValue

    ; ─── Bascule d'état ────────────────────────────────
    INC  HL
    INC  HL                      ; HL = State (+2)

    LD   A, (HL)
    XOR  1                       ; Inverse le bit 0 (0 <-> 1)
    LD   (HL), A

    RRA                          ; Carry = nouvel état
                                 ; Carry=0 → ColorA, Carry=1 → ColorB

    ; ─── Sélection stylo Gate Array ────────────────────
    INC  HL                      ; HL = Pen (+3)

    LD   B, HI(GA_PORT)
    LD   A,(HL)                  ; A = Pen
    OUT  (C),A                   ; sélection du stylo

    ; ─── Sélection couleur ─────────────────────────────
    INC  HL                      ; HL = ColorA (+4)

    LD   A, (HL)                 ; Charge ColorA (spéculatif)
    JR   NC, .WRITE              ; État 0 : ColorA confirmée

    INC  HL                      ; HL = ColorB (+5)
    LD   A, (HL)                 ; État 1 : ColorB

.WRITE
    OUT  (C), A                  ; Écrit la couleur active
    RET
```

---

## Analyse du flux d'exécution

```
Appel (HL = &Counter)
    │
    ▼
┌─────────────┐
│  DEC (HL)   │───[Counter ≠ 0]──► RET (31 T-states, sortie rapide)
└─────────────┘
    │
    ▼ [Counter = 0]
┌─────────────┐
│   Reload    │  Counter = CounterValue
│   Toggle    │  State = State XOR 1
│   RRA       │  Carry = nouvel état
└─────────────┘
    │
    ▼
┌─────────────┐
│  OUT (C),A  │  Sélection stylo (Pen)
└─────────────┘
    │
    ▼
┌─────────────┐
│  LD A,Color │  Spéculatif : charge ColorA
│  JR NC      │  Test Carry
└─────────────┘
    │           │
    ▼ Carry=0   ▼ Carry=1
┌─────────┐   ┌─────────┐
│ ColorA  │   │ ColorB  │
│ confirmé│   │ chargé  │
└────┬────┘   └────┬────┘
     │             │
     └──────┬──────┘
            ▼
     ┌────────────┐
     │ OUT (C),A  │  Écriture couleur Gate Array
     │    RET     │
     └────────────┘
```

---

## Tableau des cycles (T-states)

| Phase | Instruction | T-states | Cumul |
|-------|-------------|----------|-------|
| **Sortie rapide** | `DEC (HL)` | 11 | 11 |
| | `RET NZ` (pris) | 20 | **31** |
| **Recharge** | `DEC (HL)` | 11 | 11 |
| | `RET NZ` (non pris) | 11 | 22 |
| | `INC HL` | 6 | 28 |
| | `LD A,(HL)` | 7 | 35 |
| | `DEC HL` | 6 | 41 |
| | `LD (HL),A` | 7 | 48 |
| **Toggle** | `INC HL` | 6 | 54 |
| | `INC HL` | 6 | 60 |
| | `LD A,(HL)` | 7 | 67 |
| | `XOR 1` | 7 | 74 |
| | `LD (HL),A` | 7 | 81 |
| | `RRA` | 4 | 85 |
| **Stylo** | `INC HL` | 6 | 91 |
| | `LD B,#7F` | 7 | 98 |
| | `LD A,(HL)` | 7 | 105 |
| | `OUT (C),A` | 12 | 117 |
| **Couleur (état 0)** | `INC HL` | 6 | 123 |
| | `LD A,(HL)` | 7 | 130 |
| | `JR NC` (pris) | 12 | 142 |
| | `OUT (C),A` | 12 | 154 |
| | `RET` | 10 | **164** |
| **Couleur (état 1)** | `INC HL` | 6 | 123 |
| | `LD A,(HL)` | 7 | 130 |
| | `JR NC` (non pris) | 7 | 137 |
| | `INC HL` | 6 | 143 |
| | `LD A,(HL)` | 7 | 150 |
| | `OUT (C),A` | 12 | 162 |
| | `RET` | 10 | **172** |

---

## Points techniques

### 1. Chargement spéculatif de ColorA

Avant de connaître l'état final ( Carry après `RRA` ), la routine charge systématiquement `ColorA`. Si `Carry = 0`, le `JR NC` saute directement à l'écriture. Si `Carry = 1`, deux instructions supplémentaires chargent `ColorB`.

C'est un **branch prediction hardware du Z80** : l'état 0 est légèrement privilégié (9 T-states de moins). Si vos entités passent plus de temps dans un état que l'autre, placez la couleur dominante en `ColorA`.

### 2. Absence de pile

Aucun `PUSH`/`POP` n'est utilisé. La routine est **entièrement réentrante** et peut être appelée depuis une interruption (IM1/IM2) sans risque de corruption de pile.

---

## Utilisation typique

```asm
    ; Initialisation d'une entité clignotante
    LD   HL, ENTITY_PLAYER_PEN
    LD   (HL), 25                ; Counter = 25 frames
    INC  HL
    LD   (HL), 25                ; CounterValue = 25
    INC  HL
    LD   (HL), 0                 ; State = 0 (ColorA)
    INC  HL
    LD   (HL), #4A               ; Pen = 10 (stylo 10)
    INC  HL
    LD   (HL), #1A               ; ColorA = rouge vif
    INC  HL
    LD   (HL), #00               ; ColorB = noir

    ; Dans la boucle principale (VBL)
MAIN_LOOP:
    LD   B, #7F                  ; Port Gate Array
    LD   HL, ENTITY_PLAYER_PEN
    CALL F_UPDATE_FLASH_ENTITY
    
    LD   HL, ENTITY_ENEMY_PEN
    CALL F_UPDATE_FLASH_ENTITY
    
    ; ... autres entités
    
    HALT                         ; Attente VBL
    JR   MAIN_LOOP
```

---

## Limites et extensions

| Limite | Valeur | Contournement |
|--------|--------|---------------|
| Période max | 255 frames | Chainer deux compteurs ou utiliser un facteur de décimation externe |
| États | 2 seulement | Remplacer `XOR 1` par `INC A / CP 4 / JR NZ` pour un cycle de 4 couleurs |
| Stylo unique | 1 par entité | Créer plusieurs entités pour clignoter plusieurs stylos |

---

## Résumé

```
┌─────────────────────────────────────────┐
│  F_FLASH_ENTITY                  │
│  28 octets · 31-172 T-states · 0 pile   │
│                                         │
│  + Compteur décroissant avec reload     │
│  + Toggle binaire 0/1                   │
│  + Double OUT Gate Array optimisé       │
│  + Sortie rapide si compteur actif      │
└─────────────────────────────────────────┘
```


### Document de Présentation Projet : PONG Z80 (Amstrad CPC)

> **Plateforme Cible :** Amstrad CPC (464 / 664 / 6128)  
> **Langage :** Assembleur Z80  
> **Résolution :** Mode 0 Custom ($128 \\times 192$ pixels, 16 couleurs)  
> **Auteur :** DEVZ80CPC - Patrick MAES
> **Moteur :** Z80LIB Moteur Physics & Graphics Engine 2D

---

## 📋 Fiche d'Identité du Projet

| Élément | Spécification Technique |
| :--- | :--- |
| **Nom du Projet** | PONG Z80 (Mode 0 Custom) |
| **Plateforme Cible** | Amstrad CPC 464 / 664 / 6128 |
| **Langage & Toolchain** | Assembleur Z80 (RASM / SJASMPLUS) |
| **Mode d'Affichage** | Mode 0 Custom ($128 \\times 192$ pixels, 16 couleurs) |
| **Consommation VRAM** | 12 Ko (Économie de 25% par rapport au $160 \\times 200$ standard) |
| **Cadencement Frame Rate** | 50 Hz constants (Synchronisation VBL / CRTC 6845) |

---

## 🎯 Objectifs & Valeur Ajoutée

* **Performance Matérielle :** Exploitation directe des registres du CRTC 6845 pour recentrer l'image et libérer des ressources mémoire.
* **Moteur Physique sur Mesure :** Implémentation d'une détection de collision AABB 8 bits à chevauchement strict pour éliminer les bugs d'encastrement des sprites.
* **Identité Visuelle Rétro :** Utilisation optimale de la palette 16 couleurs du Mode 0 pour offrir un rendu vivant avec des pixels $2 \\times 1$.
* **Compatibilité Totale :** Exécution garantie sur matériel d'origine (100% compatible Amstrad CPC 464 sans extension RAM).

---

## 🛠️ Architecture Technique

* **Gestion Graphique (CRTC) :** Reconfiguration des registres ($R1=32$, $R6=24$) pour un affichage fenêtré $128 \\times 192$ pixels.
* **Système d'Entrées :** Prise en charge simultanée du clavier (touches QA / OP) et du joystick via l'interrogation directe du PPI 8255 et du PSG AY-3-8912.
* **Effets Sonores :** Bips de collision et de point marqué générés en temps réel par la puce audio AY-3-8912.

---

## 🗓️ Planning & Livrables

| Phase | Intitulé | Livrables & Objectifs |
| :--- | :--- | :--- |
| **Phase 1** | **Scène Titre** | Affichage d'une image de titre 3 secondes. |
| **Phase 2** | **Scène Menu** | Affichage du Menu avec Text Clignotant. |
| **Phase 3** | **Scène Game** | Intégration du GamePlay d'une partie. |
| **Phase 4** | **Scène GameOver** | Intégration de la fin de la partie. |
"""

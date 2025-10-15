# 🎮 Motus Game

Un jeu de lettres inspiré du célèbre jeu télévisé Motus, développé en C avec la bibliothèque SDL2.

![Motus Game](https://img.shields.io/badge/Language-C-blue.svg)
![SDL2](https://img.shields.io/badge/Library-SDL2-green.svg)

## 📝 Description

Ce projet est une implémentation du jeu Motus en langage C. Le joueur doit deviner un mot en un nombre limité d'essais. Chaque tentative donne des indices visuels :
- Lettres correctes et bien placées
- Lettres correctes mais mal placées
- Lettres absentes du mot

Le jeu utilise SDL2 pour l'interface graphique et SDL2_ttf pour le rendu du texte.

## 🛠️ Technologies utilisées

- **Langage** : C
- **Bibliothèques** :
  - SDL2 (Simple DirectMedia Layer) - Interface graphique
  - SDL2_ttf - Rendu de texte
- **Build System** : Code::Blocks

## 📋 Prérequis

Pour compiler et exécuter ce projet, vous aurez besoin de :

- Un compilateur C (GCC recommandé)
- SDL2
- SDL2_ttf
- Code::Blocks (optionnel, pour utiliser le fichier projet)

### Installation des dépendances

**Sur macOS (avec Homebrew) :**
```bash
brew install sdl2 sdl2_ttf
```

**Sur Ubuntu/Debian :**
```bash
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```

**Sur Windows :**
Téléchargez les bibliothèques SDL2 depuis [libsdl.org](https://www.libsdl.org/download-2.0.php)

## 🚀 Installation et lancement

### Compilation manuelle

```bash
# Compiler tous les fichiers source
gcc -o motus main.c common.c game.c render.c utils.c -lSDL2 -lSDL2_ttf

# Lancer le jeu
./motus
```

### Avec Code::Blocks

1. Ouvrez le fichier `Motus.cbp` avec Code::Blocks
2. Appuyez sur F9 pour compiler et exécuter

## 📁 Structure du projet

```
.
├── main.c          # Point d'entrée du programme
├── common.c/h      # Fonctions communes et structures
├── game.c/h        # Logique du jeu
├── render.c/h      # Rendu graphique SDL2
├── utils.c/h       # Fonctions utilitaires
├── dictionary.txt  # Dictionnaire de mots
├── scores.txt      # Sauvegarde des scores
├── save.txt        # Sauvegarde de partie
└── font.ttf        # Police de caractères
```

## 🎯 Fonctionnalités

- ✅ Interface graphique avec SDL2
- ✅ Système de score
- ✅ Sauvegarde de partie
- ✅ Dictionnaire de mots
- ✅ Indices visuels pour chaque tentative
- ✅ Documentation Doxygen

## 📚 Documentation

La documentation complète du code est disponible dans le dossier `doxygen/` et peut être générée avec Doxygen.

## 👨‍💻 Auteur

**Yassine Badaoui** - [@ez1kl](https://github.com/ez1kl)

Projet réalisé dans le cadre du Semestre 1 - Langage C avancé - ESGI Aix-en-Provence

## 📄 Remarques

Ce projet a été développé à des fins éducatives dans le cadre de ma formation à l'ESGI.

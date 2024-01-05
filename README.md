# "License to kill" project 

![The monitor program](./doc/figures/spies_monitor.png)

## Description

Ce projet est une simulation complexe d'un réseau d'espionnage opérant dans une ville fictive. 
Il met en œuvre une application multiprocessus utilisant diverses techniques de programmation en C 
pour simuler des interactions dynamiques entre des agents d'espionnage, des officiers de contre-espionnage, et des 
citoyens ordinaires. La simulation intègre des mécanismes tels que la communication interprocessus, 
la synchronisation des threads, le partage de mémoire, et la gestion des signaux.

## Fonctionnalités

* Simulation de différents personnages (espions, officiers de contre-espionnage, citoyens) se déplaçant dans une ville.
* Gestion des interactions entre personnages, y compris le suivi, l'espionnage et les tentatives d'évasion.
* Utilisation de la mémoire partagée pour le suivi de l'état de la simulation.
* Communication entre processus à l'aide de files de messages et de sémaphores.

## Prérequis

* GCC (GNU Compiler Collection) pour la compilation du code C. 
* Système d'exploitation de type UNIX (Linux, macOS).
* Bibliothèque ncurses pour l'interface utilisateur du moniteur.

## Installation

Pour une installation et execution rapide, utilisez la commande suivante :

```bash
make run
```

### Compiler le projet 

* Clonez le dépôt Git ou téléchargez les sources du projet.
* Accédez au répertoire racine du projet.
* Pour compiler le projet, utilisez la commande suivante :
```bash
make all
```

### Exécution

Après la compilation, plusieurs exécutables seront générés. Voici comment les exécuter :

Lancez le processus principal de simulation :

```bash
./main_program
```


### Nettoyage

Pour nettoyer les fichiers compilés, utilisez la commande ```make distclean``` :

```bash
make distclean
```

## Documentation

La documentation du projet est disponible dans le répertoire [doc](./doc/).

Vous pouvez aussi générer la documentation à partir des sources du projet en utilisant la commande suivante :

```bash
make doc
```

## Auteurs

* Clément Jantet <clement[dot]jantet[at]ecole[dot]ensicaen[dot]fr>
* Calliste Ravix <calliste[dot]ravix[at]ecole[dot]ensicaen[dot]fr>
* Guillain Le Goff <guillain[dot]le-goff[at]ecole[dot]ensicaen[dot]fr>
* Maxime Michel <maxime[dot]michel[at]ecole[dot]ensicaen[dot]fr>


## Licence

Ce projet est sous licence Apache 2.0. Voir le fichier LICENSE pour plus de détails.
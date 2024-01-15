# Bataille navale
Dans ce dépôt, vous retrouverez le codes en langage C pour le TP3.

Ce logiciel est un jeu de bataille navale. 

N.B. Dans cette version, le joueur doit choisir la taille de son plateau de jeu. Cette taille doit être entre 6 et 50 puisqu'il doit tout de même y avoir assez de place sur le plateau pour les six navires de tailles diverses et que ce tableau doit pouvoir être affiché sur un écran de taille moyenne.

## Fichiers :
Voici les fichiers contenus dans ce dépot :

* `README.md` : Ce fichier.
* `batnav.c` : Le code source non-compilé.
* `Makefile` : Makefile pour automatiser la compilation avec `Make`.
* `.gitignore` : Liste des types de fichiers à ne pas synchroniser avec Git.

## Compilation :
Le code source C contenu dans ce projet peut être compilé sous Linux en utilisant la commande :
```sh
$ gcc batnav.c -o batnav
```
ou via **Make** et le `Makefile` du projet en utilisant la commande :

```sh
$ make
```
N.B. La commande make lancera ensuite automatiquement l'exécution du logiciel.

## Exécution :
Le programme compilé peut ensuite être exécuté manuelement avec la commande :
```sh
$ ./batnav
```

## Mode d'affichage secret :

Un mode d'affichage `radar` permettant de voir où se trouvent tous les navires sur le plateau peut être accédé en entrant un code secret quand vient le temps d'entrer les coordonnées X-Y.

Ce code est une variante du "code Konami" : `uuddlrlrba`

## Version `deluxe` du jeu :

Une version améliorée du jeu est également disponible en compilant le code avec l'option

 `-DDELUXE` : 
```sh
$ gcc batnav.c -DDELUXE -o batnav
```
ou via Make et le Makefile du projet en utilisant la commande suivante :

```sh
$ make deluxe
```
Cette verion ajoute l'option de sauvegarder une partie en cours de route et de la continuer à un autre moment. Elle offre également un affichage  du plateau de jeu en couleur!

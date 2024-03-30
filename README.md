# Projet OS
## Description générale
Il s’agit d’implémenter une bibliothèque de fonctions de gestion de fichiers simulant celle d’UNIX. 
Après formatage d’une _partition_ (un gros fichier UNIX classique), les fonctions devront permettre : 
 - la **création**,
 - la **lecture**
 - l'**écriture**
 - la **recherche d’informations** dans un fichier
 - la **destruction**.
## Travail à fournir
La bibliothèque à construire devra contenir au minimum un type file et les fonctions suivantes: 
  - int myFormat(char* partitionName); la fonction retourne 0 si le “formatage” a réussi.
  - file * myOpen(char* fileName); la fonction “ouvre” ou crée un fichier de ce nom.
  - int myWrite(file* f, void* buffer, int nBytes); la fonction retourne le nombre d’octets écrits.
  - int myRead(file* f, void* buffer, int nBytes); la fonction retourne le nombre d’octets lus.
  - void mySeek(file* f, int offset, int base); la fonction déplace le pointeur de lecture / écriture du fichier
## Extension 
- gestion de “blocs disque” afin de résoudre les problèmes de fragmentation (extension fortement recommandée!)
- visualisation de la partition avec les parties réservées et libres évoluant en cours de programme
- fonctions supplémentaires (ex: int size(file* f);)

## Rapport 
En ce qui concerne la documentation, on demande de rédiger deux (petits) documents PDF: 
- Un **guide de l’utilisateur**, où le fonctionnement et l’utilisation du programme (paramètres, fichier(s) de configuration, . . . ) sont décrits. Le but de ce premier document est d’aider un programmeur système à utiliser ce programme (et, le cas échéant, les fonctions de la bibliothèque).
- Un **guide du développeur,** où l’algorithme utilisé (ainsi que vos structures de données) est précisé. Le but de ce second document est d’aider un programmeur qui souhaiterait améliorer / modifier la bibliothèque et / ou le programme de test à s’y retrouver dans le code de vos fonctions.

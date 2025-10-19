# Projet Logiciel Transversal: SmallWorld

---

#### Julien METZELARD – Tarek TALSI – Maël ARCHENAULT – Victor MOREL

---

![Illustration sur la boite du jeu](./rapport/img/game_illustration.jpg)


## 1 Objectif

### 1.1 Présentation générale

L'objectif de ce projet est de créer une version numérique du jeu de plateau "SmallWorld". Le projet est mené par 4 étudiants. Le jeu est codé en C++.

### 1.2 Règles du jeu

Le jeu choisi est un jeu de stratégie en tour par tour. Pour gagner, les joueurs doivent avoir à la fin du jeu le plus d'argent.

SmallWorld se joue grâce à un plateau représentant une carte de territoires, et des pions faisant office de troupes. Lorsque son tour arrive, le joueur doit attaquer les territoires de ses adversaires afin de conquérir un maximum de terrain. A la fin de son tour, il reçoit autant d'argent que de terrains qu'il possède.

![Plateau de jeu SmallWorld](./rapport/img/map.jpg)


À ces mécaniques de bases s'ajoutent des systèmes de pouvoirs. Chaque joueur choisit une "espèce" pour ses troupes ainsi qu'un "pouvoir". Les combinaisons espèce/pouvoir sont définies de manière aléatoire. Chaque espèce a un effet différent, de même pour les pouvoirs. Cela peut aller d'un bonus de troupes lors d'un attaque à un bonus de récompense à la fin du tour. Les pouvoirs sont très variés.

Chaque joueur peut, s'il le veut, abandonner son espèce actuelle et en prendre une nouvelle. L'espèce abandonnée reste sur le plateau, mais est marquée comme "en déclin". Les territoires occupés par une espèce en déclin rapportent toujours des récompenses à son ancien propriétaire.

Tout l'intérêt du jeu réside dans la capacité à changer d'espèce au bon moment, et à choisir la bonne combinaison espèce/pouvoir parmi celles proposées.


### 1.3 Conception Logiciel
Présenter ici les packages de votre solution, ainsi que leurs dépendances.


## 2 Description et conception des états

L'objectif de cette section est une description très fine des états dans le projet. Plusieurs niveaux de descriptions sont attendus. Le premier doit être général, afin que le lecteur puisse comprendre les éléments et principes en jeux. Le niveau suivant est celui de la conception logicielle. Pour ce faire, on présente à la fois un diagramme des classes, ainsi qu'un commentaire détaillé de ce diagramme. Indiquer l'utilisation de patron de conception sera très apprécié. Notez bien que les règles de changement d'état ne sont pas attendues dans cette section, même s'il n'est pas interdit d'illustrer de temps à autre des états par leurs possibles changements.


### 2.1 Description des états

L’état global du jeu est centralisé dans la classe Game_State, qui contient toutes les informations nécessaires pour décrire la partie à un instant donné :
- la carte (Map) contenant l’ensemble des informations sur les zones de la carte avec ses specificités ainsi que les troupes des joueurs,
- la liste des joueurs (Player),
- les tribus (combinaison d'espèce et de pouvoir) disponibles via une pile (Tribe_Stack),
- des paramètres de gestion de tour comme le nombre de joueur actif ainsi que le nombre de rounds.

Chaque Player représente un joueur de la partie et possède :
- un identifiant unique,
- un ensemble de tribus actives qu’il contrôle,
- un compteur de points de victoire accumulés,
- les méthodes associées à la conquête ou au déploiement d’unités.

Une Tribe correspond à la combinaison d’une espèce et d’un pouvoir spécial, ce qui définit les capacités et bonus de la tribu. Chaque tribu possède :
- un nombre d’unités disponibles,
- des descriptions (Species_Description, Power_Description) déterminant ses effets,
- des méthodes associé aux tribes comme go_in_decline et autre qui seront explicités plus tard.

Les zones (Area) modélisent les régions de la carte. Elles contiennent :
- un biome (Area_Biome) et éventuellement des spécial tokens (forteresse, tanière, etc.),
- une liste de voisins, pour représenter les connexions de la carte,
- un propriétaire (Tribe), ici il est important que le propriétaire soit une Tribe et non pas un Player car un Player peut avoir plusieurs Tribe et que ces Tribe peuvent s'attaquer mutuellement,
- et des méthodes permettant la conquête ou le déploiement d’unités.

L’ensemble de ces zones est géré par la classe Map, qui stocke leur liste et permet le chargement depuis un fichier JSON (utilisé pour initialiser la carte).

Enfin, les effets spéciaux sont gérés par la hiérarchie Effects_Bundle :
- La classe abstraite Effects_Bundle définit une interface générique (apply_first_round_effect, apply_conquest_effect, etc.).
- Des classes concrètes comme Dwarf_Effects_Bundle, Ratmen_Effects_Bundle ou Giant_Effects_Bundle héritent de cette interface pour appliquer des bonus spécifiques.


### 2.2 Conception logicielle
	
### 2.3 Conception logicielle: extension pour le rendu

### 2.4 Conception logicielle: extension pour le moteur de jeu

### 2.5 Ressources

Illustration 1: Diagramme des classes d'état

<!-- 
## 3 Rendu: Stratégie et Conception
Présentez ici la stratégie générale que vous comptez suivre pour rendre un état. Cela doit tenir compte des problématiques de synchronisation entre les changements d'états et la vitesse d'affichage à l'écran. Puis, lorsque vous serez rendu à la partie client/serveur, expliquez comment vous aller gérer les problèmes liés à la latence. Après cette description, présentez la conception logicielle. Pour celle-ci, il est fortement recommandé de former une première partie indépendante de toute librairie graphique, puis de présenter d'autres parties qui l'implémente pour une librairie particulière. Enfin, toutes les classes de la première partie doivent avoir pour unique dépendance les classes d'état de la section précédente.

### 3.1 Stratégie de rendu d'un état

### 3.2 Conception logicielle

### 3.3 Conception logicielle: extension pour les animations

### 3.4 Ressources

### 3.5 Exemple de rendu

Illustration 2: Diagramme de classes pour le rendu

## 4 Règles de changement d'états et moteur de jeu
Dans cette section, il faut présenter les événements qui peuvent faire passer d'un état à un autre. Il faut également décrire les aspects liés au temps, comme la chronologie des événements et les aspects de synchronisation. Une fois ceci présenté, on propose une conception logicielle pour pouvoir mettre en œuvre ces règles, autrement dit le moteur de jeu.

### 4.1 Horloge globale

### 4.2 Changements extérieurs

### 4.3 Changements autonomes

### 4.4 Conception logicielle

### 4.5 Conception logicielle: extension pour l'IA

### 4.6 Conception logicielle: extension pour la parallélisation

Illustration 3: Diagrammes des classes pour le moteur de jeu


## 5 Intelligence Artificielle
Cette section est dédiée aux stratégies et outils développés pour créer un joueur artificiel. Ce robot doit utiliser les mêmes commandes qu'un joueur humain, ie utiliser les mêmes actions/ordres que ceux produit par le clavier ou la souris. Le robot ne doit pas avoir accès à plus information qu'un joueur humain. Comme pour les autres sections, commencez par présenter la stratégie, puis la conception logicielle.
### 5.1 Stratégies

#### 5.1.1 Intelligence minimale

#### 5.1.2 Intelligence basée sur des heuristiques

#### 5.1.3 Intelligence basée sur les arbres de recherche

### 5.2 Conception logicielle

### 5.3 Conception logicielle: extension pour l'IA composée

### 5.4 Conception logicielle: extension pour IA avancée

### 5.5 Conception logicielle: extension pour la parallélisation


## 6 Modularisation
Cette section se concentre sur la répartition des différents modules du jeu dans différents processus. Deux niveaux doivent être considérés. Le premier est la répartition des modules sur différents threads. Notons bien que ce qui est attendu est un parallélisation maximale des traitements: il faut bien démontrer que l'intersection des processus communs ou bloquant est minimale. Le deuxième niveau est la répartition des modules sur différentes machines, via une interface réseau. Dans tous les cas, motivez vos choix, et indiquez également les latences qui en résulte.

### 6.1 Organisation des modules

#### 6.1.1 Répartition sur différents threads

#### 6.1.2 Répartition sur différentes machines

### 6.2 Conception logiciel

### 6.3 Conception logiciel: extension réseau

### 6.4 Conception logiciel: client Android

Illustration 4: Diagramme de classes pour la modularisation -->

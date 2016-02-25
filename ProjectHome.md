### Logiciel libre, destiné à consulter le dictionnaire de la langue française d'Émile Littré, ouvrage de référence des amoureux de la littérature. ###

![http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureI.gif](http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureI.gif)

## Fonctions du logiciel. ##

J'indique **en gras** les fonctions ajoutées par la version 2.0 :

  * Consulter l'intégralité du dictionnaire et de son supplément (excepté les étymologies grecques, qui paraîtront dans la version 3.0) ;
  * Naviguer d'un mot à l'autre par double-clic ;
  * Avancer ou reculer dans l'historique des articles consultés ;
  * Afficher l'application en plein écran ;
  * Exporter l'article courant aux formats HTML, **OpenDocument, PDF ou texte simple** ;
  * Imprimer l'article courant **et afficher l'aperçu avant impression** ;
  * Survoler les divers sens et acceptions dans l'article courant ;
  * Chercher **et mettre en surbrillance** un mot ou une expression rationnelle dans l'article courant ;
  * Chercher **et voir d'un coup d'œil** tous les emplois d'un mot ou d'une expression dans le dictionnaire ;
  * Afficher la conjugaison ou les féminins et pluriels ;
  * **Imprimer la conjugaison ou l'exporter aux formats mentionnés plus haut** ;
  * **Changer les couleurs et la typographie de chaque élément à l'aide d'une feuille de style personnelle** ;
  * **Défiler à fur et à mesure que les lettres d'un mot sont entrées** ;
  * **Consulter un mot sans entrer les accents ni la cédille** (cette fonction est désactivée par défaut) ;
  * Grâce à l'icône dans la barre des tâches :
    * Sous Linux : Consulter un mot sélectionné dans une autre application ;
    * Sous Windows : Consulter un mot copié dans le presse-papiers ;
  * Lire la préface et la causerie d'Émile Littré, **et les hommages rendus à son œuvre par Pasteur, Renan, Zola** ;
  * **Garder l'état du dictionnaire d'une session à l'autre** ;
  * **Déceler la présence de lecteurs d'écran et rendre l'interface accessible aux personnes aveugles** (reportez-vous à la section « Accessibilité »).

![http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureII.gif](http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureII.gif)

## Installation. ##

Vous **n'**avez **pas** besoin de privilèges administratifs pour installer « Dictionnaire le Littré ».

#### Sous Linux. ####

  1. Télécharger l'archive [Littre-linux-2.0.tar.bz2](http://dictionnaire-le-littre.googlecode.com/files/Littre-linux-2.0.tar.bz2) dans un coin de votre disque dur.
  1. Extraire le dossier qui s'y trouve.
  1. Dans les propriétés du fichier _Littre_, cocher la case _Autoriser l'exécution du fichier comme un programme_.
  1. Double-cliquer sur _Littre_ pour lancer le dictionnaire.

#### Sous Windows. ####

  1. Télécharger le fichier d'installation [Littre-windows-2.0.exe](http://dictionnaire-le-littre.googlecode.com/files/Littre-windows-2.0.exe) dans un coin de votre disque dur.
  1. Double-cliquer sur le fichier. L'assistant d'installation s'ouvre.
  1. Une fois l'installation terminée, cliquer sur l'icône _Dictionnaire le Littré_ pour lancer le logiciel.

Si vous désirez transporter le logiciel avec vous, il suffit de copier le dossier où il a été installé sur une clé USB ou sur un disque compacte. Tous les fichiers dont se sert « Dictionnaire le Littré » (y compris votre fichier de préférences) sont réunis dans ce dossier.

![http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureIV.gif](http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureIV.gif)

## Paramètres de la ligne de commandes. ##

Vous pouvez lancer le fichier exécutable _Littre_ en précisant un ou plusieurs des paramètres suivants :

```
--titre <titre> (par exemple, --titre "Dictionnaire ‒ Collège Louis-Riel, Manitoba")
```

Change le titre de la fenêtre principale du dictionnaire.

```
--repertoire <répertoire>
```

Précise le répertoire où _Littre_ cherchera ses fichiers de données (uniquement pour usagers avancés).

```
--remise-a-neuf
```

Ramène tous les éléments du dictionnaire à leur état d'origine et efface vos préférences.

## Compilation du code. ##

« Dictionnaire le Littré » est compatible avec une multitude de plates-formes informatiques. Le code de « Dictionnaire le Littré » repose exclusivement sur la bibliothèque Qt, qui permet de créer des fichiers exécutables pour Macintosh, tout comme pour Linux ou Windows, et ce, sans rien changer à la source. Voici comment :

  1. Télécharger l'archive [Littre-source-2.0.tar.bz2](http://dictionnaire-le-littre.googlecode.com/files/Littre-source-2.0.tar.bz2) dans un coin de votre disque dur.
  1. Extraire le dossier qui s'y trouve.
  1. Installer [Qt SDK LGPL](http://qt.nokia.com/downloads) pour la plate-forme voulue.
  1. Une fois l'installation terminée, cliquer sur l'icône _Qt Creator_.
  1. Choisir File > Open... et ouvrir le fichier _Littre.pro_ (extrait à l'étape 2).
  1. Choisir Build > Build All dans le menu de _Qt Creator_.

Le fichier exécutable _Littre_ est alors créé dans le sous-dossier _bin_ (voir l'étape 2). Avec d'importantes modifications et les outils appropriés, il est même possible de compiler « Dictionnaire le Littré » pour Windows CE ou Symbian OS.

## Accessibilité. ##

Lorsque vous lancez « Dictionnaire le Littré », il décèle la présence de lecteurs d’écran, destinés aux personnes aveugles ou malvoyantes. Le texte « Mode accessible » apparaît alors dans le titre de la fenêtre principale, et l'interface devient lisible par la synthèse vocale. Il faut dire que la bibliothèque Qt, sur laquelle repose le logiciel, ne produit presque rien d'accessible. Il m'a fallu parfois des pages de code pour décrire imparfaitement un menu ou une zone de liste. Il s'agit d'un travail artisanal et parcellaire, mais j'ose croire qu'il profitera à certains d'entre vous. Je garantis de bons résultats avec le lecteur libre [NVDA](http://www.nvda.fr/).

## Crédits. ##

Logiciel « Dictionnaire le Littré » par Murielle Descerisiers

http://dictionnaire-le-littre.googlecode.com/

Base de données « XMLittré » par François Gannaz

http://francois.gannaz.free.fr/Littre/

Icônes « Buuf » et « Buuf Deuce » par Paul Davey

http://mattahan.deviantart.com/

Police « DejaVu Serif » par Štěpán Roh

http://dejavu-fonts.org/

![http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureIII.gif](http://dictionnaire-le-littre.googlecode.com/svn/trunk/CaptureIII.gif)

## Licence. ##

© Murielle Descerisiers, 2009

[murielle.descerisiers@gmail.com](mailto:murielle.descerisiers@gmail.com)

Ce logiciel est un programme informatique développé au Québec et servant à consulter le dictionnaire de la langue française d'Émile Littré.

Ce logiciel est régi par la licence CeCILL soumise au droit français et respectant les principes de diffusion des logiciels libres. Vous pouvez utiliser, modifier et/ou redistribuer ce programme sous les conditions de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site http://www.cecill.info.

En contrepartie de l'accessibilité au code source et des droits de copie, de modification et de redistribution accordés par cette licence, il n'est offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une responsabilité restreinte pèse sur l'auteur du programme, le titulaire des droits patrimoniaux et les concédants successifs.

À cet égard l'attention de l'utilisateur est attirée sur les risques associés au chargement, à l'utilisation, à la modification et/ou au développement et à la reproduction du logiciel par l'utilisateur étant donné sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et qui le réserve donc à des développeurs et des professionnels avertis possédant des connaissances informatiques approfondies. Les utilisateurs sont donc invités à charger et tester l'adéquation du logiciel à leurs besoins dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris connaissance de la licence CeCILL, et que vous en avez accepté les termes.

![http://dictionnaire-le-littre.googlecode.com/svn/trunk/Quebec.jpg](http://dictionnaire-le-littre.googlecode.com/svn/trunk/Quebec.jpg)
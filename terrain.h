#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include <stdio.h>

typedef enum { LIBRE, EAU, ROCHER, MARQUE} Case;

#define DIM_MAX 256

// indexation utilisée :
//  1er  indice : abscisse = colonne (colonne de gauche : abscisse = 0)
//  2ème indice : ordonnée = ligne   (ligne du haut     : ordonnée = 0)

typedef struct {
  int largeur, hauteur;
  Case tab[DIM_MAX][DIM_MAX];
} Terrain;

typedef enum {
  OK,

  //si le fichier ne sais pas ouvert
  ERREUR_FICHIER, 

   //si a la lecture de la largeur/hauteur il y a un souci (pas un int...)
  ERREUR_FICHIER_LARGEUR,
  ERREUR_FICHIER_HAUTEUR,

   //si a la lecture de la largeur/hauteur la donne n'est pas dans le bon domaine (0 <= donee < DIM_MAX)
  ERREUR_LARGEUR_DOMAINE,
  ERREUR_HAUTEUR_DOMAINE,

   //si il y a moin de ligne que hauteur
  ERREUR_MOIN_DE_LIGNE,

  //si apres avoir lu toutes les ligne il en reste encore une 
  ERREUR_TROP_DE_LIGNE,

  //si la largeur d'une ligne est trop petite/longue
  ERREUR_LARGEUR_TROP_PETITE,
  ERREUR_LARGEUR_TROP_GRANDE,

  //si un des caractère lu n'est pas '#', '.', '~' ou 'C'
  ERREUR_CARACTERE,

  //si il y a pas de case depart dans le fichier lu
  ERREUR_CASE_DEPART
} erreur_terrain;
/*retourn le type de la case qui correspond au caractere c*/
Case type_case(char c);

/* Lecture d'un terrain dans un fichier f, ouvert en lecture
   Résultats :
   t le terrain lu
   x, y position initiale du robot lue dans le fichier terrain
   Renvoie :
   OK si la lecture s'est déroulée correctement
   ERREUR_FICHIER si le fichier n'a pas pu être ouvert
   ... (à compléter)
 */
erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y);

/* Largeur d'un terrain */
int largeur(Terrain *t);

/* Hauteur d'un terrain */
int hauteur(Terrain *t);

/* Indique si la case de coordonnées (x,y) est libre
   Renvoie vrai ssi les 3 conditions suivantes sont vraies :
    - 0 <= x < largeur
    - 0 <= y < hauteur
    - t.tab[x][y] = LIBRE
 */
int est_case_libre(Terrain *t, int x, int y);

/*Renvoie  '~' si la case est de l'eau, '#' si c'est un rocher ou ' ' si c'est libre*/
char afficher_case(Case c);

/* Affichage d'un terrain t sur la sortie standard */
void afficher_terrain(Terrain *t);

/* Écriture d'un terrain t dans un fichier f ouvert en écriture.
   x et y contiennent les coordonnées du robot
   Le terrain est écrit au format lisible par lire_terrain */
void ecrire_terrain(FILE *f, Terrain *t, int x, int y);

#endif

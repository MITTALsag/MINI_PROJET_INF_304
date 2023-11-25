
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>


Case type_case(char c){
  switch(c){
    case '~':
      return EAU;
    case '#':
      return ROCHER;
    default: //si c = '.' ou 'C' (C correspond a une case libre) (on verifie deja que c = '#', '.' '~', 'C')
      return LIBRE;
  }
}

erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y) {
  int largeur, hauteur;   // Dimensions du terrain

  // Coordonnées initiales du robot
  int rx = -1; //pour verifier que C est dans le terrain
  int ry; 
  int nb_val;
  char c; //pour mettre dans c chaque element d'une ligne 
  char* ligne; //pour recuperer toutes les ligne avec fgets
  char tab_ligne[DIM_MAX]; //tableau d'une ligne
  int len_ligne; //pour avoir la longeur d'une ligne
  Case case_ij; //pour recuperer la case qui correspond a la ligne i et la colonne j (grace a la fonction type_case)
  

  if (f == NULL) 
    return ERREUR_FICHIER;
  


  // Lecture de la largeur
  nb_val = fscanf(f, "%d", &largeur);
  if (nb_val == 0)
    return ERREUR_FICHIER_LARGEUR;
  if (largeur < 0 || largeur > DIM_MAX) 
    return ERREUR_LARGEUR_DOMAINE;
    
  t->largeur = largeur;


  // Lecture de la hauteur
  nb_val = fscanf(f, "%d\n", &hauteur);
  if (nb_val == 0)
    return ERREUR_FICHIER_HAUTEUR;
  if (hauteur < 0 || hauteur > DIM_MAX) 
    return ERREUR_HAUTEUR_DOMAINE;
    
  t->hauteur = hauteur;

  // Lecture du terrain
  
  for(int i = 0 ; i < hauteur ; i++){
    //on lit la ligne
    ligne = fgets(tab_ligne, DIM_MAX, f);

    if (!ligne)
      return ERREUR_MOIN_DE_LIGNE;
    
    len_ligne = strlen(tab_ligne) - 1; //le -1 est la pour enlever '\n' 
    if (len_ligne < largeur)
      return ERREUR_LARGEUR_TROP_PETITE;
    if (len_ligne > largeur)
      return ERREUR_LARGEUR_TROP_GRANDE;

    //lecture de chaque element de la ligne pour mettre dans t->tab
    for(int j = 0  ; j < largeur ; j++){

      c = tab_ligne[j];

      if (c != '#' && c != '.' && c != '~' && c != 'C' && c != 'M')
        return ERREUR_CARACTERE;

      if (c == 'C'){
        rx = j;
        ry = i;
      }

      case_ij = type_case(c);
      t->tab[j][i] = case_ij;
    }
  }

  if (rx == -1)
    return ERREUR_CASE_DEPART;

  *x = rx;
  *y = ry;

  if (fgets(tab_ligne, DIM_MAX, f))
    return ERREUR_TROP_DE_LIGNE;
    
  

  // Initialisation de la position du robot
  // À compléter

  return OK;
}

int largeur(Terrain *t){
  return t->largeur;
}

int hauteur(Terrain *t){
  return t->hauteur;
}

int est_case_libre(Terrain *t, int x, int y){
  return ((0 <= x && x < t->largeur) && (0 <= y && y < t->hauteur) && (t->tab[x][y] == LIBRE));
}

char afficher_case(Case c){
  switch (c){
    case LIBRE :
      return '.';
    case EAU:
      return '~';
    case ROCHER:
      return '#';
    case MARQUE:
      return 'M';
  }
}

void afficher_terrain(Terrain *t){

  for(int ligne = 0; ligne < t->hauteur; ligne++){
    for(int colonne = 0; colonne < t->largeur; colonne++){
      printf("%c", afficher_case(t->tab[colonne][ligne]));
    }
    printf("\n");
  }
}

void ecrire_terrain(FILE *f, Terrain *t, int x, int y){

  fprintf(f, "%d\n", t->largeur);
  fprintf(f, "%d\n", t->hauteur);

  for(int ligne = 0; ligne < t->hauteur; ligne++){
    for(int colonne = 0; colonne < t->largeur; colonne++){
      if (colonne == x && ligne == y){
        fprintf(f, "C");
      }
      else 
        fprintf(f, "%c", afficher_case(t->tab[ligne][colonne]));
    }
    fprintf(f, "\n");
  }
}
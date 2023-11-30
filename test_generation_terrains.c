#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

bool silent_mode = false;

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  int N, i, l, h;
  float dObst;
  float dens_obtenu; /* pour recup la densité de chaque terrain valide (avec chemein vers l'exterieur existant)*/
  float dens_moy = 0; /* pour recupe la somme des denstité des terrains valide (avec chemein vers l'exterieur existant) pour ensuite calculer la moyenne */
  int nb_terrain_valide = 0; /* nombre de terrain valide (pour calculer la densité moyenne) */
  FILE *resFile;
  Terrain T;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "Nombre de terrain voulu : %d\n", N);
  fprintf(resFile, "----------------------------------------------------------------------------\n");


  // Initialisation de la fonction random
  srand(time(NULL));
  

  for(i = 0 ; i < N ; i++){
    dens_obtenu = generation_aleatoire(&T, l, h, dObst);

    if (!silent_mode){
      afficher_terrain(&T);
      if (existe_chemin_vers_sortie(&T)){
        printf(GRN"Il existe un chemin du centre vers la sortie. Donc on le met dans le fichier %s\n"RESET, argv[5]);
        fprintf(resFile, "Il existe un chemin du centre vers la sortie.\n");

      }
      else{
        printf(RED"Il n'existe pas de chemin vers le bord du terrain depuis le centre. Donc on ne le met pas dans le fichier %s\n"RESET, argv[5]);
      }
      stop();
    }

    if (existe_chemin_vers_sortie(&T)){
      ecrire_terrain(resFile, &T, l/2, h/2);
      nb_terrain_valide++;
      dens_moy += dens_obtenu;
      fprintf(resFile, "Densité d'obstacle du terrain : %f\n", dens_obtenu);
      if (dens_obtenu > dObst)
        fprintf(resFile, "La densité de ce terrain est suprérieur a celle attendue.\n");
      else
        fprintf(resFile, "La densité de ce terrain est inferieur a celle attendue.\n");
        
      fprintf(resFile, "----------------------------------------------------------------------------\n");

    }
    
  }

  // Écriture/Affichage des statistiques 
  if (nb_terrain_valide > 0){
    fprintf(resFile, "La densité moyenne d'obstacle sur l'ensemble des fichier valide est : %f\n", dens_moy/nb_terrain_valide);
    fprintf(resFile, "La densité attendue est : %f.\n", dObst);
    if(dens_moy > dObst)
      fprintf(resFile, "La densité moyenne est donc supérieur a celle voulu.\n");
    else if (dens_moy == dObst)
      fprintf(resFile, "La densité moyenne est bien égale à celle voulu.\n");
    else 
      fprintf(resFile, "La densité moyenne est inférieure à celle voulu.\n");
  }
  else 
    fprintf(resFile, "Il n'y a aucun terrain valide.\n");



  // fermeture des fichiers
  fclose(resFile);
  return 0;
}

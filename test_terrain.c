#include "terrain.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;
  erreur_terrain err; //pour verifier si la fonction lire_terrain c'est bien passe
  char fichier[256]; //our le nom du fichier en cas d'erreur de argv[1]
  bool err_fich = false; //pour la reecriture du fichier (savoir si on prend argv[1] car bon fich ou fichier car argv[1] pas bon)
  char *nom_fich; //pour la reecriture du fichier


  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  err = lire_terrain(f, &t, &x, &y);

  
  if (f) fclose(f);

  while (err != OK){
    printf("Le fichier n'est pas au bon format, recommencer : ");
    scanf("%s", fichier);
    f = fopen(fichier, "r");
    err = lire_terrain(f, &t, &x, &y);
    if (f) fclose(f);
    err_fich = true;
  }

  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);

  //reecriture du fichier dans au format <nom_fichier>.reecriture
  if (!err_fich)
    nom_fich = strcat(argv[1], ".reecriture");
  else
    nom_fich = strcat(fichier, ".reecriture");

  FILE* fich = fopen(nom_fich, "w+");
  ecrire_terrain(fich, &t, x, y);
  fclose(fich);
}

#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "gestion_erreur.h"

#include <stdio.h>
#include <stdlib.h>

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}


int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  if (argc < 3) {
    printf("Usage: %s <terrain> <programme>\n", argv[0]);
    return 1;
  }

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
  errt = initialise_environnement(&envt, argv[1]);
  gestion_erreur_terrain(errt);

  /* Lecture du programme */
  errp = lire_programme(&prog, argv[2]);
  gestion_erreur_programme(errp);

  afficher_envt(&envt);
  printf ("Programme:");
  afficher_prog(&prog);
  //print(&etat.stack);
  printf ("\n");
  char c = afficher_case(envt.t.tab[7][2]);
  printf ("%c\n", c);
  c = afficher_case(envt.t.tab[8][2]);
  printf ("%c\n", c);
  stop(); //pour faire le programme pas a pas 

  /* Initialisation de l'état */
  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);

    /* Affichage du terrain et du robot si le robot est OK*/
    if (res == OK_ROBOT){
      afficher_envt(&envt);
      printf ("Programme:");
      afficher_prog(&prog);
      printf("\n");
      stop();
      }

  } while (res == OK_ROBOT);

  gestion_erreur_interprete(res);
}

#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "gestion_erreur.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


bool silent_mode = true;
bool silent_avant_apres = true; //sert a montrer au debut et a la fin de l'execution


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

  if (!silent_avant_apres){
    afficher_envt(&envt);
    /* Afficher le programme */
    printf ("Programme:");
    afficher_prog(&prog);
    printf("\n");
    /* Afficher la pile */
    printf ("Pile:");
    print(&etat.stack);
    printf ("\n");
    /* Afficher sp */
    printf ("SP:");
    print(&etat.sp);
    printf ("\n");
    stop();
  }

  /* Initialisation de l'état */
  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);

    /* Affichage du terrain et du robot si le robot est OK*/
    if (res == OK_ROBOT){
      if (!silent_mode){
        afficher_envt(&envt);
        /* Afficher le programme */
        printf ("Programme:");
        afficher_prog(&prog);
        printf("\n");
        /* Afficher la pile */
        printf ("Pile:");
        print(&etat.stack);
        printf ("\n");
        /* Afficher sp */
        printf ("SP:");
        print(&etat.sp);
        printf ("\n");
        /* choisir entre un des deux : stop pour exec pas a pas et usleep pour afficher toutes les etapes mais tres vite */
        usleep(100);
        //stop(); 
      }
    }

  } while (res == OK_ROBOT);
  
  if (silent_mode)
    afficher_envt(&envt);

  gestion_erreur_interprete(res);
}

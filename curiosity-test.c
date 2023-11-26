#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "gestion_erreur.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RESET "\x1B[0m"
#define YEL "\x1B[33m"

bool silent_mode = false;
bool silent_avant_apres = false; //sert a montrer au debut et a la fin de l'execution


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
    char fich_ter[256];
    char fich_prog[256];
    int nb_pas;
    int i; /* nombre de pas deja effectuer */
    char event;
    FILE* f;

    FILE* fich_err; /* pour mettre toutes les erreur dans ce fichiers */
    fich_err = fopen("fichier_erreur.txt", "w");
    if (!fich_err){
        printf("Erreur lors de l'ouverture ou de la creation du fichier d'erreur\n");
        exit(1);
    }

    if (argc < 2) {
        printf("Usage: %s <fichier d'execution>\n", argv[0]);
        return 1;
    }

    /* boucle pour tout les fichier test */
    for(int j = 1 ; j < argc ; j++){

        printf("\n");
        printf("------------------------------------------------\n");
        printf(YEL"test avec le fichier %s\n\n"RESET, argv[j]);

        /* Ouverture du fichier d'execution*/
        f = fopen(argv[j], "r");
        if (!f){
            printf("ERREUR OUVERTURE FICHIER %s\n", argv[1]);
            return 1;
        }


        fscanf(f,"%s", fich_ter);
        /* Initialisation de l'environnement : lecture du terrain, initialisation de la position du robot */
        errt = initialise_environnement(&envt, fich_ter);
        gestion_erreur_terrain(errt);


        fscanf(f,"%s\n", fich_prog);
        /* Lecture du programme */
        errp = lire_programme(&prog, fich_prog);
        gestion_erreur_programme(errp);

        /* recuperation du nombre de pas max*/
        fscanf(f, "%d\n", &nb_pas);

        /* recuperation de l'evenement attendue */
        fscanf(f, "%c\n", &event);

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
        i = 0;
        do {
            res = exec_pas(&prog, &envt, &etat);
            i++;
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
                stop();
                }
            }

        } while (res == OK_ROBOT && (i < nb_pas));

        /* Affichage de fin d'execution si silent_mode == true */
        if (!silent_avant_apres)
            afficher_envt(&envt);
        
        
        fprintf(fich_err, "curiosity-test------>%s : \n", argv[j]);
        /* gestion de l'evenement */
        gestion_erreur_event(event, res, &envt.r, f, fich_err);
        fprintf(fich_err,"\n\n");

        fclose(f);
    }
    fclose (fich_err);
}

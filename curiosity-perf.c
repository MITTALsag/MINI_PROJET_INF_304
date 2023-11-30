#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "gestion_erreur.h"
#include "generation_terrains.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool silent_mode = true;
bool silent_avant_apres = false; //sert a montrer au debut et a la fin de l'execution

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"


void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

/* Pour ecrire le resultat du fichier i a la ligne i */
void gestion_res(FILE* fich_res, resultat_inter res, int nb_pas ,int* nb_sortie, int* nb_bloque, int* nb_obstacle, int* nb_pas_moy){
    switch(res){
        case OK_ROBOT:
        case ARRET_ROBOT:
            fprintf(fich_res, "curiosity est bloqué (Programme arreté ou nb_pas_max atteint) (-1).\n");
            *nb_bloque +=1;
            break;
        case PLOUF_ROBOT:
            fprintf(fich_res, "curiosity est tombé dans l'eau (-2).\n");
            *nb_obstacle +=1;
            break;
        case CRASH_ROBOT:
            fprintf(fich_res, "curiosity s'est écrasé contre un rocher (-3).\n");
            *nb_obstacle +=1;
            break;
        case SORTIE_ROBOT:
            fprintf(fich_res, "curiosity est sort, avec (%d) pas.\n", nb_pas);
            *nb_sortie += 1;
            *nb_pas_moy += nb_pas;
            break;
        default:
            printf("Erreur resultat impossible\n");
            exit(1);
    }
}


int main(int argc, char **argv) {
    Environnement envt;
    Programme prog;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;
    int N, i, j, l, h, nb_step_max, graine;
    float dObst;
    FILE *resFile;
    Terrain T;
    /* initialisation valeur pour les stats */
    int nb_sortie = 0;
    int nb_bloque = 0;
    int nb_obstacle = 0;
    int nb_pas = 0;

    int pour_sortie = 0;
    int pour_obstacle = 0;
    int pour_bloque = 0;


    if (argc < 9) {
        printf("Usage: %s <fichier_programme> <N> <L> <H> <d> <graine> <nb_step_max> <fichier_res>\n%d", argv[0], argc);
        return 1;
    }

    /* Lecture du programme */
    errp = lire_programme(&prog, argv[1]);
    gestion_erreur_programme(errp);

    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    dObst = strtof(argv[5], NULL);
    graine = strtol(argv[6], NULL, 10);
    nb_step_max = strtol(argv[7], NULL, 10);


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
    resFile = fopen(argv[8], "w");

    // Écriture du nombre de terrains
    fprintf(resFile, "Nombre de terrain testé : %d\n", N);
    fprintf(resFile, "----------------------------------------------------------------------------\n");

    // Initialisation de la fonction random
    srand(time(NULL));

    for(i = 0 ; i < N ; i++){
        generation_aleatoire(&T, l, h, dObst);

        
        if (!silent_avant_apres){
            afficher_terrain(&T);
            if (existe_chemin_vers_sortie(&T)){
                printf(GRN"Il existe un chemin du centre vers la sortie. Donc on execute le programme %s\n"RESET, argv[1]);
            }
            else{
                printf(RED"Il n'existe pas de chemin vers le bord du terrain depuis le centre. Donc on n'execute pas le programme %s\n"RESET, argv[1]);
                printf("----------------------------------------------------------------------------\n");

            }
            stop();
        }
    

        if (existe_chemin_vers_sortie(&T)){
            /* Initialisation de l'état */
            init_etat(&etat);
            /* initialisation de envt */
            envt.t = T; 
            init_robot(&(envt.r), l/2, h/2, Est);
            /* initialisation nb_pas_max */
            j = 0;
            do {

                res = exec_pas(&prog, &envt, &etat);
                j++;

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

            } while (res == OK_ROBOT && j < nb_step_max);

            /* affichage dans resFILe du resultat du terrain i */
            gestion_res(resFile, res, j, &nb_sortie, &nb_bloque, &nb_obstacle, &nb_pas);

            /* pour verifier que gestion_res est bon */
            if(!silent_avant_apres){
                gestion_erreur_interprete(res);
                printf("----------------------------------------------------------------------------\n");
            }


        }
        else {
            fprintf(resFile, "curiosity est bloqué (il n'existe pas de chemin vers le bord du terrain depuis le centre) (-1).\n"); 
            nb_bloque++;
        }
    } 

    /* Écriture/Affichage des statistiques */

    pour_sortie = (int) (((float) nb_sortie / (float) N) * 100.);
    pour_bloque = (int) (((float) nb_bloque / (float) N) * 100.);
    pour_obstacle = (int) (((float) nb_obstacle / (float) N) * 100.);

    printf("Curiosity est sortie %d fois. Ce qui represente %d%% de sortie.\n", nb_sortie, pour_sortie);
    printf("Curiosity à été bloqué %d fois. Ce qui represente %d%%\n", nb_bloque, pour_bloque);
    printf("Curiosity à rencontré %d obstacle. Ce qui represente %d%% d'accident (rocher ou eau)\n", nb_obstacle, pour_obstacle);

    if (nb_sortie > 0)
        printf("Le nombre de pas moyen pour sortir est : %d\n", (int)((float)nb_pas/(float)nb_sortie));

    fclose(resFile);

}

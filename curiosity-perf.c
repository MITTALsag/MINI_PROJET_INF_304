#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "gestion_erreur.h"
#include "generation_terrains.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool silent_mode = true;
bool silent_avant_apres = true; //sert a montrer au debut et a la fin de l'execution

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
void gestion_res(FILE* fich_res, resultat_inter res, int nb_pas ,int* nb_sortie, int* nb_bloque, int* nb_bloque_pas_max, int* nb_obstacle, int* nb_pas_moy){
    switch(res){
        case OK_ROBOT:
            fprintf(fich_res, "\nCuriosity est bloqué : le robot à atteint le nombre de pas max (-1)\n");
            *nb_bloque_pas_max +=1;
            break;
        case ARRET_ROBOT:
            fprintf(fich_res, "\ncuriosity est bloqué (Programme arreté) (-1).\n");
            *nb_bloque +=1;
            break;
        case PLOUF_ROBOT:
            fprintf(fich_res, "\ncuriosity est tombé dans l'eau (-2).\n");
            *nb_obstacle +=1;
            break;
        case CRASH_ROBOT:
            fprintf(fich_res, "\ncuriosity s'est écrasé contre un rocher (-3).\n");
            *nb_obstacle +=1;
            break;
        case SORTIE_ROBOT:
            fprintf(fich_res, "\ncuriosity est sorti, avec (%d) pas.\n", nb_pas);
            *nb_sortie += 1;
            *nb_pas_moy += nb_pas;
            break;
        default:
            printf("Erreur resultat impossible\n");
            exit(1);
    }
    fprintf(fich_res, "\n----------------------------------------------------------------------------\n");

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
    int nb_obstacle = 0;
    int nb_pas = 0;
    int nb_bloque_pas_max = 0; //bloque car nb_pas_max atteint
    int nb_bloque_impossible = 0; //bloque car pas de sortie vers l'exterieur
    int nb_bloque = 0; //car programme fini mais pas sorti


    float pour_sortie = 0;
    float pour_obstacle = 0;
    float pour_bloque = 0;
    float pour_bloque_impossible = 0;
    float pour_bloque_pas_max = 0;


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
    fprintf(resFile, "\nNombre de terrain testé : %d\n", N);
    fprintf(resFile, "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    // Initialisation de la fonction random
    //srand(graine);
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
            gestion_res(resFile, res, j, &nb_sortie, &nb_bloque, &nb_bloque_pas_max, &nb_obstacle, &nb_pas);

            /* pour verifier que gestion_res est bon */
            if(!silent_avant_apres){
                gestion_erreur_interprete(res);
                printf("----------------------------------------------------------------------------\n");
            }


        }
        else {
            fprintf(resFile, "curiosity est bloqué (il n'existe pas de chemin vers le bord du terrain depuis le centre) (-4).\n"); 
            nb_bloque_impossible++;
            fprintf(resFile, "----------------------------------------------------------------------------\n");

        }
    } 

    /* Écriture/Affichage des statistiques */

    pour_sortie = (((float) nb_sortie / (float) N) * 100.);
    pour_bloque = (((float) nb_bloque / (float) N) * 100.);
    pour_obstacle = (((float) nb_obstacle / (float) N) * 100.);
    pour_bloque_impossible = (((float) nb_bloque_impossible / (float) N) * 100.);
    pour_bloque_pas_max = (((float)nb_bloque_pas_max / (float) N) * 100.);

    printf("\n----------------------------------------------------------------------------\n");
    printf("\nCuriosity est sortie %d fois. Ce qui represente %f%% de sortie.\n", nb_sortie, pour_sortie);
    printf("\n----------------------------------------------------------------------------\n");

    printf("\nCuriosity à été bloqué %d fois. Ce qui represente %f%%\n", nb_bloque, pour_bloque);
    printf("\n----------------------------------------------------------------------------\n");

    printf("\nCuriosity à été bloqué car il a atteint le nombre de pas max %d fois. Ce qui represente %f%%\n", nb_bloque_pas_max, pour_bloque_pas_max);
    printf("\n----------------------------------------------------------------------------\n");

    printf("\nIl y a eut %d terrain ou il était impossible de sortir car pas de chemin vers l'exeterieur. Ce qui represente %f%% des terrain cree.\n", nb_bloque_impossible, pour_bloque_impossible);
    printf("\n----------------------------------------------------------------------------\n");
    
    printf("\nCuriosity à rencontré %d obstacle. Ce qui represente %f%% d'accident (rocher ou eau)\n", nb_obstacle, pour_obstacle);
    printf("\n----------------------------------------------------------------------------\n");
    
    if (nb_sortie > 0){
        printf("\nLe nombre de pas moyen pour sortir est : %d\n", (int)((float)nb_pas/(float)nb_sortie));
        printf("\n----------------------------------------------------------------------------\n");
    }

    fclose(resFile);

}

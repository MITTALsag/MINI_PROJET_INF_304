#ifndef _GESTION_ERREUR_H_
#define _GESTION_ERREUR_H_



//fichier de gestion d'erreur 

void gestion_erreur_terrain(erreur_terrain e);

void affichage_position_programme(erreur_programme e);

void gestion_erreur_programme(erreur_programme e);

void gestion_erreur_interprete(resultat_inter res);

void gestion_erreur_NF(Robot* rob, FILE* fich, FILE* fich_err); /* Pour les evenement N et F */

void gestion_erreur_event(char event, resultat_inter res, Robot* rob, FILE* fich, FILE* fich_err); /* Pour gerer les evenment attendu */

#endif
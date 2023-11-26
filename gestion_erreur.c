#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"


void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_FICHIER_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_FICHIER_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_DOMAINE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_DOMAINE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LARGEUR_TROP_GRANDE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LARGEUR_TROP_PETITE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_MOIN_DE_LIGNE:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_TROP_DE_LIGNE:
    printf("Erreur lecture du terrain : lignes en trop\n");
    exit(1);
  case ERREUR_CASE_DEPART:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

void gestion_erreur_interprete(resultat_inter res) {
  /* Affichage du résultat */
  switch (res) {
    case OK_ROBOT:
      printf("Robot sur une case libre, programme non terminé (ne devrait pas arriver)\n");
      break;
    case SORTIE_ROBOT:
      printf("Le robot est sorti :-)\n");
      break;
    case ARRET_ROBOT:
      printf("Robot sur une case libre, programme terminé :-/\n");
      break;
    case PLOUF_ROBOT:
      printf("Le robot est tombé dans l'eau :-(\n");
      break;
    case CRASH_ROBOT:
      printf("Le robot s'est écrasé sur un rocher X-(\n");
      break;
    case ERREUR_PILE_VIDE:
      printf("ERREUR : pile vide\n");
      break;
    case ERREUR_ADRESSAGE:
      printf("ERREUR : erreur d'adressage\n");
      break;
    case ERREUR_DIVISION_PAR_ZERO:
      printf("ERREUR : division par 0\n");
      break;
    }
}

void gestion_erreur_NF(Robot* rob, FILE* fich, FILE* fich_err){
  int x, y; /* recupere la position attendu du robot */
  int xr, yr; /* position reel du robot*/
  char o; /* recupere l'orintation attendu du robot */
  Orientation or; /* orientation reel du robot */

  fscanf(fich , "%d %d\n", &x, &y);
  fscanf(fich , "%c\n", &o);

  position(rob, &xr, &yr);
  or = orient(rob);

  if (xr != x || yr != y){
    printf(RED"La position attendu du robot est (%d, %d) alors que la position reel du robot est (%d, %d)\n"RESET, x, y, xr, yr);
    fprintf(fich_err, "-La position attendu du robot est (%d, %d) alors que la position reel du robot est (%d, %d)\n", x, y, xr, yr );
  }
  else {
    printf(GRN"Position attendu : Ok\n"RESET);
    fprintf(fich_err, "-Position attendu : Ok\n");
  }
  switch(or){
    case Nord:
      if (o != 'N'){
        printf(RED"L'orientaion du robot attendu Nord  mais l'orientation reel est %c.\n"RESET, o);
        fprintf(fich_err, "-L'orientaion du robot attendu Nord  mais l'orientation reel est %c.\n", o);
      }
      else {
        printf(GRN"Orientation attendu : OK\n"RESET);
        fprintf(fich_err, "-Orientation attendu : OK\n");
      }
      break;
    case Sud:
      if (o != 'S'){
        printf(RED"L'orientaion du robot attendu Sud  mais l'orientation reel est %c.\n"RESET, o);
        fprintf(fich_err, "-L'orientaion du robot attendu Sud  mais l'orientation reel est %c.\n", o);
      }
      else {
        printf(GRN"Orientation attendu : OK\n"RESET);
        fprintf(fich_err, "-Orientation attendu : OK\n");
      }
      break;
    case Est:
      if (o != 'E'){
        printf(RED"L'orientaion du robot attendu Est  mais l'orientation reel est %c.\n"RESET, o);
        fprintf(fich_err,"L'orientaion du robot attendu Est  mais l'orientation reel est %c.\n", o);
      }
      else {
        printf(GRN"Orientation attendu : OK\n"RESET);
        fprintf(fich_err, "-Orientation attendu : OK\n");
      }
      break;
    case Ouest:
      if (o != 'O'){
        printf(RED"L'orientaion du robot attendu Ouest mais l'orientation reel est %c.\n"RESET, o);
        fprintf(fich_err,"L'orientaion du robot attendu Ouest mais l'orientation reel est %c.\n", o);
      }
      else {
        printf(GRN"Orientation attendu : OK\n"RESET);
        fprintf(fich_err, "-Orientation attendu : OK\n");
      }
      break;
  }
}


/* Pour curiostity_test, pour les evenement */
void gestion_erreur_event(char event, resultat_inter res, Robot* rob, FILE* fich, FILE* fich_err){
  /* Affichage des resultats */
  switch(event){
    case 'N':
      if (res == ARRET_ROBOT || res == OK_ROBOT){
        printf(GRN"Evenement : OK\n"RESET);
        fprintf(fich_err, "-Evenement : OK\n");
        gestion_erreur_NF(rob, fich, fich_err);
      }
      else{ 
        printf(RED"Erreur : le robot n'a pas terminer sur une case libre\n"RESET);
        fprintf(fich_err, "-Erreur : le robot n'a pas terminer sur une case libre\n");
      }
      break;
    case 'F':
      if (res != OK_ROBOT){ /* si le robot est soit sorti s'est crashé, est tombé dans l'eau ou le programme est finis*/
        printf(GRN"Evenement : OK\n"RESET);
        fprintf(fich_err, "-Evenement : OK\n");
        gestion_erreur_NF(rob, fich, fich_err);
      }
      else{ 
        printf(RED"Erreur : le programme n'est pas finis\n"RESET);
        fprintf(fich_err, "-Erreur : le programme n'est pas finis\n");
      }
      break;
    case 'S':
      if (res == SORTIE_ROBOT){
        printf(GRN"Evenement : OK\n"RESET);
        fprintf(fich_err, "-Evenement : OK\n");
      }
      else{ 
        printf(RED"Erreur : le robot n'est pas sortie du terrain.\n"RESET);
        fprintf(fich_err, "-Erreur : le robot n'est pas sortie du terrain.\n");
      }
      break;
    case 'O':
      if (res == CRASH_ROBOT){
        printf(GRN"Evenement : OK\n"RESET);
        fprintf(fich_err, "-Evenement : OK\n");
      }
      else{ 
        printf(RED"Erreur : le robot n'a pas rencontré d'obstacle\n"RESET);
        fprintf(fich_err, "-Erreur : le robot n'a pas rencontré d'obstacle\n");
      }
      break;
    case 'P':
      if (res == PLOUF_ROBOT){
        printf(GRN"Evenement : OK\n"RESET);
        fprintf(fich_err, "-Evenement : OK\n");
      }
      else{ 
        printf(RED"Erreur : le robot n'est pas tombé dans l'eau\n"RESET);
        fprintf(fich_err, "-Erreur : le robot n'est pas tombé dans l'eau\n");
      }
      break;
  }
}
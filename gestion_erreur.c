#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>

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
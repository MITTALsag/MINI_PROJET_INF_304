#include "observateur.h"



bool detection_M(Programme* prog){

    return prog->tab[prog->lg].cmd == Mesure;

}


bool detection_groupe(Programme* prog, int courant){

    return prog->tab[courant].cmd == DebutBloc;
    

}
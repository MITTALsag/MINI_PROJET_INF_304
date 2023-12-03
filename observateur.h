#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_


#include <stdio.h>
#include <stdbool.h>

#include "programme.h"
#include "type_pile.h"

/* detecte si dans le programme la prochaine commande est M*/
bool detection_M(Programme* prog);

bool detection_groupe(Programme* prog, int courant);

#endif
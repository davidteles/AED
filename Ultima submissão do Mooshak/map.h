/*
//  map.h
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/

#include "database.h"

/*
 This function reads the file with the map creating a map with a 3d matrix, a list with al the acess and
 the size of the map.
 It receives a pointer to the file, to the map, to acess list, to the size, number of entries and number of exits.
 It returns a a pointer to the complete map.
 */
EDGE *** readmap(FILE *fp, EDGE ***cube, LinkedList **acesslist, POSITION *size, int *entries, int *exits);
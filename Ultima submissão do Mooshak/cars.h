/*
//  cars.h
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/

#ifndef ____cars__
#define ____cars__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "map.h"

/*******************************************************************************************************
 *  Function: ReadCarEntry
 *
 *  Description: Le o ficheiro de entrada de viaturas no parque e cria lista de carros
 *
 *  Arguments: ficheiro de leitura
 *
 *  Return value: ponteiro para lista de carros
 ******************************************************************************************************/
LinkedList * ReadCarEntry(FILE * fp);

/*******************************************************************************************************
 *  Function: findcar
 *
 *  Description: percorre a lista de carros e procura pelo nome
 *
 *  Arguments: lista de carros, nome do carro a encontrar
 *
 *  Return value: ponteiro para carro encontrado ou NULL se nao for encontrado
 ******************************************************************************************************/
LinkedList *findcar(LinkedList *pointer, char *name);
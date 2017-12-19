/*
//  restrictions.h
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/

#ifndef ____restrictions__
#define ____restrictions__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "cars.h"


/*******************************************************************************************************
 *  Function: ReadRestrictions
 *
 *  Description: Le o ficheiro de restriçoes e atualiza a lista de restriçoes com o valor de inicio e
 *               fim da restriçao em cada ponto da estrutura EDGE
 *
 *  Arguments: ficheiro de leitura; estrutura de dados EDGE; tamanho do parque
 *
 *  Return value: estrutura de dados EDGE
 ******************************************************************************************************/
LinkedList * ReadRestrictions(FILE * fp);


/******************************************************************************************************
 *  Function: Restrict
 *
 *  Description: percorre a lista de restriçoes e atualiza o mapa com as restriçoes que irao
 *               estar ativas num determinado instante (tempo de entrada do carro)
 *
 *  Arguments: mapa; lista de restriçoes; tempo; tamanho do mapa
 *
 *  Return value: head of the list
 ******************************************************************************************************/
LinkedList *Restrict(EDGE ***edge, LinkedList *reslist, int time, POSITION size, int *lastrest);



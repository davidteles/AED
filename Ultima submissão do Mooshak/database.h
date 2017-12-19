/*
//  database.h
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/

#ifndef ____database__
#define ____database__


#include "LinkedList.h"

#endif /* defined(____database__) */

/*estrutura com coordenadas*/
typedef struct POSITION_{
    int x;
    int y;
    int z;
}POSITION;

typedef struct _CAR CAR;

typedef struct _RES RES;

typedef struct EDGE_ EDGE;

typedef struct ACESS_ ACESS;

typedef struct GRAPH_ GRAPH;

/*retorna o caracter do ponto (se e estrada, estacionamento,parede,etc)*/
char gettype(EDGE *pointer);

/*escreve na estrutura edge se o ponto tem ou nao restriçao, dependendo do argumento a*/
void writerestriction(int a, EDGE *pointer);

/*atualiza o caracter do ponto do mapa*/
void writetype(char a, EDGE *pointer);

/*escreve a distancia do ponto ao lugar de estacionamento*/
void writedistance(int a, EDGE *pointer);

/*retorna a distancia do ponto ao lugar de estacionamento*/
int getdistanceedge(EDGE *pointer);

/*retorna ponto da estrutura EDGE indicada por pos*/
EDGE *getedgepointer(EDGE ***cube,POSITION pos);

/*retorna o ponto da estrutura EDGE indicada pelas coordenadas */
EDGE *getedgepointercart(EDGE ***cube, int x, int y, int z);

/*retorna a variavel valid da estrutura ACESS*/
int getvalid(ACESS *pointer);

/*altera a variavel valid da estrurura ACESS*/
void changevalid(int time, ACESS *pointer);

/*escreve nome do acesso */
void getacessname(ACESS *pointer, char *name);

/*retorna posiçao do acesso*/
POSITION getacessposition(ACESS *pointer);

/*retorna tipo de acesso*/
char getacesstype(ACESS *pointer);

/*liberta memoria do acesso*/
void freeacess(ACESS *pointer);

/*grava o ponteiro para o inicio do grafo na estrutura ACESS*/
void writegraph(ACESS *pointer, GRAPH *first);

/*retorna posiçao do vertice do grafo e este existir*/
POSITION getmyposition(GRAPH *pointer);

/*retorna lista dos vertices descendestes do vertice apontado pelo ponteiro do argumento*/
LinkedList *getlist(GRAPH *pointer);

/*retorna ponto da EDGE correspondente ao vertice do grafo*/
EDGE *getcurrentedge(GRAPH *pointer);

/*retorna a distancia do vertice ao estacionamento*/
int getdistancegraph(GRAPH *pointer);

/*atualiza a distancia do vertice ao estacionamento*/
void changedistance(GRAPH *pointer, int newvalue);

/*adiciona vertice aos descendentes do vertice apontado por pointer*/
void addtolist(GRAPH *pointer, GRAPH *new);

/*retorna o vertice do grafo correspondente ao acesso*/
GRAPH *getacessgraph(ACESS *pointer);

/*retorna o nome do carro*/
void getcarname(CAR *pointer, char *name);

/*retorna a entrada do carro*/;
POSITION getcarentry(CAR*pointer);

/*retorna o estacionamento de destino do carro*/
POSITION getcardest(CAR*pointer);

/*retorna o tipo de acesso do carro*/
char getcartype(CAR *pointer);

/*retorna o tempo de entrada no parque do carro */
int getcartime(CAR *pointer);

/*escreve a posiçao do estacionamento de destino do carro*/
void writecardest(POSITION a, CAR *pointer);

/*retorna o tempo de inicio da restriçao*/
int getresstart(RES *pointer);

/*retorna o tempo de fim da restriçao*/
int getresstop(RES *pointer);

/*retorna a posiçao onde a restriçao e ativa*/
POSITION getrespos(RES *pointer);

/*verifica se duas posiçoes sao iguais*/
int compareposition(POSITION pos1, POSITION pos2);

/*retorna a variavel restrictions do ponto*/
int checkrestrictions(EDGE *pointer);

/*liberta memoria da variavel do tipo RES*/
void freerestriction(RES *pointer);

/*liberta memoria da variavel do tipo GRAPH*/
void freepoint(GRAPH *pointer);

/*liberta memoria da variavel do tipo CAR*/
void freecar(CAR *pointer);

/*altera o valor do ponteiro para NULL*/
void resetacessgraph(ACESS *pointer);

/*liberta memoria da variavel do tipo POSITION*/
void freeposition(POSITION *pointer);

/*******************************************************************************************************
 *  Function: newedgecube
 *
 *  Description: aloca memoria para estrutura de dados EDGE
 *
 *  Arguments: dimensoes do parque
 *
 *  Return value: estrutura EDGE
 ******************************************************************************************************/
EDGE *** newedgecube(int x, int y, int z);

/******************************************************************************************************
 *  Function: freeedgecube
 *
 *  Description: liberta a memoria da estrutura de dados EDGE
 *
 *  Arguments: estrutura EDGE; tamanho do parque
 *
 *  Return value: void
 ******************************************************************************************************/
void freeedgecube(EDGE ***pointer,POSITION size);

/*******************************************************************************************************
 *  Function: newacess
 *
 *  Description: cria estrutura do tipo ACESS
 *
 *  Arguments: nome do acesso; posiçao do acesso; tipo de acesso
 *
 *  Return value: ponteiro para estrutura criada
 ******************************************************************************************************/
ACESS * newacess(char name[4],POSITION pos, char c);

/*******************************************************************************************************
 *  Function: newpoint
 *
 *  Description: insere vertice no grafo correspondente ao ponto da EDGE
 *
 *  Arguments: posiçao ; ponteiro para o ponto a ser inserido no grafo
 *
 *  Return value: ponteiro para vertice do grafo
 ******************************************************************************************************/
GRAPH *newpoint(POSITION pos);

/*******************************************************************************************************
 *  Function: resetcubedistance
 *
 *  Description: reiniciliza as distancias de todos os pontos da EDGE do parque
 *
 *  Arguments: estrutura EDGE; tamanho do parque
 *
 *  Return value: void
 ******************************************************************************************************/
void resetcubedistance(EDGE ***cube, POSITION size);

/*******************************************************************************************************
 *  Function: newcar
 *
 *  Description: cria uma nova estrutura do tipo CAR
 *
 *  Arguments: nome do carro; posiçao de entrada; tipo de acesso do carro; tempo de de entrada do carro
 *
 *  Return value: ponteiro para a estrutura criada
 ******************************************************************************************************/
CAR * newcar(char name[4],POSITION pos, char type, int tempo);

/*******************************************************************************************************
 *  Function: newres
 *
 *  Description: cria uma nova estrutura do tipo RES
 *
 *  Arguments: nome do carro; posiçao de entrada; tipo de acesso do carro; tempo de de entrada do carro
 *
 *  Return value: ponteiro para a estrutura criada
 ******************************************************************************************************/
RES * newres(POSITION pos, int start, int stop);

/*guarda a said usada*/
void saveexit(CAR *pointer ,ACESS *exit);

/*retorna a saida usada*/
ACESS *getexit(CAR *pointer);

/*Returns a pointer to the list of parking spaces*/
LinkedList *getparking(ACESS *pointer);

/*Saves the pointer to the parking list*/
void saveparking(ACESS *pointer, LinkedList *new);
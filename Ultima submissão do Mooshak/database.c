/*
//  database.c
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/


#include "database.h"

/*estrutura com informaçao de cada ponto do mapa*/
struct EDGE_ {
    int distance; /*distancia a entrada*/
    char type; /*caracter do mapa, distingue estrada, lugar, parede, etc*/
    int restriction; /*indica se o ponto do mapa tem restriçao(1) ou nao(0)*/
 
};
/*estrutura com informaçao de cada vertice do grafo*/
struct GRAPH_ {
    int distance; /*distancia do ponto a entrada, usado para calcular lugar mais proximo*/
    POSITION pos; /*posiçao no mapa do vertice*/
    LinkedList *Next; /*ponteiro para proximo vertice do grafo*/
};

/*estrutura com infomaçao de entradas e acessos*/
struct ACESS_ {
    char name[6]; /*nome da entrada ou acesso*/
    POSITION pos; /*posiçao*/
    char type; /*tipo de acesso ou - se for uma entrada*/
    GRAPH *pointer; /*ponteiro para o vertice do grafo*/
    int valid; /*instante de tempo em que foi criado o grafo com este acesso*/
    LinkedList *parking;
};

/*estrutura com informaçao de um carro*/
struct _CAR{
    char name[10]; /*nome do carro*/
    int tempo; /*instande de entrada no parque*/
    char type; /*tipo de acesso pretendido*/
    POSITION entry; /*posiçao de entrada do parque*/
    POSITION dest; /*posiçao do estacionamento calculado*/
    ACESS *exit; /*Apontador para a saida usada*/
};

/*estrutura com tempo de inicio e fim e posiçao da restriçao*/
struct _RES{
    int start;
    int stop;
    POSITION pos;
};

/*retorna o caracter do ponto (se e estrada, estacionamento,parede,etc)*/
char gettype(EDGE *pointer){
    return (pointer->type);
}

/*escreve na estrutura edge se o ponto tem ou nao restriçao, dependendo do argumento a*/
void writerestriction(int a, EDGE *pointer){
    pointer->restriction=a;
}

/*atualiza o caracter do ponto do mapa*/
void writetype(char a, EDGE *pointer){
    pointer->type=a;
}

/*escreve a distancia do ponto ao lugar de estacionamento*/
void writedistance(int a, EDGE *pointer){
    pointer->distance=a;
}

/*retorna a distancia do ponto ao lugar de estacionamento*/
int getdistanceedge(EDGE *pointer){
    return (pointer->distance);
}



/*******************************************************************************************************
 *  Function: newedgecube
 *
 *  Description: aloca memoria para estrutura de dados EDGE
 *
 *  Arguments: dimensoes do parque
 *
 *  Return value: estrutura EDGE
 ******************************************************************************************************/
EDGE *** newedgecube(int x, int y, int z){
    EDGE ***new;
    int i, j;
    new=(EDGE***) malloc(x*sizeof(EDGE**));
    for (i=0; i<x; i++) {
        new[i]=(EDGE**)malloc(y*sizeof(EDGE*));
        for(j=0; j<y; j++){
            new[i][j]=(EDGE*)malloc((z)*sizeof(EDGE));;
            
        }
    }
    return new;
}

/*******************************************************************************************************
 *  Function: freeedgecube
 *
 *  Description: liberta a memoria da estrutura de dados EDGE
 *
 *  Arguments: estrutura EDGE; tamanho do parque
 *
 *  Return value: void
 ******************************************************************************************************/
void freeedgecube(EDGE ***pointer, POSITION size){
    int i, j;
    for (i=0; i<size.x; i++) {
        for(j=0; j<size.y; j++){
            free(pointer[i][j]);
        }
        free(pointer[i]);
    }
    free(pointer);
}

/*retorna ponto da estrutura EDGE indicada por pos*/
EDGE *getedgepointer(EDGE ***cube,POSITION pos){
    return &cube[pos.x][pos.y][pos.z];
}

/*retorna o ponto da estrutura EDGE indicada pelas coordenadas */
EDGE *getedgepointercart(EDGE ***cube, int x, int y, int z){
    return &cube[x][y][z];
}

/*******************************************************************************************************
 *  Function: newacess
 *
 *  Description: cria estrutura do tipo ACESS
 *
 *  Arguments: nome do acesso; posiçao do acesso; tipo de acesso
 *
 *  Return value: ponteiro para estrutura criada
 ******************************************************************************************************/
ACESS * newacess(char name[6],POSITION pos, char type){
    ACESS *new;
    new=(ACESS*)malloc(sizeof(ACESS));
    strcpy(new->name, name);
    new->pos=pos;
    new->type=type;
    new->pointer=NULL;
    new->valid=1;
    new->parking=NULL;
    return new;
}

/*Returns a pointer to the list of parking spaces*/
LinkedList *getparking(ACESS *pointer){
    return pointer->parking;
}

/*Saves the pointer to the parking list*/
void saveparking(ACESS *pointer, LinkedList *new){
    pointer->parking=new;
}

/*retorna a variavel valid da estrutura ACESS*/
int getvalid(ACESS *pointer){
    return pointer->valid;
}

/*altera a variavel valid da estrurura ACESS*/
void changevalid(int time, ACESS *pointer){
    pointer->valid=time;
}

/*escreve nome do acesso */
void getacessname(ACESS *pointer, char *name){
    strcpy(name, pointer->name);
}

/*retorna posiçao do acesso*/
POSITION getacessposition(ACESS*pointer){
    return pointer->pos;
}

/*retorna tipo de acesso*/
char getacesstype(ACESS *pointer){
    return pointer->type;
}

/*liberta memoria do acesso*/
void freeacess(ACESS *pointer){
    free(pointer);
}

/*grava o ponteiro para o inicio do grafo na estrutura ACESS*/
void writegraph(ACESS *pointer, GRAPH *first){
    pointer->pointer=first;
}

/*******************************************************************************************************
 *  Function: newpoint
 *
 *  Description: insere vertice no grafo correspondente ao ponto da EDGE
 *
 *  Arguments: posiçao ; ponteiro para o ponto a ser inserido no grafo
 *
 *  Return value: ponteiro para vertice do grafo
 ******************************************************************************************************/
GRAPH *newpoint(POSITION pos){
    GRAPH *new;
    new=(GRAPH*)malloc(sizeof(GRAPH));
    new->distance=0;
    new->pos=pos;
    new->Next=initLinkedList();
    return new;
}

/*retorna posiçao do vertice do grafo e este existir*/
POSITION getmyposition(GRAPH *pointer){
    if (pointer==NULL) {
        exit(0);
    }
    return pointer->pos;
}

/*retorna lista dos vertices descendestes do vertice apontado pelo ponteiro do argumento*/
LinkedList *getlist(GRAPH *pointer){
    return pointer->Next;
}


/*retorna a distancia do vertice ao estacionamento*/
int getdistancegraph(GRAPH *pointer){
    if (pointer==NULL) {
        return -1;
    }
    return pointer->distance;
}

/*atualiza a distancia do vertice ao estacionamento*/
void changedistance(GRAPH *pointer, int newvalue){
    if (pointer==NULL) {
        return;
    }
    pointer->distance=newvalue;
}

/*adiciona vertice aos descendentes do vertice apontado por pointer*/
void addtolist(GRAPH *pointer, GRAPH *new){
    pointer->Next=insertUnsortedLinkedList(pointer->Next, new);
}

/*retorna o vertice do grafo correspondente ao acesso*/
GRAPH *getacessgraph(ACESS *pointer){
    if (pointer==NULL) {
        return NULL;
    }
    return pointer->pointer;
}

/*******************************************************************************************************
 *  Function: resetcubedistance
 *
 *  Description: reiniciliza as distancias de todos os pontos da EDGE do parque
 *
 *  Arguments: estrutura EDGE; tamanho do parque
 *
 *  Return value: void
 ******************************************************************************************************/
void resetcubedistance(EDGE ***cube, POSITION size){
    int i, j, k;
    for (i=0; i<size.x; i++) {
        for (j=0; j<size.y; j++) {
            for (k=0; k<size.z; k++) {
                cube[i][j][k].distance=-1;
            }
        }
    }
}

/*retorna o nome do carro*/
void getcarname(CAR *pointer, char *name){
    strcpy(name, pointer->name);
}

/*retorna a entrada do carro*/
POSITION getcarentry(CAR*pointer){
    return pointer->entry;
}

/*retorna o estacionamento de destino do carro*/
POSITION getcardest(CAR*pointer){
    return pointer->dest;
}

/*retorna o tipo de acesso do carro*/
char getcartype(CAR *pointer){
    return pointer->type;
}

/*retorna o tempo de entrada no parque do carro */
int getcartime(CAR *pointer){
    return pointer->tempo;
}

/*escreve a posiçao do estacionamento de destino do carro*/
void writecardest(POSITION a, CAR *pointer){
    pointer->dest.x=a.x;
    pointer->dest.y=a.y;
    pointer->dest.z=a.z;
}

/*******************************************************************************************************
 *  Function: newcar
 *
 *  Description: cria uma nova estrutura do tipo CAR
 *
 *  Arguments: nome do carro; posiçao de entrada; tipo de acesso do carro; tempo de de entrada do carro
 *
 *  Return value: ponteiro para a estrutura criada
 ******************************************************************************************************/
CAR * newcar(char name[4],POSITION pos, char type, int tempo){
    CAR *new;
    POSITION aux;
    new=(CAR*)malloc(sizeof(CAR));
    aux.x=-1;
    aux.y=-1;
    aux.z=-1;
    strcpy(new->name, name);
    new->entry=pos;
    new->dest=aux;
    new->type=type;
    new->tempo=tempo;
    return new;
}

/*******************************************************************************************************
 *  Function: newres
 *
 *  Description: cria uma nova estrutura do tipo RES
 *
 *  Arguments: nome do carro; posiçao de entrada; tipo de acesso do carro; tempo de de entrada do carro
 *
 *  Return value: ponteiro para a estrutura criada
 ******************************************************************************************************/
RES * newres(POSITION pos, int start, int stop){
    RES*new;
    new=(RES*)malloc(sizeof(RES));

    new->pos=pos;
    new->start=start;
    new->stop=stop;
    return new;
}

/*retorna o tempo de inicio da restriçao*/
int getresstart(RES *pointer){
    return pointer->start;
}

/*retorna o tempo de fim da restriçao*/
int getresstop(RES *pointer){
    return pointer->stop;
}

/*retorna a posiçao onde a restriçao e ativa*/
POSITION getrespos(RES *pointer){
    return pointer->pos;
}

/*verifica se duas posiçoes sao iguais*/
int compareposition(POSITION pos1, POSITION pos2){
    if (pos1.x==pos2.x && pos1.y==pos2.y && pos1.z==pos2.z) {
        return 1;
    }
    return 0;
}

/*retorna a variavel restrictions do ponto*/
int checkrestrictions(EDGE *pointer){
    return pointer->restriction;
}

/*liberta memoria da variavel do tipo RES*/
void freerestriction(RES *pointer){
    free(pointer);
}

/*liberta memoria da variavel do tipo GRAPH*/
void freepoint(GRAPH *pointer){
    free(pointer);
}

/*liberta memoria da variavel do tipo CAR*/
void freecar(CAR *pointer){
    free(pointer);
}

/*altera o valor do ponteiro para NULL*/
void resetacessgraph(ACESS *pointer){
    pointer->pointer=NULL;
}

/*liberta memoria da variavel do tipo POSITION*/
void freeposition(POSITION *pointer){
    free(pointer);
}

/*guarda a said usada*/
void saveexit(CAR *pointer ,ACESS *exit){
    pointer->exit=exit;
}

/*retorna a saida usada*/
ACESS *getexit(CAR *pointer){
    return pointer->exit;
}

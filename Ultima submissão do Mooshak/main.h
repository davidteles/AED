/*
//  main.h
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/

#ifndef gestor_main_h
#define gestor_main_h
#define NEW

#endif
#include "restrictions.h"
#include "escreve_saida.h"

/*This struct is only use to return when finding where to park the car*/
typedef struct DESTINATION_{
    int distancesum;
    GRAPH *current;
}DESTINATION;

/*Thi struct is only use to return the heads of the lists of the function run*/
typedef struct LISTS_{
    LinkedList *cars;
    LinkedList *restrictions;
    LinkedList *waitinglist;
}LISTS;


/*
 The funcion run is the second most important funcion on this program.
 It's the back bone that does all the heavy lifting to get the job done, that is why it is massive.
 It receives a pointer to the exit file, the map, list with acess (where to enter and exit),
 with all the cars records, the restrictons and also to the size of the map.
 (Names of variables self explanatory)
 
 It returns a struct with three pointers to the heads of the lists so they can be free in main.
 
 
 */
LISTS run(FILE *fout, EDGE ***cube, LinkedList *acesslist, LinkedList *cars, POSITION size, LinkedList *restrictions);
/*
 Simple function that free the path list
 Receives a pointer to the head of the list and returns nothing
 */
void freepath(LinkedList *pointer);
/*
 This is the function that deals with the output.
 It works in a recursive way!!
 This funcion only works if there is any incorrect moves
 Recives as arguments a pointer to the output file, to the list with the path, to the car in question.
 It also receives one int that tells what direction it is should write, a char with the last move and int that counts ow many times is run.
 
 It returns how many times the function was called
 */
int output(FILE *fp, LinkedList *path, CAR *car, int direction, char cart, int counter);
/*
 This function creats graphs using minimun suport trees.
 Using some clever thinking (lets see if it true) this function creats trees by adding
 elements right next to the last element added.
 So it adds the elements at distance 1, 2, 3, ..., n-1, n until there is no more elements to be added
 It receives as arguments a pointer to the map ant to the acess, it also reveives the size of the map.
 
 It returns nothing
 */
void newgraph(EDGE *** map, void *from, POSITION size);
/*
 This function adds points to the graph and creats a list with all the elements that where added.
 It receives has arguments a pointer to the map, to one point of the graph, the size of the map
 and the head of the list where the points will be also added. It also receives a value to increment
 to the distance.
 It returns the head of the list.
 */
LinkedList * addtograph(EDGE *** map, GRAPH *current, POSITION size, LinkedList * head, int counter, ACESS *pointer);
/*
 This funcion is used to find the closest parking space to in all the map.
 It receives a pointer to the list os acess, a char for the type of acess the user want,
 a pointer that will use to pass the closest acess (exit), a pointer to the map, the size o f the map,
 a pointer to the graph of the entrie and the current instante.
 It returns the closest parking space.
 */
POSITION findnearemptyslot(LinkedList *acesslist, char type, ACESS **exit, EDGE ***map, POSITION side, ACESS *entrie, int time, int lastresalt);

/*
 This function load the path from the entrie or exit to a position.
 It receives a pointer to the graph and other to the map and the position that it wnats to reach.
 It returns apointer to a list with all the positions.
 */
LinkedList *getpath(GRAPH *pointer,POSITION pos, EDGE ***map);
/*
 This function loads the distance from the graph to the map.
 It receives a pointer to the graph and to the map.
 It returns nothing.
 Works in a recursive way.
 */
void loaddistancetomap(GRAPH *pointer, EDGE ***map);
/*
 This function free's every element of the graph.
 Receives a pointer to the graph and retuns nothing.
 Works in a recursive way.
 */
void freegraph(GRAPH *pointer);


/*
 This function searches for the closest empty parking space to only one acess at one time.
 It receives as arguments a pointer to the exit graph end a pointer to the map.
 It returns a struct with the minimum distance encouter and the point of the graph os that postion.
 Recursive search
 */
DESTINATION searchold(GRAPH *pointer, EDGE ***map);
/*
 This function loads the distance from the graph to the map.
 It receives a pointer to the graph and to the map.
 It returns nothing.
 Works in a recursive way.
 */
void loaddistancetomapold(GRAPH *pointer, EDGE ***map);
/*
 This function searches for the closest empty parking space to only one acess at one time.
 It receives as arguments a pointer to a list with parking spaces and a pointer to the map.
 It returns a struct with the minimum distance encouter and the point of the graph os that postion.
 */
DESTINATION searchnew(LinkedList *pointer, EDGE ***map);
/*
 This function loads the distance from the graph to the map.
 It receives a pointer to a list with parking spaces and to the map.
 It returns nothing.
 */
void loaddistancetomapnew(LinkedList *pointer, EDGE ***map);


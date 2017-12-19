/*
//  cars.c
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/


#include "cars.h"

/*******************************************************************************************************
 *  Function: ReadCarEntry
 *
 *  Description: Le o ficheiro de entrada de viaturas no parque e cria lista de carros
 *
 *  Arguments: ficheiro de leitura
 *
 *  Return value: ponteiro para lista de carros
 ******************************************************************************************************/
LinkedList * ReadCarEntry(FILE * fp)
{
  char name[10];
  int tempo;
  char type;
  POSITION pos;
  int flag = 0;

  LinkedList * carlist;

  /* Initialize linked list                                       */
  carlist = NULL;

  /* Cycle through file rows until end of file                                    */  
    while(!feof(fp))
    {

      flag = fscanf(fp, "%s %d %c %d %d %d", name, &tempo, &type, &pos.x ,&pos.y, &pos.z);

      /*se apenas forem lidos 3 caracteres (saida de carro conhecido)*/
      if(flag == 3)
      {
        pos.x = -1;
        pos.y = -1;
        pos.z = -1;
      }
        
      /*insere entrada ou saida de carro na lista de carros*/
      carlist=insertUnsortedLinkedList(carlist, (void*)newcar(name, pos, type, tempo));

    }
    fclose(fp);
  return carlist;
}

/*******************************************************************************************************
 *  Function: findcar
 *
 *  Description: percorre a lista de carros e procura pelo nome
 *
 *  Arguments: lista de carros, nome do carro a encontrar
 *
 *  Return value: ponteiro para carro encontrado ou NULL se nao for encontrado
 ******************************************************************************************************/
LinkedList *findcar(LinkedList *pointer, char *name){
    LinkedList *aux;
    char buffer[10];
    for (aux=pointer; aux!=NULL; aux=getNextNodeLinkedList(aux)) {
        getcarname(getItemLinkedList(aux), buffer);
        if (strcmp(name, buffer)==0) {
            return aux;
        }
        
    }
    return NULL;
}

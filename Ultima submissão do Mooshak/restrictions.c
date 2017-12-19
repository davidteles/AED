/*
//  restrictions.c
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/


#include "restrictions.h"



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
LinkedList * ReadRestrictions(FILE * fp)
{
  int start, stop;
  POSITION pos;
  int flag = 0;

  LinkedList * reslist;

  reslist = NULL;

  /* Cycle through file rows     */  
    while(!feof(fp))
    {

      flag = fscanf(fp, "%*s %d %d %d %d %d", &start, &stop, &pos.x ,&pos.y, &pos.z);

      /*se apenas forem lidos 4 caracteres(restriçao de um piso inteiro)*/
      if(flag == 3)
      {
      	pos.z = pos.x;
        pos.x = -1;
        pos.y = -1;
      }

      /*insere restriçao na lista de restriçoes*/
      reslist = insertUnsortedLinkedList(reslist, (Item*)newres(pos, start, stop));

    }
    fclose(fp);
  return reslist;
}

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
LinkedList *Restrict(EDGE ***edge, LinkedList *reslist, int time, POSITION size, int *lastrest)
{
  LinkedList *aux, *next;
  POSITION pos, floor;
    EDGE *save;
    int j, k;
    pos.x=0;
    pos.y=0;
    pos.z=0;
    floor=pos;
  for(aux=reslist; aux != NULL; aux=next)
  {
    
    /*Load theposition of the restriction*/
    pos = getrespos(getItemLinkedList(aux));
      /*To prevent some input errors*/
      if (pos.x>=size.x || pos.y>=size.y || pos.z>=size.z) {
          /*Load next from the list*/
          next=getNextNodeLinkedList(aux);
          continue;
      }
    /*If the restrictions as started but not ended load it to the map*/
    if(getresstart(getItemLinkedList(aux)) <= time && (getresstop(getItemLinkedList(aux)) >= time || getresstop(getItemLinkedList(aux))==0))
    {   /*If the x and y postion are at -1 it means that the retriction afects the all floor*/
        if (pos.x==-1 || pos.y==-1) {
            floor.z=pos.z;
            /*Run all positons of that floor*/
            for (j=0; j<size.x; j++) {
                for (k=0; k<size.y; k++) {
                    floor.x=j;
                    floor.y=k;
                    save=getedgepointer(edge, floor);
                    if (/*checkrestrictions(save)!=1 && */(gettype(save)=='.' || gettype(save)=='x')) {
                        /*Put restriction on the map*/
                        writerestriction(1, getedgepointer(edge, floor));
                        *lastrest=time;
                    } else {
                        j=size.x;
                        k=size.y;
                    }

                }
            }
        } else {
            if (/*checkrestrictions(getedgepointer(edge, pos))!=*/1) {
                /*Put restriction on the map*/
                writerestriction(1, getedgepointer(edge, pos));
                *lastrest=time;
            }

        }
    }
      /*If the restriction already ended remove from map and remove from the list*/
    if(getresstop(getItemLinkedList(aux)) < time && getresstop(getItemLinkedList(aux))!=0)
    {
        /*If the x and y postion are at -1 it means that the retriction afected the all floor*/
        if (pos.x==-1 || pos.y==-1) {
            floor.z=pos.z;
            /*Run all positons of that floor*/
            for (j=0; j<size.x; j++) {
                for (k=0; k<size.y; k++) {
                    floor.x=j;
                    floor.y=k;
                    save=getedgepointer(edge, floor);
                    if (/*checkrestrictions(save)!=0 && */(gettype(save)=='.' || gettype(save)=='x')) {
                        /*Put restriction on the map*/
                        writerestriction(0, getedgepointer(edge, floor));
                        *lastrest=time;
                    }                }
            }
        } else {
            if (/*checkrestrictions(getedgepointer(edge, pos))!=0*/1) {
                /*Put restriction on the map*/
                writerestriction(0, getedgepointer(edge, pos));
                *lastrest=time;
            }
        }
        /*Load next from the list*/
        next=getNextNodeLinkedList(aux);
        /*Remover da lista de restriçoes node*/
        reslist=removenode(reslist, aux, (void*)freerestriction);
        /*Contiue to the top so no repeat call of next*/
        continue;
    }
      if (getresstart(getItemLinkedList(aux)) > time) {
          return reslist;
      }
      next=getNextNodeLinkedList(aux);
  }
    /*Return head of the list of restrictions*/
  return reslist;
}
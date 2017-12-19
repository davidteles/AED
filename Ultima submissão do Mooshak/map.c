/*
//  map.c
//  Gestor de Parque
//
//  Created by David Teles/Joao Loureiro on 13/11/15.
//  IST181053 and IST181164
//  All rights reserved.
*/

#include "map.h"

/*
    This function reads the file with the map creating a map with a 3d matrix, a list with al the acess and
    the size of the map.
    It receives a pointer to the file, to the map, to acess list, to the size, number of entries and number of exits.
    It returns a a pointer to the complete map.
*/
EDGE *** readmap(FILE *fp, EDGE ***cube, LinkedList **acesslist, POSITION *size, int *entries, int *exits){

    POSITION pbuffer, pos;
    char nbuffer[4], tbuffer;
    int x, y, z;
    int i, j, k;
    int flags;
    /*Reads the first line of the document*/
    flags=fscanf(fp, "%d %d %d %d %d", &x, &y, &z, entries, exits);
    if (flags!=5) {
        exit(0);
    }
    size->x=x;
    size->y=y;
    size->z=z;
    
    /*Alloc the map*/
    cube=newedgecube(x, y, z);
    /*Inicialize the list of acess's*/
    *acesslist=initLinkedList();
    k=0;
    /*Read every charecter of the map and save to the map*/
    while (k<z) {
        for (i=y-1; i>=0; i--) {
            /*To get rid of the \n on the end of every line*/
            getc(fp);
            for (j=0; j<x; j++) {
                tbuffer=getc(fp);
                /*To try take care of errors on the file that contains the map*/
                while (tbuffer=='\n') {
                tbuffer=getc(fp);
                }
                
                pos.x=j;
                pos.y=i;
                pos.z=k;
                /*Safe and initialize everything*/
                writetype(tbuffer, getedgepointer(cube, pos));
                writerestriction(0, getedgepointer(cube, pos));
                writedistance(-1, getedgepointer(cube, pos));
            }
            
        }
        /*run until it find a '+'*/
        while(1){
            /*read the first element of the line*/
            flags=fscanf(fp,"%s" ,nbuffer);
            if (flags!=1) {
                exit(0);
            }
            /*Check if the string read is a '+'*/
            if (nbuffer[0]=='+') {
                break;
            }
            /*Read the other parts of the list*/
            flags=fscanf(fp, "%d %d %d %c", &pbuffer.x, &pbuffer.y, &pbuffer.z, &tbuffer);
            if (flags!=4) {
                exit(0);
            }
            /*Add info to the acess list*/
            *acesslist=insertUnsortedLinkedList(*acesslist, (Item*)newacess(nbuffer, pbuffer, tbuffer));
        }
        /*Increment the floor*/
        k++;
        
    }

    /*Close the file*/
    fclose(fp);
    /*Return a pointer to the map*/
    return cube;

}

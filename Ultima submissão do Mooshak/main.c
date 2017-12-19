/*
 //  main.c
 //  Gestor de Parque
 //
 //  Created by David Teles/Joao Loureiro on 13/11/15.
 //  IST181053 and IST181164
 //  All rights reserved.
 */

#include "main.h"

#define MAIN

int main(int argc, const char * argv[]) {
    FILE *fp, *fout, *fres, *fcars;
    LinkedList *acesslist=NULL, *cars=NULL, *restrictions=NULL, *aux=NULL;
    EDGE ***cube=NULL;
    int entries, exits, buffersize;
    POSITION size;
    LISTS final;
    char *buffer;
    /*Verify if all arguments needed to run are present and opens all the files and checks if they are found*/
    if(argc<3){
        /*puts("No arguments found!");*/
        exit(0);
    }
    
    fp=fopen(argv[1], "r");
    if (fp==NULL){
        
        /*puts("File not found!");*/
        exit(0);
    }
    
    fcars=fopen(argv[2], "r");
    if (fcars==NULL){
        /*puts("File not found!");*/
        exit(0);
    }
    if(argc==4){
        fres=fopen(argv[3], "r");
        if (fres==NULL){
            /*puts("File not found!");*/
            exit(0);
        }
        
        /*Read the restrictions files if the user put the file in the arguments*/
        restrictions=ReadRestrictions(fres);
        
    }
    /*Do some manipulation to creat and open the results file*/
    buffersize=(int)strlen(argv[1]);
    buffer=(char*)malloc(buffersize*sizeof(char)+2);
    if (buffer==NULL) {
        exit(0);
    }
    strcpy(buffer, argv[1]);
    buffer[buffersize-3]='p';
    buffer[buffersize-2]='t';
    buffer[buffersize-1]='s';
    fout=fopen(buffer, "w");
    if (fout==NULL) {
        exit(0);
    }
    /*Read the map*/
    cube=readmap(fp, cube, &acesslist, &size, &entries, &exits);
    /*Read the file with all the cars*/
    cars=ReadCarEntry(fcars);
    /*Run all the cars through the system*/

    final=run(fout, cube, acesslist, cars, size, restrictions);
    /*Frees all the memory resources left by the system*/
    for(aux=acesslist; aux!=NULL; aux=getNextNodeLinkedList(aux)){
        freeLinkedList(getparking(getItemLinkedList(aux)), NULL);
        freegraph(getacessgraph(getItemLinkedList(aux)));
        freepoint(getacessgraph(getItemLinkedList(aux)));
    }
    
    freeLinkedList(acesslist, (void*) freeacess);
    freeedgecube(cube, size);
    freeLinkedList(final.cars, (void*) freecar);
    freeLinkedList(final.restrictions, (void*)freerestriction);
    freeLinkedList(final.waitinglist, NULL);
    fclose(fout);
    free(buffer);
    /*The program run suceffuly to get to this point, with every memory allocation free the program exits*/
    exit(0);
    return 0;
}
/*
 The funcion run is the second most important funcion on this program.
 It's the back bone that does all the heavy lifting to get the job done, that is why it is massive.
 It receives a pointer to the exit file, the map, list with acess (where to enter and exit),
 with all the cars records, the restrictons and also to the size of the map.
 (Names of variables self explanatory)
 
 It returns a struct with three pointers to the heads of the lists so they can be free in main.
 
 
 */
LISTS run(FILE *fout, EDGE ***map, LinkedList *acesslist, LinkedList *cars, POSITION size, LinkedList *restrictions){
    LinkedList *auxcars, *nextcar, *auxacess, *path, *waitinglist=NULL, *helper, *wh, *nextwh;
    ACESS *acessexit, *entriebuffer=NULL;
    LISTS out;
    char buffer[10];
    CAR *wt;
    POSITION slot, aux, wtpos;
    int type=0, counter=0, time, lastresalt=0;
    char lasttype='*';
    /*Go through the list of car one by one*/
    for (auxcars=cars; auxcars!=NULL; auxcars=nextcar) {
        nextcar=getNextNodeLinkedList(auxcars);
        /*Load the restrictions to the map*/
        restrictions=Restrict(map, restrictions, getcartime(getItemLinkedList(auxcars)), size, &lastresalt);
        if (lastresalt==getcartime(getItemLinkedList(auxcars))) {
            for (wh=waitinglist; wh!=NULL; wh=nextwh) {
                nextwh=getNextNodeLinkedList(wh);
                /*See if there are cars in the waiting list*/
                if (waitinglist!=NULL) {
                    /*There cars in the wainting list so find there entrie*/
                    for (auxacess=acesslist; auxacess!=NULL; auxacess=getNextNodeLinkedList(auxacess)) {
                        if (compareposition(getcarentry(getItemLinkedList(wh)), getacessposition(getItemLinkedList(auxacess)))) {
                            break;
                        }
                    }
                    if (entriebuffer==getItemLinkedList(auxacess) && getcartype(getItemLinkedList(wh))==lasttype) {
                        continue;
                    }
                    entriebuffer=getItemLinkedList(auxacess);
                    lasttype=getcartype(getItemLinkedList(wh));
                    /*Verificar se o grafo é valido para o instante de entrada*/
                    if ((getvalid(getItemLinkedList(auxacess))<=lastresalt )|| getacessgraph(getItemLinkedList(auxacess))==NULL){
                        /*Resets all values on the map that are needed to creat the graph*/
                        resetcubedistance(map, size);
                        /*Because we are getting a new graph change the variable that tell if it is valid*/
                        changevalid(getcartime(getItemLinkedList(auxcars)), getItemLinkedList(auxacess));
                        /*Free the old graph*/
                        freegraph(getacessgraph(getItemLinkedList(auxacess)));
                        
                        /*One point left to be free*/
                        freepoint(getacessgraph(getItemLinkedList(auxacess)));
                        /*Set the pointer to NULL*/
                        resetacessgraph(getItemLinkedList(auxacess));
                        /*Free list with parking spaces*/
                        freeLinkedList(getparking(getItemLinkedList(acesslist)), NULL);
                        saveparking(getItemLinkedList(acesslist), NULL);
                        /*Creat new graph*/
                        newgraph(map, getItemLinkedList(auxacess), size);
                        
                    }
                    /*Find where to send the car and what is the best exit*/
                    slot=findnearemptyslot(acesslist, getcartype(getItemLinkedList(wh)), &acessexit, map, size, getItemLinkedList(auxacess), getcartime(getItemLinkedList(auxcars)), lastresalt);
                    if (slot.x!=-1 || slot.y!=-1 || slot.z!=-1 ) {
                        time=getcartime(getItemLinkedList(auxcars));
                        /*Load all the positions to the parking*/
                        path=getpath(getacessgraph(getItemLinkedList(auxacess)), slot ,map);
                        /*Write the path to the file*/
                        counter=output(fout, path, getItemLinkedList(wh), 0, 'w', time);
                        /*free up the memory because we dont need the path no time soon*/
                        freepath(path);
                        /*Mark the space was full*/
                        writetype('x', getedgepointer(map, slot));
                        /*Tell the car where is is parked*/
                        writecardest(slot, getItemLinkedList(wh));
                        /*Load the path to the exit*/
                        path=getpath(getacessgraph(acessexit), slot ,map);
                        /*Write the path to the file*/
                        output(fout, path, getItemLinkedList(wh), 1, 'q', counter);
                        /*Free  up the memory again*/
                        freepath(path);
                        /*No longer in waiting list so remove it from the list*/
                        waitinglist=removenode(waitinglist, wh, NULL);
                    }
                } else {
                    break;
                }
            }
        }

        /*Find out if it is dealing with and entrie or exit*/
        if (getcartype(getItemLinkedList(auxcars))!='S') {
            type=1;
            
        } else if(getcartype(getItemLinkedList(auxcars))=='S'){
            type=2;
        }
        if (restrictions==NULL && lastresalt<getcartime(getItemLinkedList(auxcars)) && waitinglist!=NULL && type==1) {
            /*Get all the info about this car to write in the output file*/
            wt=getItemLinkedList(auxcars);
            getcarname(wt, buffer);
            wtpos=getcarentry(wt);
            escreve_saida(fout, buffer , getcartime(wt), wtpos.x, wtpos.y, wtpos.z, 'i');
            /*Adicionar á lista de espera*/
            waitinglist=insertUnsortedLinkedList(waitinglist, wt);
            /*Car in waiting list so we can resume to the next element (car) of the list*/
            /*Just get the type to 0 to be shure*/
            type=0;
            continue;
        }
        /*If it is ans entrie it finds out if it is in memory and gets all the info */
        for (auxacess=acesslist; auxacess!=NULL || type!=2 ; auxacess=getNextNodeLinkedList(auxacess)) {
            if (compareposition(getcarentry(getItemLinkedList(auxcars)), getacessposition(getItemLinkedList(auxacess)))) {
                break;
            }
            
            
        }

        /*If it was an entrie*/
        if (type==1) {
            /*If there was no acess in that position something is wrong and it need to exit*/
            if(auxacess==NULL){
                exit(0);
            }
            /*Verificar se o grafo é valido para o instante de entrada*/
            if ((getvalid(getItemLinkedList(auxacess))<=lastresalt )|| getacessgraph(getItemLinkedList(auxacess))==NULL){
                /*Resets all values on the map that are needed to creat the graph*/
                resetcubedistance(map, size);
                /*Because we are getting a new graph change the variable that tell if it is valid*/
                changevalid(getcartime(getItemLinkedList(auxcars)), getItemLinkedList(auxacess));
                /*Free the old graph*/
                freegraph(getacessgraph(getItemLinkedList(auxacess)));
                /*One point left to be free*/
                freepoint(getacessgraph(getItemLinkedList(auxacess)));
                /*Set the pointer to NULL*/
                resetacessgraph(getItemLinkedList(auxacess));
                /*Free list with parking spaces*/
                freeLinkedList(getparking(getItemLinkedList(acesslist)), NULL);
                saveparking(getItemLinkedList(acesslist), NULL);
                /*Creat new graph*/
                newgraph(map, getItemLinkedList(auxacess), size);
                
            }

            /*No car in wainting list so Find where to send the car and what is the best exit*/
            slot=findnearemptyslot(acesslist, getcartype(getItemLinkedList(auxcars)), &acessexit, map, size, getItemLinkedList(auxacess), getcartime(getItemLinkedList(auxcars)), lastresalt);
            /*If the returned position was all or even any item at -1 it is interpret as the park is full*/
            if (slot.x==-1 || slot.y==-1 || slot.z==-1) {
                /*Get all the info about this car to write in the output file*/
                wt=getItemLinkedList(auxcars);
                getcarname(wt, buffer);
                wtpos=getcarentry(wt);
                escreve_saida(fout, buffer , getcartime(wt), wtpos.x, wtpos.y, wtpos.z, 'i');
                /*Adicionar á lista de espera*/
                waitinglist=insertUnsortedLinkedList(waitinglist, wt);
                /*Car in waiting list so we can resume to the next element (car) of the list*/
                /*Just get the type to 0 to be shure*/
                type=0;
                continue;
                
            }
            /*Ok the park is not full, now lets get the car to is parking space*/
            /*Load all the positions to the parking*/
            path=getpath(getacessgraph(getItemLinkedList(auxacess)), slot ,map);
            
            if(path==NULL){
                /*UPS no path something is wrong lets end now*/
                exit(0);
            }
            /*We have the path to the parking space, so now we just need to get the car to it*/
            counter=output(fout, path, getItemLinkedList(auxcars), 0, 'n',getcartime(getItemLinkedList(auxcars)));
            /*We don't need this path anymore to time to free some memory*/
            freepath(path);
            /*The space in no longer available, so mark it as such*/
            writetype('x', getedgepointer(map, slot));
            /*Tell the car is final place so the driver can find it when it is time to leave*/
            writecardest(slot, getItemLinkedList(auxcars));
            /*Save it ecause it can be helpfull in the future*/
            saveexit(getItemLinkedList(auxcars),acessexit);
            /*Time to get the path to outside the park*/
            path=getpath(getacessgraph(acessexit), slot ,map);
            /*Now we need to tell the driver the path*/
            output(fout, path, getItemLinkedList(auxcars), 1, 'q',counter);
            /*OK we dont need this to so just free some memory*/
            freepath(path);
            
            /*if it was an exit*/
        } else if (type==2){
            acessexit=NULL;
            /*Ok find if we know is position*/
            aux=getcarentry(getItemLinkedList(auxcars));
            /*Save some info to laiter use*/
            wt=getItemLinkedList(auxcars);
            getcarname(wt, buffer);
            time=getcartime(wt);
            /*Check if is position is valid*/
            if (aux.x!=-1 && aux.y!=-1 && aux.z!=-1) {
                /*Ok it looks that we know where it is so we can mark that space as empty*/
                writetype('.', getedgepointer(map, aux));
                /*Write that a car just left*/
                escreve_saida(fout, buffer , getcartime(wt), aux.x, aux.y, aux.z, 's');
                /*Ok we dont need to deal with this anymore so we take it from the list to get some memory back and minimize our time next time we need to go throw the list*/
                cars=removenode(cars, auxcars, (void*)freecar);
            } else {
                /*OK so at start we don't know where it is*/
                /*Get is name*/
                getcarname(getItemLinkedList(auxcars), buffer);
                /*Find the car*/
                helper=findcar(cars, buffer);
                /*If it isn't on the parking list something is wrong exit*/
                if(helper==NULL){
                    exit(0);
                }
                /*Did we found the only the exit someting is wrong exit*/
                if(helper==auxcars){
                    exit(0);
                }
                /*Get where it is parked*/
                wtpos=getcardest(getItemLinkedList(helper));
                /*Check if it even enter the park*/
                if(wtpos.x==-1 || wtpos.y==-1 || wtpos.z==-1){
                    /*The car did to enter the park*/
                    wtpos=getcarentry(getItemLinkedList(helper));
                    /*Write car exit*/
                    escreve_saida(fout, buffer , time, wtpos.x, wtpos.y, wtpos.z, 's');
                    /*Find the car on the waiting list*/
                    wh=findcar(waitinglist, buffer);
                    /*Remove car entrie and exit from the list*/
                    cars=removenode(cars, helper, (void*)freecar);
                    cars=removenode(cars, auxcars, (void*)freecar);
                    waitinglist=removenode(waitinglist, wh,NULL);
                    /*There is any parking free so process new car*/
                    type=0;
                    continue;
                } else {
                    /*Ok the car is parked*/
                    /*Write car exit*/
                    escreve_saida(fout, buffer , time, wtpos.x, wtpos.y, wtpos.z, 's');
                    /*Mark the parking space as empty*/
                    writetype('.', getedgepointer(map, getcardest(getItemLinkedList(helper))));
                    /*Get the exit because it can be helpfull for the car in wainting list*/
                    acessexit=getexit(getItemLinkedList(helper));
                    /*Remove car entrie and exit from the list*/
                    cars=removenode(cars, helper, (void*)freecar);
                    cars=removenode(cars, auxcars, (void*)freecar);
                    /*If the space was a active restriction no need lose more time*/
                    if (checkrestrictions(getedgepointer(map, wtpos))==1) {
                        type=0;
                        continue;
                    }
                }
                
                
            }

            /*Check if there is car in the waiting list*/
            for (wh=waitinglist; wh!=NULL; wh=getNextNodeLinkedList(wh)) {
                /*There cars in the wainting list so find there entrie*/
                for (auxacess=acesslist; auxacess!=NULL; auxacess=getNextNodeLinkedList(auxacess)) {
                    if (compareposition(getcarentry(getItemLinkedList(wh)), getacessposition(getItemLinkedList(auxacess)))) {
                        break;
                    }
                }
                /*Verificar se o grafo é valido para o instante de entrada*/
                if ((getvalid(getItemLinkedList(auxacess))<=lastresalt )|| getacessgraph(getItemLinkedList(auxacess))==NULL){
                    /*Resets all values on the map that are needed to creat the graph*/
                    resetcubedistance(map, size);
                    /*Because we are getting a new graph change the variable that tell if it is valid*/
                    changevalid(getcartime(getItemLinkedList(auxcars)), getItemLinkedList(auxacess));
                    /*Free the old graph*/
                    freegraph(getacessgraph(getItemLinkedList(auxacess)));
                    /*One point left to be free*/
                    freepoint(getacessgraph(getItemLinkedList(auxacess)));
                    /*Set the pointer to NULL*/
                    resetacessgraph(getItemLinkedList(auxacess));
                    /*Free list with parking spaces*/
                    freeLinkedList(getparking(getItemLinkedList(acesslist)), NULL);
                    saveparking(getItemLinkedList(acesslist), NULL);
                    /*Creat new graph*/
                    newgraph(map, getItemLinkedList(auxacess), size);
                    
                }
                if (acessexit!=NULL) {
                    if (getacesstype(acessexit)!=getcartype(getItemLinkedList(wh))) {
                        /*Find the slot and closest exit*/
                        slot=findnearemptyslot(acesslist, getcartype(getItemLinkedList(wh)), &acessexit, map, size, getItemLinkedList(auxacess), getcartime(getItemLinkedList(wh)), lastresalt);
                    } else {
                        slot=wtpos;
                    }
                } else {
                    /*Find the slot and closest exit*/
                    slot=findnearemptyslot(acesslist, getcartype(getItemLinkedList(wh)), &acessexit, map, size, getItemLinkedList(auxacess), getcartime(getItemLinkedList(wh)), lastresalt);
                }

                if (slot.x==-1 || slot.y==-1 || slot.z==-1 ) {
                    type=0;
                    continue;
                }
                /*Load all the positions to the parking*/
                path=getpath(getacessgraph(getItemLinkedList(auxacess)), slot ,map);
                /*Write the path to the file*/
                counter=output(fout, path, getItemLinkedList(wh), 0, 'w', time);
                /*free up the memory because we dont need the path no time soon*/
                freepath(path);
                /*Mark the space was full*/
                writetype('x', getedgepointer(map, slot));
                /*Tell the car where is is parked*/
                writecardest(slot, getItemLinkedList(wh));
                /*Load the path to the exit*/
                path=getpath(getacessgraph(acessexit), slot ,map);
                /*Write the path to the file*/
                output(fout, path, getItemLinkedList(wh), 1, 'q', counter);
                /*Free  up the memory again*/
                freepath(path);
                /*No longer in waiting list so remove it from the list*/
                waitinglist=removenode(waitinglist, wh, NULL);
                break;
            }
        }
        /*Just to be shure*/
        type=0;
    }
    /*Ok no more things to be done, so save all the head's of the lists to do some free's in the main*/
    out.cars=cars;
    out.restrictions=restrictions;
    out.waitinglist=waitinglist;
    /*Return a struct with the head of the lists*/
    return out;
}
/*
 Simple function that free the path list
 Receives a pointer to the head of the list and returns nothing
 */
void freepath(LinkedList *pointer){
    LinkedList *aux, *next;
    /*Go through all the elements freeing them*/
    for (aux=pointer; aux!=NULL; aux=next) {
        next=getNextNodeLinkedList(aux);
        freeposition(getItemLinkedList(aux));
        free(aux);
    }
}
/*
 This is the function that deals with the output.
 It works in a recursive way!!
 This funcion only works if there is any incorrect moves
 Recives as arguments a pointer to the output file, to the list with the path, to the car in question.
 It also receives one int that tells what direction it is should write, a char with the last move and int that counts ow many times is run.
 
 It returns how many times the function was called
 */
int output(FILE *fp, LinkedList *path, CAR *car, int direction, char cart, int counter){
    char where='*', namebuffer[10], move='*';
    int writeenable=0, internalcounter, value=1;
    POSITION *aux, *next;
    static char lastname[10]="\0";
    /*Copies the counter value to an internal variable*/
    internalcounter=counter;
    aux=getItemLinkedList(path);
    /*Check if it got to the end of the list*/
    if (getNextNodeLinkedList(path)!=NULL) {
        next=getItemLinkedList(getNextNodeLinkedList(path));
    } else {
        /*If it is the send it sets the pointers to the same value*/
        next=aux;
    }
    /*Copies last move to a internal variable so it can be change*/
    where=cart;
    /*Checks is it moved in the x axis and if the last move wasn't in the x axis*/
    if (aux->x!=next->x && cart!='x') {
        writeenable=1;
        where='x';
        /*Checks is it moved in the y axis and if the last move wasn't in the y axis*/
    } else if (aux->y!=next->y && cart!='y'){
        writeenable=1;
        where='y';
        /*Checks is it moved in the z axis and if the last move wasn't in the z axis*/
    } else if (aux->z!=next->z && cart!='z'){
        writeenable=1;
        /*If moving up or down it costs more so set value to 2*/
        value=2;
        where='z';
    }
    /*Verificar a direçao de escrita*/
    if (direction==0) {
        /*Driving the car so set the move type to the char m*/
        move='m';
        /*Check if we at the end of the list*/
        if (getNextNodeLinkedList(path)==NULL) {
            where='*';
            /*Set the move type to e because we are parking*/
            move='e';
            /*Enable wrinting in the output file*/
            writeenable=1;
        }
    } else {
        /*Going on foot to set the move char to p*/
        move='p';
        /*Ugly i know but i couldn't think in anything else!!*/
        if (getNextNodeLinkedList(path)!=NULL) {
            if (getNextNodeLinkedList(getNextNodeLinkedList(path))!=NULL) {
                if (cart=='q' && getNextNodeLinkedList(getNextNodeLinkedList(getNextNodeLinkedList(path)))==NULL) {
                    where='+';
                    
                }
            }
        }
    }
    /*Check if the car was on the waitin list, if so ignore the first movement*/
    if (cart=='w') {
        writeenable=0;
        move='i';
    }
    /*Check if coming from the entrie*/
    if (cart=='n') {
        writeenable=1;
        move='i';
    }
    /*Check if is going to the exit*/
    if (cart=='q') {
        writeenable=1;
        move='a';
    }
    /*Verifies the writing direction. Same*/
    if (direction==0) {
        if (writeenable) {
            /*Get the name o the car*/
            getcarname(car, namebuffer);
            /*Writes in the file*/
            escreve_saida(fp, namebuffer , internalcounter, aux->x, aux->y, aux->z, move);
            
        }
        /*Increment the counter*/
        internalcounter+=value;
    }
    /*If not in the last node call the fuction again*/
    if (getNextNodeLinkedList(path)!=NULL) {
        internalcounter=output(fp, getNextNodeLinkedList(path), car, direction, where, internalcounter);
    }
    /*Verifies the writing direction. Reverse*/
    if (direction==1) {

        /*Increment the counter*/
        internalcounter+=value;
        if (writeenable) {
            /*Get the name o the car*/
            getcarname(car, namebuffer);
            /*Writes in the file*/
            escreve_saida(fp, namebuffer , internalcounter, aux->x, aux->y, aux->z, move);
        }
        /*If in last move write that the driver left the park*/
        if (move=='a' && cart=='q' && strcmp(lastname, namebuffer)) {
            /*To get the values as asked some clever mathematics where aplied in this function to get
             the correct results*/
            escreve_saida(fp, namebuffer , getcartime(car) , counter+1 , internalcounter ,(internalcounter-counter-1)*3+counter-getcartime(car)+1 , 'x');
            strcpy(lastname, namebuffer);
        }
    }
    /*Some math to correct strange results*/
    if (direction==0 && move=='e') {
        internalcounter=internalcounter-2;
        
    }
    /*return the value of the counter*/
    return internalcounter;
}

/*
 This function creats graphs using minimun suport trees.
 Using some clever thinking (lets see if it true) this function creats trees by adding
 elements right next to the last element added.
 So it adds the elements at distance 1, 2, 3, ..., n-1, n until there is no more elements to be added
 It receives as arguments a pointer to the map ant to the acess, it also reveives the size of the map.
 
 It returns nothing
 */
void newgraph(EDGE *** map, void *from, POSITION size){
    GRAPH *aux;
    POSITION pos;
    LinkedList *next, *list1=NULL, *list2=NULL;
    int value=1;
    char *buffer;
    /*Allocs a string*/
    buffer=(char*)malloc(10*sizeof(char));
    /*Gets the name of the acess*/
    getacessname(from, buffer);
    /*To know wat values to use see if it is an exit on en entrie*/
    switch (buffer[0]) {
        case 'E':
            value=1;
            break;
        case 'A':
            value=3;
            break;
            
        default:
            break;
    }
    /*Free of the buffer (Why not!!)*/
    free(buffer);
    /*Get the location of the acess (entrie or exit)*/
    pos=getacessposition(from);
    /*add the first point to the graph*/
    aux=newpoint(pos);
    /*sets the distance on the edge to know that it is already on the graph*/
    writedistance(0, getedgepointer(map, pos));
    /*save the graph in the struck of that acess*/
    writegraph(from, aux);
    /*Adds the firt row of elements*/
    list1=addtograph(map, aux, size, list1, value, from);
    /*To be shure*/
    list2=NULL;
    /*While there is elements to be added it runs*/
    while (1) {
        /*If no elements added exit the loop*/
        if (list1==NULL) {
            break;
        }
        /*Go through the list of the last elements added and add new elements*/
        for (next=list1; next!=NULL; next=getNextNodeLinkedList(next)){
            list2=addtograph(map, getItemLinkedList(next), size,list2, value, from);
        }
        /*free the list1 because no more need for it*/
        freeLinkedList(list1, NULL);
        /*exchange the pointer information*/
        list1=list2;
        /*Resets list2*/
        list2=NULL;
        
        
    }
    /*Free the last list to enshure no memory leaks*/
    freeLinkedList(list1, NULL);
    /*Reset distance to be shure*/
    resetcubedistance(map, size);
    
}

/*
    This function adds points to the graph and creats a list with all the elements that where added.
    It receives has arguments a pointer to the map, to one point of the graph, the size of the map
    and the head of the list where the points will be also added. It also receives a value to increment
    to the distance.
    It returns the head of the list.
 */

LinkedList * addtograph(EDGE *** map, GRAPH *current, POSITION size, LinkedList *head, int value, ACESS *pointer){
    GRAPH *aux;
    POSITION pos, newpos;
    pos=getmyposition(current);
    int ramp=1;
    char buffer;
    /*Verifies if is working in a entrie graph to put all exits as leafs*/
    if (getdistancegraph(current)>2 && (gettype((void*)getedgepointer(map,pos))=='a' || gettype((void*)getedgepointer(map,pos))=='e')) {
        /*Resets the distance on the exit so it can be added from other position*/
        writedistance(-1, (void*)getedgepointer(map, getmyposition(current)));
        return head;
    }
    /*Verifies if is parking space to put it has a leaf*/
    buffer=gettype((void*)getedgepointer(map,pos));
    if (buffer=='.' || buffer=='x') {
        saveparking(pointer, insertUnsortedLinkedList(getparking(pointer), current));
        return head;
    }
    if (gettype(getedgepointer(map, pos))=='u' ) {
        if (getdistanceedge(getedgepointercart(map, pos.x, pos.y, pos.z+1))!=-1) {
            ramp=1;
        } else {
            ramp=0;
        }
    } else if (gettype(getedgepointer(map, pos))=='d') {
        if (getdistanceedge(getedgepointercart(map, pos.x, pos.y, pos.z-1))!=-1) {
            ramp=1;
        } else {
            ramp=0;
        }
    } else {
        if (ramp==0) {
            ramp=0;
        } else {
            ramp=1;
        }
    }
    /*IF inside the map space*/
    if(pos.x-1>=0){
        /*Verifica se não é uma parede*/
        if(gettype((void*)getedgepointercart(map,pos.x-1,pos.y,pos.z))!='@' && checkrestrictions((void*)getedgepointercart(map,pos.x-1,pos.y,pos.z))==0 && ramp){
            /*Verificar se já se encontra no grafo*/
            if(getdistanceedge((void*)getedgepointercart(map,pos.x-1,pos.y,pos.z))==-1){
                /*Set new position*/
                newpos=pos;
                newpos.x=newpos.x-1;
                /*Creat new graph point*/
                aux=newpoint(newpos);
                /*Change the distance to the root*/
                changedistance(aux, getdistancegraph(current)+value);
                writedistance(getdistancegraph(current)+value, getedgepointer(map, newpos));
                /*Insert in graph*/
                addtolist(current, aux);
                /*Insert in the list to be returned*/
                head=insertUnsortedLinkedList(head, aux);
            }
            
        }
    }
    /*IF inside the map space*/
    if(pos.y+1<size.y){
        /*Verifica se não é uma parede*/
        if(gettype((void*)getedgepointercart(map,pos.x,pos.y+1,pos.z))!='@' && checkrestrictions((void*)getedgepointercart(map,pos.x,pos.y+1,pos.z))==0 && ramp){
            if(getdistanceedge((void*)getedgepointercart(map,pos.x,pos.y+1,pos.z))==-1){
                /*Verificar já se encontra no grafo*/
                /*Set new position*/
                newpos=pos;
                newpos.y=newpos.y+1;
                /*Creat new graph point*/
                aux=newpoint(newpos);
                /*Change the distance to the root*/
                changedistance(aux, getdistancegraph(current)+value);
                writedistance(getdistancegraph(current)+value, (void*)getedgepointercart(map,pos.x,pos.y+1,pos.z));
                /*Insert in graph*/
                addtolist(current, aux);
                /*Insert in the list to be returned*/
                head=insertUnsortedLinkedList(head, aux);
            }
        }
    }
    /*IF inside the map space*/
    if(pos.x+1<size.x){
        /*Verifica se não é uma parede*/
        if(gettype((void*)getedgepointercart(map,pos.x+1,pos.y,pos.z))!='@' && checkrestrictions((void*)getedgepointercart(map,pos.x+1,pos.y,pos.z))==0 && ramp){
            if(getdistanceedge((void*)getedgepointercart(map,pos.x+1,pos.y,pos.z))==-1){
                /*Verificar já se encontra no grafo*/
                /*Set new position*/
                newpos=pos;
                newpos.x=newpos.x+1;
                /*Creat new graph point*/
                aux=newpoint(newpos);
                /*Change the distance to the root*/
                changedistance(aux, getdistancegraph(current)+value);
                writedistance(getdistancegraph(current)+value, (void*)getedgepointercart(map,pos.x+1,pos.y,pos.z));
                /*Insert in graph*/
                addtolist(current, aux);
                /*Insert in the list to be returned*/
                head=insertUnsortedLinkedList(head, aux);
            }
        }
    }
    
    /*IF inside the map space*/
    if(pos.y-1>=0){
        /*Verifica se não é uma parede*/
        if(gettype((void*)getedgepointercart(map,pos.x,pos.y-1,pos.z))!='@' && checkrestrictions((void*)getedgepointercart(map,pos.x,pos.y-1,pos.z))==0 && ramp){
            if(getdistanceedge((void*)getedgepointercart(map,pos.x,pos.y-1,pos.z))==-1){
                /*Verificar já se encontra no grafo*/
                /*Set new position*/
                newpos=pos;
                newpos.y=newpos.y-1;
                /*Creat new graph point*/
                aux=newpoint(newpos);
                /*Change the distance to the root*/
                changedistance(aux, getdistancegraph(current)+value);
                writedistance(getdistancegraph(current)+value, (void*)getedgepointercart(map,pos.x,pos.y-1,pos.z));
                /*Insert in graph*/
                addtolist(current, aux);
                /*Insert in the list to be returned*/
                head=insertUnsortedLinkedList(head, aux);
            }
        }
    }
    /*IF inside the map space*/
    if(pos.z-1>=0){
        /*Verifica se é uma rampa para descer*/
        if(gettype((void*)getedgepointer(map,pos))=='d' && checkrestrictions((void*)getedgepointercart(map,pos.x,pos.y,pos.z-1))==0){
            if(getdistanceedge((void*)getedgepointercart(map,pos.x,pos.y,pos.z-1))==-1){
                /*Verificar já se encontra no grafo*/
                /*Set new position*/
                newpos=pos;
                newpos.z=newpos.z-1;
                /*Creat new graph point*/
                aux=newpoint(newpos);
                /*Change the distance to the root*/
                changedistance(aux, getdistancegraph(current)+value*2);
                writedistance(getdistancegraph(current)+value*2, (void*)getedgepointercart(map,pos.x,pos.y,pos.z-1));
                /*Insert in graph*/
                addtolist(current, aux);
                /*Insert in the list to be returned*/
                head=insertUnsortedLinkedList(head, aux);
            }
        }
    }
    /*IF inside the map space*/
    if(pos.z+1<size.z){
        /*Verifica se é uma rampa para subir*/
        if(gettype((void*)getedgepointer(map,pos))=='u' && checkrestrictions((void*)getedgepointercart(map,pos.x,pos.y,pos.z+1))==0){
            if(getdistanceedge((void*)getedgepointercart(map,pos.x,pos.y,pos.z+1))==-1){
                /*Verificar já se encontra no grafo*/
                newpos=pos;
                
                newpos.z=newpos.z+1;
                /*Creat new graph point*/
                aux=newpoint(newpos);
                /*Change the distance to the root*/
                changedistance(aux, getdistancegraph(current)+value*2);
                writedistance(getdistancegraph(current)+value*2, (void*)getedgepointercart(map,pos.x,pos.y,pos.z+1));
                /*Insert in graph*/
                addtolist(current, aux);
                /*Insert in the list to be returned*/
                head=insertUnsortedLinkedList(head, aux);
                
            }
        }
    }
    /*Returns the head of the list with the last added elements*/
    return head;
}
/*
    This funcion is used to find the closest parking space to in all the map.
    It receives a pointer to the list os acess, a char for the type of acess the user want,
    a pointer that will use to pass the closest acess (exit), a pointer to the map, the size o f the map,
    a pointer to the graph of the entrie and the current instante.
    It returns the closest parking space.
 */

POSITION findnearemptyslot(LinkedList *acesslist, char type, ACESS **exit, EDGE ***map, POSITION size, ACESS *entrie, int time, int lastresalt){
    POSITION pos;
    LinkedList *aux;
    ACESS *pointer;
#ifndef NEW
    GRAPH *pass;
#endif
    DESTINATION closest;
    int  min=9999999;
    pos.x=-1;
    pos.y=-1;
    pos.x=-1;
    /*Run all acess*/
    for (aux=acesslist; aux!=NULL; aux=getNextNodeLinkedList(aux)) {
        /*Get the pointer to the acess struct*/
        pointer=getItemLinkedList(aux);
        /*Check if the pointer i valid*/
        if (pointer==NULL) {
            return pos;
        }
        /*Check if the acess is the same as the chosen by the user off the park on entrie*/
        if (getacesstype(pointer)==type) {
            /*Verifies if there a graph and if it is valid for that instance*/
            if ((getvalid(getItemLinkedList(acesslist))<=lastresalt) || getacessgraph(pointer)==NULL){
                /*Change the valid to the actual time*/
                changevalid(time, getItemLinkedList(acesslist));
                /*Reset the distances in the cube*/
                resetcubedistance(map, size);
                /*Free the of graph*/
                freegraph(getacessgraph(pointer));
                /*Free the last point of the graph*/
                freepoint(getacessgraph(pointer));
                /*Reset the graph pointer of the acess*/
                resetacessgraph(pointer);
                /*Free list with parking spaces*/
                freeLinkedList(getparking(pointer), NULL);
                saveparking(pointer, NULL);
                /*Creat new graph*/
                newgraph(map, pointer, size);
                
            }
            /*Load the distances to the map*/
#ifndef NEW
            loaddistancetomapold(getacessgraph(entrie), map);
#else
            loaddistancetomapnew(getparking(entrie), map);
#endif

            /*Search for the closest parking space*/
#ifndef NEW
            /*Load the graph from the exit*/
            pass=getacessgraph(pointer);
            closest=searchold(pass, map);
#else
            closest=searchnew(getparking(pointer), map);
#endif
            /*Check if the funcion found a slot, if 9999999 resctrions dind't let graph continue*/
            if (closest.distancesum==9999999) {
                continue;
            }
            /*Update the closest exit a parking space*/
            if (closest.distancesum<min) {
                min=closest.distancesum;
                pos=getmyposition(closest.current);
                *exit=pointer;
            }
        }
    }
    /*didnt found any place to park the car*/
    if(min>=9999990){
        pos.x=-1;
        pos.y=-1;
        pos.z=-1;
    }
    /*Returns the closest parking sapace*/
    return pos;
}

/*
    This function load the path from the entrie or exit to a position.
    It receives a pointer to the graph and other to the map and the position that it wnats to reach.
    It returns apointer to a list with all the positions.
 */


LinkedList *getpath(GRAPH *pointer,POSITION pos, EDGE ***map){
    LinkedList *aux=NULL, *next=NULL;
    POSITION prov, *new;
    prov=getmyposition((void*) pointer);
    /*Check if it is in the final position*/
    if (prov.x==pos.x && prov.y==pos.y && prov.z==pos.z) {
        new=(POSITION*)malloc(sizeof(POSITION));
        *new=prov;
        aux=insertStack(aux, new);
        return aux;
    }
    /*Call the adjacent points*/
    for (next=getlist(pointer); next!=NULL; next=getNextNodeLinkedList(next)) {
        
        aux=getpath(getItemLinkedList(next), pos, map);
        /*If there a return from the function*/
        if (aux!=NULL) {
            /*alloc a postition*/
            new=(POSITION*)malloc(sizeof(POSITION));
            *new=prov;
            /*Save the alloced postion on a stack*/
            aux=insertStack(aux, new);
            /*Return head of atack*/
            return aux;
        }
    }
    /*Return head of atack*/
    return aux;
}



/*
    This function free's every element of the graph.
    Receives a pointer to the graph and retuns nothing.
    Works in a recursive way.
*/
void freegraph(GRAPH *pointer){
    LinkedList *aux;
    /*If it is on the end stop*/
    if (pointer==NULL) {
        return;
    }
    /*Call function for every element*/
    for (aux=getlist(pointer); aux!=NULL; aux=getNextNodeLinkedList(aux)) {
        freegraph(getItemLinkedList(aux));
    }
    /*Frees the list and elements*/
    freeLinkedList(getlist(pointer), (void*) freepoint);
    
    
}
/*
 This function searches for the closest empty parking space to only one acess at one time.
 It receives as arguments a pointer to the exit graph end a pointer to the map.
 It returns a struct with the minimum distance encouter and the point of the graph os that postion.
 Recursive search
 */

DESTINATION searchold(GRAPH *pointer, EDGE ***map){
    DESTINATION min, next;
    LinkedList *aux;
    EDGE *curr;
    min.current=pointer;
    curr=getedgepointer(map, getmyposition(pointer));
    min.distancesum=getdistanceedge(curr)+getdistancegraph(pointer);
    /*If it isnt an empty parking slot it makes no diference to the search*/
    if (gettype(curr)!='.') {
        min.distancesum=9999999;
    }
    /*If it is not on the graph doesnt count*/
    if (getdistanceedge(curr)==-1 && gettype(curr)!='a') {
        min.distancesum=9999999;
    }
    /*If there is elements aed continue by caling the function again*/
    for (aux=getlist(pointer); aux!=NULL; aux=getNextNodeLinkedList(aux)) {
        /*Call the function in a recursive way*/
        next=searchold(getItemLinkedList(aux), map);
        /*IF the return is smaller than my actual distance change min*/
        if (next.distancesum<min.distancesum) {
            min=next;
        }
    }
    /*Returns a destination with the minimun distance and a pointer to that point in the graph*/
    return min;
}

/*
 This function loads the distance from the graph to the map.
 It receives a pointer to the graph and to the map.
 It returns nothing.
 Works in a recursive way.
 */

void loaddistancetomapold(GRAPH *pointer, EDGE ***map){
    LinkedList *next=NULL;
    /*If it is on the end stop*/
    if(pointer==NULL){
        return;
    }
    /*Call the funcion for every element*/
    for (next=getlist(pointer); next!=NULL; next=getNextNodeLinkedList(next)) {
        loaddistancetomapold(getItemLinkedList(next), map);
    }
    /*Save distance on the map*/
    writedistance(getdistancegraph(pointer), getedgepointer(map, getmyposition(pointer)));
}
/*
 This function searches for the closest empty parking space to only one acess at one time.
 It receives as arguments a pointer to a list with parking spaces and a pointer to the map.
 It returns a struct with the minimum distance encouter and the point of the graph os that postion.
 */
DESTINATION searchnew(LinkedList *pointer, EDGE ***map){
    DESTINATION min;
    EDGE *current;
    int mindist;
    LinkedList *aux;
    min.distancesum=9999999;
    min.current=NULL;
    for (aux=pointer; aux!=NULL; aux=getNextNodeLinkedList(aux)) {
        current=getedgepointer(map, getmyposition(getItemLinkedList(aux)));

        if (gettype(current)=='.' && getdistanceedge(current)!=-1) {
            mindist=getdistancegraph(getItemLinkedList(aux))+getdistanceedge(current);
            if (mindist<min.distancesum) {
                min.distancesum=mindist;
                min.current=getItemLinkedList(aux);

            }
        }
    }
    return min;
}
/*
 This function loads the distance from the graph to the map.
 It receives a pointer to a list with parking spaces and to the map.
 It returns nothing.
 */
void loaddistancetomapnew(LinkedList *pointer, EDGE ***map){
    LinkedList *next=NULL;
    GRAPH *aux;
    /*If it is on the end stop*/
    for (next=pointer; next!=NULL; next=getNextNodeLinkedList(next)) {
        aux=getItemLinkedList(next);
        if (getdistancegraph(aux)<999999) {
            writedistance(getdistancegraph(aux), getedgepointer(map, getmyposition(aux)));
        }
    }

}

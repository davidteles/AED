/*
 *  File name: LinkedList.c
 *
 *  Author: 2013 Fall Semester AED Team
 *  Edited by David Teles and Joao Loureiero
 *
 *  Release date: 2013/10/04
 *
 *  Description: C-Implementation of an Abstract Linked List
 *
 *  Implementation details:
 *    Linked list node: Each node of the list contains a pointer to
 *   the item stored in the node and a pointer to the next node. The
 *   item is specified in the file defs.h.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 */



/* Header Inclusions                                              */
#include<stdio.h>
#include<stdlib.h>
#include "database.h"



/* Include Header File with Data Type and Function Prototypes     */
#include"LinkedList.h"



/*
 *  Data Type: LinkedListStruct
 *
 *  Description: Structure with:
 *                 1) Pointer to the item of the linked list node
 *                 2) Pointer to next node of the linked list.
 */
struct LinkedListStruct
{
  Item this;
  LinkedList * next;
};



/*
 *  Function:
 *    initLinkedList
 *
 *  Description:
 *    Initializes a new linked list.
 *
 *  Arguments:
 *    None
 *
 *  Return value:
 *    Returns the pointer to a new linked list.
 */
LinkedList * initLinkedList(void)
{
  return NULL;
}



/*
 *  Function:
 *    freeLinkedList
 *
 *  Description:
 *    Frees the memory allocated to a linked list.
 *
 *  Arguments:
 *    Pointer to the first element of a linked list:
 *      (LinkedList *) first
 *    Function to free the memory allocated to the items:
 *      void freeItem(Item)
 *
 *  Return value:
 *    None
 */
void freeLinkedList(LinkedList * first, void (* freeItemFnt)(Item))
{
  LinkedList * next;
  LinkedList * aux;
  
  /* Cycle from the first to the last element                     */
  for(aux = first; aux != NULL; aux = next) 
  {
    /* Keep trace of the next node                                */
    next = aux->next;

    /* Free current item         */
      if (freeItemFnt!=NULL) {
          if (aux->this!=NULL) {
              freeItemFnt(aux->this);
          }
          
      }



    /* Free current node                                          */
    free(aux);
  }

  return;
}

/*
 *  Function:
 *    removenode
 *
 *  Description:
 *    remove um elemento da lista e faz free do seu conteudo
 *
 *  Arguments:
 *    Pointer to the first element of a linked list:
 *      (LinkedList *) head
 *    Function to free the memory allocated to the items:
 *      void freeItem(Item)
 *    Pointer to the element of a linked list to be removed:
 *      (LinkedList *) remove
 *
 *  Return value:
 *    first element of the list
 */
LinkedList *removenode(LinkedList *head, LinkedList *remove, void (* freeItemFnt)(Item)){

    LinkedList * aux;
    
    /*se o elemento a remover for o primeiro da lista*/
    if (remove==head) {
        head=remove->next;
        if (freeItemFnt!=NULL) {
            freeItemFnt(remove->this);
        }
        
        
        
        /* Free current node                                          */
        free(remove);
        
        return head;
    }
    
    /* Cycle from the first to the last element                     */
    for(aux = head; aux->next != remove && aux!=NULL; aux = aux->next){}

    aux->next=remove->next;
    /* Free current item         */
    if (freeItemFnt!=NULL) {
        freeItemFnt(remove->this);
    }
    
    
    
    /* Free current node                                          */
    free(remove);

    return head;
}

/*
 *  Function:
 *    lengthLinkedList
 *
 *  Description:
 *    Determines the length of a linked list.
 *
 *  Arguments:
 *    Pointer to the first node of the linked list:
 *        (LinkedList *) first
 *
 *  Return value:
 *    Returns the length of the linked list.
 */
int lengthLinkedList(LinkedList * first)
{
  LinkedList * aux;
  int counter;

  /* Length determination cycle                                   */
  for(aux = first, counter = 0;
      aux!=NULL; 
      counter++, aux = aux->next);

  return counter;
}



/*
 *  Function:
 *    getNextNodeLinkedList
 *
 *  Description:
 *    Returns the next node of a linked list.
 *
 *  Arguments:
 *    Pointer to the current linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the pointer to the next node of a linked list. NULL
 *   is returned in case the current node is empty or there is no
 *   node following the current node.
 */
LinkedList * getNextNodeLinkedList(LinkedList * node)
{
    if (node==NULL) {
        return NULL;
    }
  return ((node == NULL) ? NULL : node->next);
}



/*
 *  Function:
 *    getItemLinkedList
 *
 *  Description:
 *    Gets the item of a linked list node.
 *
 *  Arguments:
 *    Pointer to a linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the pointer to the item of a linked list node. NULL
 *   is returned if the node is NULL (or if the item is NULL).
 */
Item getItemLinkedList(LinkedList * node)
{
  /* Check if node is not empty                                   */
  if(node == NULL)
    return NULL;

  return node->this;
}



/*
 *  Function:
 *    insertUnsortedLinkedList
 *
 *  Description:
 *    Creates a new linked list node.
 *
 *  Arguments:
 *    Item to associate to the new node:
 *      Item this
 *    Pointer to the next node:
 *      (LinkedList *) next
 *
 *  Return value:
 *    Returns the pointer to the node.
 */
LinkedList * insertUnsortedLinkedList(LinkedList * head, Item this)
{
  LinkedList * new, *aux;
  /* Memory allocation                                            */
  new = (LinkedList *) malloc(sizeof(LinkedList));

  /* Check memory allocation errors                               */
  if(new == NULL)
    return NULL;

  /* Initialize new node                                          */
  new->this = this;
  
    if (head==NULL) {
        head=new;
        new->next = NULL;
        return head;
    }
    
    for (aux=head; aux->next!=NULL; aux=aux->next){
        }
    new->next=NULL;
    aux->next=new;

  return head;
}



/*
 *  Function:
 *    insertSortedLinkedList
 *
 *  Description:
 *    Inserts an item in order in an sorted linked list.
 *
 *  Arguments:
 *    Pointer to the first node of a sorted linked list:
 *        (LinkedList *) first
 *    Pointer to item to be inserted:
 *        Item item
 *    Pointer to function to compare two items:
 *        int comparisonItemFnt(void * item1, void * item2)
 *
 *        This function returns a value less, equal, or greater
 *       than zero if item1 compares less, equal, or greater than
 *       item2, respectively.
 *
 *    Pointer to integer to write error return value:
 *        (int *) err
 *
 *        0 upon sucess, 1 in case the item is NULL, and 2 in
 *   case of memory allocation failure.
 *
 *  Return value:
 *    Returns the pointer to the first node of the sorted linked list.
 */
LinkedList * insertSortedLinkedList(LinkedList * head, Item this, int (*comparator2)())
{
    LinkedList * new, *aux;
    /* Memory allocation                                            */
    new = (LinkedList *) malloc(sizeof(LinkedList));
    
    /* Check memory allocation errors                               */
    if(new == NULL)
        return NULL;
    
    /* Initialize new node                                          */
    new->this = this;
    
    if (head==NULL) {
        head=new;
        new->next = NULL;
        return head;
    }
    
    if(comparator2(new->this)<comparator2(head->this)){
        new->next=head;
        head=new;
        return head;
    }
    
    for (aux=head; aux->next!=NULL; aux=aux->next){
        if (comparator2(new->this)<comparator2((aux->next)->this)) {
            (new->next)=(aux->next);
            (aux->next)=new;
            return head;
        }
        
        
    }
    new->next=NULL;
    aux->next=new;
    
    return head;
}

/*
 *  Function:
 *    insertStack
 *
 *  Description:
 *    Inserts a new item in the head of the list.
 *
 *  Arguments:
 *    Pointer to the first node of a sorted linked list:
 *        (LinkedList *) first
 *    Pointer to item to be inserted:
 *        Item item
 *  Return value:
 *    Returns the pointer to the first node of the sorted linked list.
 */
LinkedList * insertStack(LinkedList * head, Item this){
    LinkedList * new;
    /* Memory allocation                                            */
    new = (LinkedList *) malloc(sizeof(LinkedList));
    
    /* Check memory allocation errors                               */
    if(new == NULL)
        return NULL;
    
    /* Initialize new node                                          */
    new->this = this;
    
    if (head==NULL) {
        head=new;
        new->next = NULL;
        return head;
    }
    
    new->next=head;
    head=new;
    
    return head;
}

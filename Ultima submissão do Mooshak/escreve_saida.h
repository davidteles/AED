/********************************************************************
 * File: escreve_saida.h
 *
 * Date: 30-04-2009
 *
 * Description: funcao auxiliar fornecida pelo corpo docente
 *              de AED 2008/2009 para o projecto final "Parking Garage".
 *
 * Nota: a funcao nao fecha o ficheiro!!
 *
 * History:
 *
 * Bug-report:  lms@inesc-id.pt
 *
 * Copyright (c) 2008 IST/DEEC/AED - Algoritmos e Estruturas de Dados
 ********************************************************************/

#ifndef gestor_escreve_saida_h
#define gestor_escreve_saida_h


#endif
/******************************************************************************
 * escreve_saida ()
 *
 * Arguments: fp - descriptor do ficheiro de saida
 *            vid - identificador da viatura
 *            tk - instante de tempo em que ocorre o movimento
 *            pX, pY, pZ - coordenadas (X,Y,Z) da viatura em movimento
 *            tm - tipo de movimento
 * Returns: 0 - se nao houver qualquer erro, ou seja se as coordenadas
 *              corresponderem a uma posicao valida
 *          -1 - se houver algum erro
 * Side-Effects: nenhum
 *
 * Description: escreve no ficheiro de saida um tuplo de valores do tipo
 *                          Vid T X Y Z M
 *              sendo Vid o identificador da viatura, T o instante de tempo,
 *              X, Y e Z  indicam a posicao da viatura no parque de
 *              estacionamento e M e' o tipo de movimento podendo apenas ter
 *              os caracteres 'i', 'm', 'e', 'p', 'a', 'x' ou 's'.
 *
 *****************************************************************************/

int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm);
#include <stdio.h> 
#include <stdlib.h>
#include "linked_list.h"
#include <string.h>

void ejecute_execvp(char *cmd)
{
   char * token = strtok(cmd, " "); // toma las entradas del terminal separadas por espacios
    struct LinkedList* tokens = (struct LinkedList*)malloc(sizeof(struct LinkedList)); // se crea la linklist donde se guardan en cado nodo los token de cmd
    struct LinkedList* tokens_end = tokens; // en token_end se guarda el ultimo token de la lista , inicialmente coincide con el primero
    int j = 0; // j es el numero de palabras separadas por espacios
    while( token != NULL ) // esto se ejecuta hasta que no queden palabras por procesar
    {
      int len; 
      len = strlen(token);
      if(len > 0 && token[len-1] == '\n') // verifica que cada palabra del token que tenga un /n , elimina el /n 
        {
            token[len-1] = '\0';
        }
      struct LinkedList* data = (struct LinkedList*)malloc(sizeof(struct LinkedList)); // reservo el espacio en memoria y se crea el proximo nodo de la linklist
      tokens_end->data = token; // annado el token a la linklist y actualizo referencias 
      // printf("%s", tokens->data);
      tokens_end->next = data;
      tokens_end = data;
      token = strtok(NULL, " ");
      j++;
    }
    char* tokens_list[j]; // se guardan los comandos ejecutables
    int i = 0;
    while(tokens != tokens_end || i != j) // se procesan los comandos de la linklist para que esten un una estructura compatible para e excvp
    {
      tokens_list[i] = tokens->data;
      tokens = tokens->next;
      i++;
      // printf("%d", i);
    }
    tokens_list[j] = NULL; // se annade el NULL necesario para el execvp 
    execvp(tokens_list[0],tokens_list);// se ejecuta el execvp 
    free(cmd);   
}
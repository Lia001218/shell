#include <stdio.h> 
#include <stdlib.h>

int *piper_index(char* command)
{
    int piper_operator_index[strlen(command)]; // se van a guardar los indices donde se encuentran los piper
    int count = 0;
    for (int i = 0; i <  strlen(command); i++) // se recorren lo escrito en la consola
    {
        if (command[i] == '<' || command[i] == '>' || command[i] == '|' ) 
        piper_operator_index[count++] = i;
    }
    int *final_pipes = (int*) malloc(count*sizeof(int)); 
    for (int i = 0; i < count; i++) // se guardan los indices en un array mas pequenno
    {
        final_pipes[i] = piper_operator_index[i];
    }
    return final_pipes;
}

int count_piper(char* command)
{
    int count = 0;
    for (int i = 0; i <  strlen(command); i++) // se recorren lo escrito en la consola
    {
        if (command[i] == '<' || command[i] == '>' || command[i] == '|' ) 
        count++;
    }

    return count;
}



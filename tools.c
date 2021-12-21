#include <string.h>
#include <stdlib.h>
char* get_first_work(char*str)  // se crea un array de char del tamanno de la primera palabra del comando
{
    char* Second_word = strchr(str,' '); // devuelve una cadena a partir del primer espacio hasta el final
    size_t lenghtOFfirstWord =  Second_word - str; // calcula el tamanno de la primera palabra del comando 
    char* first_word = (char*)malloc((lenghtOFfirstWord + 1)*sizeof(char));// reserva el espacio en memoria de la primera palabra del comando 
    return strncpy(first_word,str,lenghtOFfirstWord); // devuelve la primera palabra del comando 
} 
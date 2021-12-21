#include <stdio.h>
#include "shell.h"

void print_prompt1(void) // imprime en consola $
{
    fprintf(stderr, "$ ");
}

void print_prompt2(void) // si el comando es muy largo lo divide en otra linea
{
    fprintf(stderr, "> ");
}
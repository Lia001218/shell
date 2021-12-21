#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"
#include "tools.c"
#include "parser.c"
#include "ejecute_execvp.c"
#define MAX 51
char * PreviusComand(char *comand,int *piper, int current);
char * PreviusComand(char *comand, int *piper,int current)
{
  int inicio = 0 ;
  if(current != 0)
  {
    inicio = piper[current - 1] + 1;
  }
  char *previus_comand = malloc(sizeof(char)* piper[current ] - inicio);
  for (int i = inicio; i < piper[current ]; i++)
  {
    previus_comand[i] = comand[i];
  }
  return previus_comand;  
}
void redirectEntrada(char *cadena, char *previus_comand);
void redirectEntrada(char *cadena, char *previus_comand)
{
  printf("%s",cadena);
  int fd;   
  fd = open (cadena,O_RDONLY); // Se asigna la salida del fichero 
  close(0); // se cierra la salida estandar
  close(1); // se cierra la entrada estandar
  dup(fd);
  ejecute_execvp(previus_comand);// se ejecuta el comando 
}  
// void redirectSalida(char *cadena, char *previus_comand, int next_simbol)
void redirectSalida(char*cadena, char *previus_comand, int next_simbol)
{
  if(next_simbol == 0)
  {
    char *cadenaPtr;
    cadenaPtr = cadena; // Puntero a la cadena
    close (1); // se cierra la salida estandar		
    open (cadenaPtr,O_CREAT | O_WRONLY,0777); // se asigna a la salida el fichero
    ejecute_execvp(previus_comand);
    close(0); // se cierra la entrada estandar
  }
  else
  {
    if(open (cadena,O_CREAT | O_WRONLY,0777 == 3)) return; // si el archivo no esta creado no se hace nada 
    int out;
    int err = open("cerr.log", O_RDWR | O_CREAT | O_APPEND, 0600); 
    if (-1 == err) // si no se puede abrir el err hay un problema abrinedo el archivo
    {
      perror("opening cerr.log");
      
    }
    int save_out = dup(fileno(stdout));// se abre el fichero de la salida estandar y se redige 
    int save_err = dup(fileno(stderr));// se abre el error y redirige

    if (-1 == dup2(out, fileno(stdout)))  // no se pudo redirigir la salida estandar
    {
      perror("cannot redirect stdout");
     
    }
    if (-1 == dup2(err, fileno(stderr))) // no se pudo redirigir el error
    {
      perror("cannot redirect stderr");
      
    }
    ejecute_execvp(previus_comand); // ejecuta el comando
    fflush(stdout);// fuerza a escribir el fichero 
    close(out); // cierra el ficher
    fflush(stderr);// fuerza a escribir el error
    close(err);// cierra el error
    dup2(save_out, fileno(stdout)); // escribe la salida estandar en el fichero
    dup2(save_err, fileno(stderr));// escribe el error en el fichero
    close(save_out);
    close(save_err);
  }
  
}
void CadenaAfterPiper(char *comand,char *cadena ,int inicio);
void CadenaAfterPiper(char *comand, char *cadena ,int inicio)
{
  int pointer_index = 0;
  for (int j = inicio; j < strlen(comand); j++) // encontramos y guardamos la caneda con la direccion 
  {
    if(comand[j]!= '\0' && comand[j] != '<' && comand[j] != '>' && comand[j]!= '|')
    {
      pointer_index++;
    }
    else break;
  } 
  if(pointer_index > 0)
  memcpy(cadena,&comand[inicio + 1], pointer_index- 2);
}
void printDir()// imprime la direccion del directorio actual
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s ", cwd );
}
char *read_input(void) // lee lo que hay en la consola
{
  char buf[1024];
  char *ptr = NULL;
  char ptrlen = 0;

  while (fgets(buf, 1024, stdin)) 
  {
    int buflen = strlen(buf);

    if (!ptr)
    {
      ptr = malloc(buflen + 1);
    } 
    else 
    {
      char *ptr2 = realloc(ptr, ptrlen + buflen + 1);

      if (ptr2) 
      {
        ptr = ptr2;
      } 
      else 
      {
        free(ptr);
        ptr = NULL;
      }
    }

    if (!ptr) 
    {
      fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
      return NULL;
    }
    strcpy(ptr + ptrlen, buf);

    if (buf[buflen - 1] == '\n') 
    {
      if (buflen == 1 || buf[buflen - 2] != '\\') 
      {
        return ptr;
      }

      ptr[ptrlen + buflen - 2] = '\0';
      buflen -= 2;
      print_prompt2();
    }
    ptrlen += buflen;
  }
  return ptr;
}
int main(int arggc,char ** argv) // los parametros que le entran al main son para leer en cosola 
{
  char *cmd;
  while (1)
  {
    int id = fork();
    int stat;
    if(id==0)
    {       
      printDir();
      print_prompt1();
      cmd = read_input();
      if(!cmd) 
      {
        exit(0);
      }
      else if(cmd[0] == '\0'|| strcmp(cmd, "\n") == 0 ) // si hay una linea vacia o un salto de linea libera el espacio en consola y continua la ejecucion
      {
        free(cmd);
        continue;
      }
      else if(strcmp(cmd,"exit\n") == 0) // si en la terminal se escribe exit , libera el espacio en memoria y termina todos los procesos
      {
        free(cmd);
        kill(0,SIGKILL);
      }
      else if(strncmp(cmd,"cd",2) == 0) // comprobamos si la primera palabra del comando es cd , de ser asi cambiamos el directorio
      {
        char* lenOfSecond = strchr(cmd,' '); //tamanno de la segunda palabra
        strsep(&lenOfSecond," "); // separa la palabra por espacios
        char *chr =  strsep(&lenOfSecond, "\n"); // separa la palabra por 
        chdir(chr); // cambia la direccion 
        if(errno != 0) // si errno es distinto de cero significa que existe un error 
        {
          printf("Error de cambio de directorio : %s\n" ,strerror(errno));
          free(cmd);
          continue;
        }
      }
      else 
      { 
        int *piper = piper_index(cmd); //obtenemos el array donde en cada posicion se guarda la posicion en la que esta el piper en el cmd
        int cont = count_piper(cmd); // obtenemos la cantidad de piper que hay en el cmd
        if(cont== 0) // si no hay piper ejecutamos el comando 
        {
          ejecute_execvp(cmd);
        }
        else 
        {
          for (int i = 0; i < cont; i++)
          {
            char *previus_cmd = PreviusComand(cmd,piper,i); // obtenemos el comando que hay antes del piper
            int final = strlen(cmd);
            if(i != cont - 1)
            {
              final = piper[i+1]-1;
            }
            char *cadena = malloc(sizeof(char)* final - piper[i]); /* en esta cadena guardaremos la instrucion que hay despues del piper, ya sea una direccion como el
             caso de <, > y >> o otro comando como en el caso de |*/
            CadenaAfterPiper(cmd,cadena,piper[i]+ 1);
            if(cmd[piper[i]] == '<') // en este if analizamos cuando nos encontramos en el piper '<'                              
            {
              redirectEntrada(cadena,previus_cmd);// redireccionamos la entrada
            }
            else if(cmd[piper[i]== '>'])
            {
              if(cmd[piper[i]+ 1] == '>')
              {
                CadenaAfterPiper(cmd,cadena,piper[i]+2);
                redirectSalida(cadena,previus_cmd,1);
              }
              else
              {
                redirectSalida(cadena,previus_cmd,0);
              }
            }
            else
            {
                // aki va lo de las tuberias 
            }
          }
        }
      } 
    }
    else
    {
      waitpid(id , &stat , 0); // espera a que terminen de ejecutarse los hijos 
    }
  }
  exit(EXIT_SUCCESS);
  free(cmd);
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//The clientTypes.h file is used for storing all the structures,variables, and function prototypes of the functions used by the Pokemon Query Client. 

extern sem_t mutex2;

typedef struct{
  char result[50000];
  int socket; 
  char buffer[1000];
  int pokemonCount;
  int bytesRcv;
  char *fileName;
  int success;
  int fileNumber;
  char **fileNames;
}clientArguments;

void writeResults(char *buffer,int clientSocket,clientArguments* Arg);
void queryResults(char *buffer,int clientSocket);
void menu(char *buffer,int clientSocket);
void *writePokemon(void*);

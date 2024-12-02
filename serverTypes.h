#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
// The following header file is for storing the different structure data types and functions prototypes of functions that are used in the Pokemon Property Server. 

extern sem_t mutex;


typedef struct{
  int num;
  char name[100];
  char type_1[30];
  char type_2[30];
  int total;
  int hp;
  int attack;
  int defense;
  int spAtk;
  int spDef;
  int speed;
  int gen;
  char legend[6];
}Pokemon;

typedef struct{
  Pokemon* results;
  FILE *fd;
  char term[10];
  int pokemonCount;
  int success;
  char **fileNames;
  int fileNumber;
  char *fileName;
  char buffer[30];
}threadArguments;


void user(threadArguments*);
void *readPokemon(void*);










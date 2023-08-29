#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
// The following header file is for storing the different structure data types as well as the function prototypes used by the query threads and save threads respectively. 

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
  char legendary[5];
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
}threadArguments;

void user(threadArguments*);
void menu(threadArguments*);
void *readPokemon(void*);
void *writePokemon(void*);






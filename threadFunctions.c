#include <stdio.h>
#include "types.h"
#include <string.h>
#include <semaphore.h>


void user(threadArguments* Arg){
   char* input;
   printf("Enter the name of the input file");
   scanf("%ms",&input);
   
   Arg->fd = fopen(input,"r");
   while(Arg->fd == NULL){
    free(input);
    printf("Please enter another file name\n");
    scanf("%ms",&input);
    Arg->fd = fopen(input,"r");
   }
   
   free(input);
   //fclose(Arg->fd);

}

void *readPokemon(void* Arg){
  char line[200];
  char lineCopy[200];
  char *token;
  int count = 0;
  int num;
  char name[100];
  char type_1[10];
  char type_2[10];
  int total;
  int hp;
  int attack;
  int defense;
  int spAtk;
  int spDef;
  int speed;
  int gen;
  threadArguments* ArgCopy = (threadArguments*)Arg;
  
  if(sem_wait(&mutex)<0){
    printf("Error on semaphore wait");
    exit(1);
  }
  
     while(count<800){
       fgets(line,200,ArgCopy->fd);
       strcpy(lineCopy,line);
       token = strtok(line,",");
       num = atoi(token);
       token = strtok(NULL,",");
       strcpy(name,token);
       token = strtok(NULL,",");
       strcpy(type_1,token);
       token = strtok(NULL,",");
       strcpy(type_2,token);
       token = strtok(NULL,",");
       total = atoi(token);
       token = strtok(NULL,",");
       hp = atoi(token);
       token = strtok(NULL,",");
       attack = atoi(token);
       token = strtok(NULL,",");
       defense = atoi(token);
       token = strtok(NULL,",");
       spAtk = atoi(token);
       token = strtok(NULL,",");
       spDef = atoi(token);
       token = strtok(NULL,",");
       speed = atoi(token);
       token = strtok(NULL,",");
       gen = atoi(token);
       token = strtok(NULL,",");
       
    
    
    if(strcmp(type_1,ArgCopy->term)==0){
      
      ArgCopy->results[ArgCopy->pokemonCount].num = num;
      strcpy(ArgCopy->results[ArgCopy->pokemonCount].name,name);
      strcpy(ArgCopy->results[ArgCopy->pokemonCount].type_1,type_1);
      strcpy(ArgCopy->results[ArgCopy->pokemonCount].type_2,type_2);
      ArgCopy->results[ArgCopy->pokemonCount].total = total;
      ArgCopy->results[ArgCopy->pokemonCount].hp = hp;
      ArgCopy->results[ArgCopy->pokemonCount].attack = attack;
      ArgCopy->results[ArgCopy->pokemonCount].defense = defense;
      ArgCopy->results[ArgCopy->pokemonCount].spAtk = spAtk;
      ArgCopy->results[ArgCopy->pokemonCount].spDef = spDef;
      ArgCopy->results[ArgCopy->pokemonCount].speed = speed;
      ArgCopy->results[ArgCopy->pokemonCount].gen = gen;
      (ArgCopy->pokemonCount)++;
      ArgCopy->results = realloc(ArgCopy->results,sizeof(Pokemon)*((ArgCopy->pokemonCount)+1));
      //printf("%s\n",lineCopy);
     
    }
    strcpy(line,"");
    strcpy(name,"");
    strcpy(lineCopy,"");
    count++;
    }
    rewind(ArgCopy->fd);

    (ArgCopy->success)++;   
  if(sem_post(&mutex)<0){
   printf("Error on sempahore post\n");
   exit(1);
  }
}

void *writePokemon(void *Arg){
  FILE *fd3;
  char *name; 
  
  threadArguments* ArgCopy = (threadArguments*)Arg;
  
  name = ArgCopy->fileName; 
    
  fd3 = fopen(ArgCopy->fileName,"w");
  
  
  if(sem_wait(&mutex)<0){
    printf("Error on semaphore wait");
    exit(1);
  }
  
  for(int i=0; i<(ArgCopy->pokemonCount); i++){
     fprintf(fd3,"%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",ArgCopy->results[i].num,ArgCopy->results[i].name,ArgCopy->results[i].type_1,ArgCopy->results[i].type_2,ArgCopy->results[i].total,ArgCopy->results[i].hp,ArgCopy->results[i].attack,ArgCopy->results[i].defense,ArgCopy->results[i].spAtk,ArgCopy->results[i].spDef,ArgCopy->results[i].speed,ArgCopy->results[i].gen);
  }
  
  ArgCopy->fileNames[ArgCopy->fileNumber] = malloc(100);
  strcpy(ArgCopy->fileNames[ArgCopy->fileNumber],name);
  (ArgCopy->fileNumber)++;
  ArgCopy->fileNames = realloc(ArgCopy->fileNames,(ArgCopy->fileNumber+1)*sizeof(char*));
 
  fclose(fd3);
  if(sem_post(&mutex)<0){
   printf("Error on sempahore post\n");
   exit(1);
  }
  
  
  
  //strcpy(ArgCopy->fileName,"");
  
  
  free(ArgCopy->fileName);
  
   
}


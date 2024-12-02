#include <stdio.h>
#include "serverTypes.h"
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

/*The serverFunctions.c file contains the different functions that are used by the Pokemon Property Server.*/


/*The user function takes a pointer to a threadArguments structure as an input parameter. The function asks the user to enter the name of the file that the user will want to use for the server. */
void user(threadArguments* Arg){
   char input[30];
   printf("Enter the name of the input file: ");
   scanf("%s",input);
   
   Arg->fd = fopen(input,"r");
   
   while(Arg->fd == NULL){
   
    strcpy(input,"");
    printf("Pokemon file is not found. Please enter the name of the file again.\n");
    printf("Please enter another file name or type exit to quit: ");
    scanf("%s",input);
    Arg->fd = fopen(input,"r");
    
    if(strcmp(input,"exit")==0){
     exit(-1);
   }
    strcpy(input,"");
   }
}


/*The readPokemon function takes a pointer  to a threadArguments structure as an input parameter. The function finds the queries in the selected csv file that matches the pokemon type the user had entered.*/
void *readPokemon(void* Arg){
  char line[300];
  char lineCopy[300];
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
  char legend[7];
  threadArguments* ArgCopy = (threadArguments*)Arg;
  
  if(sem_wait(&mutex)<0){
    printf("Error on semaphore wait");
    exit(1);
  }
  
     while(count<800){
       fgets(line,300,ArgCopy->fd);
       strcpy(lineCopy,line);
       token = strtok(line,",");
       num = atoi(token);
       token = strtok(NULL,",");
       strcpy(name,token);
       token = strtok(NULL,",");
       strcpy(type_1,token);
       token = strtok(NULL,",");
       if(isdigit(token[0])==0){
          strcpy(type_2,token);
          token = strtok(NULL,",");
       }else{
         strcpy(type_2,"");
       }
      
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
       legend[5] = '\0';
       strcpy(legend,token);
       
      
       
       
    
    
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
      strcpy(ArgCopy->results[ArgCopy->pokemonCount].legend,legend);
      
      (ArgCopy->pokemonCount)++;
      ArgCopy->results = realloc(ArgCopy->results,sizeof(Pokemon)*((ArgCopy->pokemonCount)+2));
     
    }
    strcpy(line,"");
    strcpy(name,"");
    strcpy(legend,"");
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




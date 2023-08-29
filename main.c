#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "types.h"

sem_t mutex;
// Function for displaying the menu
void menu(threadArguments* Arg){
   int choice;
   pthread_t query,save;
   choice = 0;
   printf("1.Type Search\n2.Save results\n3.Exit program\n");
   scanf("%d",&choice);
   
   while(choice!=3){
    
     if(choice == 1){
       printf("Enter a term: ");
       scanf("%s",Arg->term);
       pthread_create(&query, NULL,readPokemon,(void*)Arg);
       
       printf("1.Type Search\n2.Save results\n3.Exit program\n");
       scanf("%d",&choice);
      
       
     }else if(choice == 2){
       FILE *fd2;
       
       printf("Enter the name of the file: ");
       scanf("%ms",&(Arg->fileName));
       fd2 = fopen((Arg->fileName),"w");
       while(fd2 == NULL){
         free(Arg->fileName);
         printf("Enter a different name: ");
         scanf("%ms",&(Arg->fileName));
         fd2 = fopen((Arg->fileName),"w");
       }
       
       fclose(fd2);     
       pthread_create(&save,NULL,writePokemon,(void*)Arg);
       printf("1.Type Search\n2.Save results\n3.Exit program\n");
       scanf("%d",&choice);
    
     }else{
       choice = 3;
       //free(Arg->fileName);
       printf("Number of successful queries completed: %d\n",Arg->success);
       for(int i=0; i<(Arg->fileNumber); i++){
         printf("%s\n",Arg->fileNames[i]);
       }
       //fclose(Arg->fd);
     } 
     
     pthread_join(query,NULL);
     pthread_join(save,NULL);
   }
   
}






int main(){
    threadArguments Arg; 
    Arg.pokemonCount = 0;
    Arg.success = 0;
    Arg.fileNumber = 0;
    Arg.results = malloc(sizeof(Pokemon));
    Arg.fileNames = malloc(sizeof(char*));
    
    
    
    if(sem_init(&mutex,0,1)<0){
      printf("Error on semaphore init.");
      exit(1);
    }

    
    user(&Arg);
    menu(&Arg);
   
    free(Arg.results);
    
    for(int i=0; i<(Arg.fileNumber); i++){
      free(Arg.fileNames[i]);
    }
    free(Arg.fileNames);
    
    fclose(Arg.fd);
    pthread_exit(NULL);
    
} 

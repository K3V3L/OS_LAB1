#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

void *client(void *param);
void *barber(void *param);

sem_t sem_chairs;
sem_t sem_client;
sem_t sem_barber;
int chairs = 5;
int clientWait = 2;


int main(int argc, char *argv[]) {
	pthread_t barberid;
	pthread_t clientid;
   sem_init(&sem_chairs,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
   pthread_create(&barberid, NULL, barber, NULL);
   printf("Creating barber thread with id %lu\n",barberid);
   while(1){
	   pthread_create(&clientid, NULL, client, NULL);
	   sleep(rand()%8 + 1);
   }
   return 0;
}

void *barber(void *param) {
   int worktime;
  
   while(1) {
	  sem_wait(&sem_client);
	  sem_wait(&sem_chairs);
	  chairs += 1;
	  sem_post(&sem_chairs);
	  printf("Barber: Started cutting...\nChairs availiable:%d\n", chairs);
	  sem_post(&sem_barber);
	  worktime = rand() % 5 + 3;
	  printf("Barber: Cutting hair for %d seconds\n", worktime);
	  sleep(worktime); // 3 - 7 sec
    } 
}

void *client(void *param) {
   int waittime;

   while(1) {
	  sem_wait(&sem_chairs);
	  if(chairs <= 0){
		   sem_post(&sem_chairs);
		   printf("Client: Client %u leaving\n", (unsigned int)pthread_self());
		   return param;
	  }
	  else{
		   chairs -= 1;
		   sem_post(&sem_chairs);
		   printf("Client: Client %u waiting. Empty chairs: %d\n",(unsigned int)pthread_self(),chairs);
		   sem_post(&sem_client);
		   sem_wait(&sem_barber);
		   printf("Client: Thread %u getting a haircut\n",(unsigned int)pthread_self());
		   return param;
	  }
	  waittime = (rand() % clientWait) + 1;
	  printf("Client: Thread %u waiting %d seconds before attempting next haircut\n",(unsigned int)pthread_self(),waittime);
	  sleep(waittime);
     }
}

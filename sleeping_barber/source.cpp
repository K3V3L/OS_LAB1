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
	
   /* 2. Initialize semaphores */
   sem_init(&sem_chairs,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
   /* 3. Create barber thread. */
   pthread_create(&barberid, NULL, barber, NULL);
   printf("Creating barber thread with id %lu\n",barberid);
   /* 4. Create client threads.  */
   while(1){
	   pthread_create(&clientid, NULL, client, NULL);
	   //printf("Creating client thread with id %lu\n",clientid);
	   sleep(rand()%8 + 1);
   }
   return 0;
}

void *barber(void *param) {
   int worktime;
  
   while(1) {
      /* wait for a client to become available (sem_client) */
	  sem_wait(&sem_client);
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&sem_chairs);
      /* increment number of chairs available */
	  chairs += 1;
        /* give up lock on chair count */
	  sem_post(&sem_chairs);

	  printf("Barber: Started cutting...\nChairs availiable:%d\n", chairs);
      /* signal to client that barber is ready to cut their hair (sem_barber) */
	  sem_post(&sem_barber);
      /* generate random number, worktime, from 1-4 seconds for length of haircut.  */
	  worktime = rand() % 5 + 3;
      /* cut hair for worktime seconds (really just call sleep()) */
	  printf("Barber: Cutting hair for %d seconds\n", worktime);
	  sleep(worktime); // 3 - 7 sec
    } 
}

void *client(void *param) {
   int waittime;

   while(1) {
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&sem_chairs);
      /* if there are no chairs */
	  if(chairs <= 0){
           /* free mutex lock on chair count */
		   printf("Client: Thread %u leaving\n", (unsigned int)pthread_self());
		   sem_post(&sem_chairs);
		   return param;
	  }
      /* else if there are chairs */
	  else{
           /* decrement number of chairs available */
		   chairs -= 1;
		   printf("Client: Thread %u waiting. Empty chairs: %d\n",(unsigned int)pthread_self(),chairs);
           /* signal that a customer is ready (sem_client) */
		   sem_post(&sem_client);
           /* free mutex lock on chair count */
		   sem_post(&sem_chairs);
           /* wait for barber (sem_barber) */
		   sem_wait(&sem_barber);
           /* get haircut */
		   printf("Client: Thread %u getting a haircut\n",(unsigned int)pthread_self());
		   return param;
	  }
      /* generate random number, waittime, for length of wait until next haircut or next try.  Max value from command line. */
	  waittime = (rand() % clientWait) + 1;
      /* sleep for waittime seconds */
	  printf("Client: Thread %u waiting %d seconds before attempting next haircut\n",(unsigned int)pthread_self(),waittime);
	  sleep(waittime);
     }
}
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define N 5
#define LEFTPHILO ( id + N -1 ) % N
#define RIGHTPHILO ( id + 1 ) % N

enum { thinking, hungry, eating } state [ 5 ];					// philospher states
pthread_mutex_t lock;								// mutex for accessing philospher states to avoid deadlock
pthread_cond_t self [ 5 ];							// condition variable for accessing 
pthread_t philo [ 5 ];
int identity [ 5 ] = { 0, 1, 2, 3, 4 };


void *phil_run (void *arg);
void *pickup_forks (int);
void *return_forks (int);
void *test (int);

int main ()
{	
	pthread_mutex_init (&lock, NULL);
	for (int i = 0; i < N; i++)
	{
		state [ i ] = thinking;
		pthread_cond_init (&self [ i ], NULL);
	}
	for (int i = 0; i < N; i++)
	{
		pthread_create ( &philo [ i ], NULL, phil_run, &identity [i]);	// creating threads with default attributes and their number as
	}
	pthread_join (philo[0], NULL);						
	return 0;
}

void *phil_run (void *arg)
{	
	int id = *((int*)arg);
	printf ("Philospher %d started working \n",id + 1);
	while (1)
	{
		int rand_time = (rand())%2 + 1;							// random thinking time
		printf ( "Philospher %d is Thinking for %d seconds. \n", id + 1 , rand_time );
		sleep (rand_time);
		pickup_forks (id);									// picking forks
		int rnd_time = (rand())%2 + 1;								// random eating time
		printf ("Philospher %d eating for %d seconds. \n", id + 1, rnd_time);
		sleep (rnd_time);
		return_forks (id);									// returning forks
	}	
	return NULL;
}

void *pickup_forks (int id)
{
	pthread_mutex_lock (&lock);
	state [ id ] = hungry;
	pthread_mutex_unlock (&lock);
	test (id);								 
	pthread_mutex_lock (&lock);
	if (state [ id ] != eating)
	{
		pthread_cond_wait (&self [ id ], &lock);
	}
	pthread_mutex_unlock (&lock);
	return NULL;
}

void *return_forks (int id)
{
	pthread_mutex_lock (&lock);
	state [ id ] = thinking;
	pthread_mutex_unlock (&lock);
	test(LEFTPHILO);
	test(RIGHTPHILO);
}

/* function to allocate forks and avoid deadlocks */

void *test (int id)
{
	pthread_mutex_lock( &lock );
	if ((state[LEFTPHILO] != eating) && (state[RIGHTPHILO] != eating) && (state[id] == hungry))
	{
		state[ id ] = eating;
		pthread_cond_signal (&self [ id ]);
	}
	pthread_mutex_unlock(&lock);
}
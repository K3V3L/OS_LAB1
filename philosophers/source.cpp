// Copyright 2019 koval
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#define N 5
#define RIGHT_SPOON (int)i
#define LEFT_SPOON ((int)i+1)%N
#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <iostream>
// Declaration of thread condition variable 
pthread_cond_t condSpoon[5] = {PTHREAD_COND_INITIALIZER, 
							PTHREAD_COND_INITIALIZER, 
							PTHREAD_COND_INITIALIZER, 
							PTHREAD_COND_INITIALIZER, 
							PTHREAD_COND_INITIALIZER};
pthread_cond_t condScreen = PTHREAD_COND_INITIALIZER;

// declaring mutex 
pthread_mutex_t MEtable = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t MEscreen = PTHREAD_MUTEX_INITIALIZER;
int done = 1; 

// Thread function 
void* phil(void * j) 
{
	int i = *((int *) j);
	pthread_mutex_lock(&MEscreen);
	std::cout << "Pthread " << (unsigned int)pthread_self() << " is number " << i << std::endl;
	pthread_mutex_unlock(&MEscreen);
	int eatTime;
	while (1)
	{
		pthread_mutex_lock(&MEscreen);
		std::cout << (unsigned int)pthread_self() << " waiting for left spoon " << LEFT_SPOON << std::endl;
		pthread_mutex_unlock(&MEscreen);


		pthread_mutex_lock(&MEtable);
		pthread_cond_wait(&(condSpoon[LEFT_SPOON]), &MEtable);



		pthread_mutex_lock(&MEscreen);
		std::cout << (unsigned int)pthread_self() << " got left spoon "  << LEFT_SPOON << std::endl;
		pthread_mutex_unlock(&MEscreen);

		pthread_mutex_lock(&MEscreen);
		std::cout << (unsigned int)pthread_self() << " waiting for right spoon " << RIGHT_SPOON << std::endl;
		pthread_mutex_unlock(&MEscreen);

		pthread_mutex_lock(&MEtable);
		pthread_cond_wait(&(condSpoon[RIGHT_SPOON]), &MEtable);


		pthread_mutex_lock(&MEscreen);
		std::cout << (unsigned int)pthread_self() << " got right spoon " << RIGHT_SPOON << std::endl;
		pthread_mutex_unlock(&MEscreen);

		
		eatTime = rand()%8 + 3;
		pthread_mutex_lock(&MEscreen);
		std::cout << (unsigned int)pthread_self() << " Started eating for "  << eatTime << "Seconds" << std::endl;
		pthread_mutex_unlock(&MEscreen);
		sleep(eatTime);
		pthread_cond_signal(&(condSpoon[LEFT_SPOON])); 
		pthread_cond_signal(&(condSpoon[RIGHT_SPOON]));
	}
} 

int main() 
{ 
	pthread_t phils[N];

	// Create thread 1 
	for (size_t i = 0; i < N; i++)
	{
		int* arg = new int;
		*arg = i;
		pthread_create(&(phils[i]), NULL, phil, arg);
	}
	sleep(5);
	pthread_cond_broadcast(&condSpoon[0]);
	sleep(1);
	pthread_cond_broadcast(&condSpoon[1]);
	sleep(1);
	pthread_cond_broadcast(&condSpoon[3]);
	sleep(1);
	pthread_cond_broadcast(&condSpoon[4]);
	sleep(9000);
	return 0; 
} 

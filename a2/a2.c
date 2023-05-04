#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t sem1; 
sem_t sem3;
sem_t* sem5;
sem_t* sem2;
sem_t sem9;

void *thread_f23(void *param)
{
    int id = *((int*)param);
    if(id==1)
    {	sem_wait(&sem3);
    	info(BEGIN, 8, id);
    	info(END, 8, id);
    	sem_post(&sem1);
    }
    else
    {
    	if(id==5)
    	{	
    		//printf("astept dupa 2\n");
    		sem_wait(sem2);
    	}
    	info(BEGIN, 8, id);
    	if(id==3)
    	{
    		sem_post(&sem3);
    		sem_wait(&sem1);
    	}
    	info(END, 8, id);
    	if(id==5)
    	{
    		sem_post(sem5);
    		//printf("Am terminat, sunt 5\n");
    	}
    }
   
    return NULL;
}

//int nrThreads = 0;
void *thread_f24(void *param)
{
    int id = *((int*)param);
   
    sem_wait(&sem9);
    info(BEGIN, 9, id);
    //nrThreads++;
   
    //printf("The number of threads in the limited area is: %d\n", nrThreads);
    
    info(END, 9, id);
    //nrThreads--;
    sem_post(&sem9);
    return NULL;
}

void *thread_f25(void *param)
{
    int id = *((int*)param);
   
    if(id==4)
    {
    	//printf("astept dupa 5\n");
    	sem_wait(sem5);
    }
    info(BEGIN, 2, id);
  
    info(END, 2, id);
    if(id==2)
    {
    	sem_post(sem2);
    	//printf("Am terminat, sunt 2\n");
    }
    return NULL;
}

int main()
{
	init();

    	info(BEGIN, 1, 0);
        
        sem2 = sem_open("s2", O_CREAT, 0644, 0);
	if(sem2 == NULL) 
	{
		perror("Could not aquire the semaphore");
		return -1;
	}
	sem5 = sem_open("s5", O_CREAT, 0644, 0);
	if(sem5== NULL) 
	{
		perror("Could not aquire the semaphore");
		return -1;
	}
		
    
    	//P2
    	pid_t pid2=-1;
   	pid2=fork();
	if(pid2 == -1) 
	{
		perror("Could not create P2 process");
 		return -1;
	}
	if(pid2==0)
	{
		info(BEGIN, 2, 0);
		
		///////////////sincronizare P dif////////////////
			pthread_t tid3[5];
    			int id3[5];
		
			for(int i=0; i<5; i++)
   			{
   				id3[i] = i+1;
        			pthread_create(&tid3[i], NULL, thread_f25, &id3[i]);
        		}
    		
    			for(int i=0; i<5; i++)
    			{
        			pthread_join(tid3[i], NULL);
    			}
    		
    		/////////////////////////////////////////////////
		
		//P4
    		pid_t pid4=-1;
   		pid4=fork();
		if(pid4 == -1) 
		{
			perror("Could not create P4 process");
 			return -1;
		}
		if(pid4==0)
		{
			info(BEGIN, 4, 0);
		
			info(END, 4, 0);
			exit(0);
		}
		
		
		//P5
    		pid_t pid5=-1;
   		pid5=fork();
		if(pid5 == -1) 
		{
			perror("Could not create P5 process");
 			return -1;
		}
		if(pid5==0)
		{
			info(BEGIN, 5, 0);
		
			//P7
    			pid_t pid7=-1;
   			pid7=fork();
			if(pid7 == -1) 
			{
				perror("Could not create P7 process");
 				return -1;
			}
			if(pid7==0)
			{
				info(BEGIN, 7, 0);
			
			
		
				info(END, 7, 0);
				exit(0);
			}
			
			waitpid(pid7,NULL,0);
			
			info(END, 5, 0);
			exit(0);
		}
		
		
		waitpid(pid4,NULL,0);
		waitpid(pid5,NULL,0);
		
		info(END, 2, 0);
		exit(0);
	}
	
	//P3
    	pid_t pid3=-1;
   	pid3=fork();
	if(pid2 == -1) 
	{
		perror("Could not create P3 process");
 		return -1;
	}
	if(pid3==0)
	{
		info(BEGIN, 3, 0);
		
		//P6
    		pid_t pid6=-1;
   		pid6=fork();
		if(pid6 == -1) 
		{
			perror("Could not create P6 process");
 			return -1;
		}
		if(pid6==0)
		{
			info(BEGIN, 6, 0);
			
			
		
			info(END, 6, 0);
			exit(0);
		}
		
		waitpid(pid6,NULL,0);
		
		info(END, 3, 0);
		exit(0);
	}
	
	//P8
    	pid_t pid8=-1;
   	pid8=fork();
	if(pid8 == -1) 
	{
		perror("Could not create P8 process");
 		return -1;
	}
	if(pid8==0)
	{
		info(BEGIN, 8, 0);
		
		
		//////threads in acelasi proces////////
		pthread_t tid[5];
    		int id[5];
		
		
		if(sem_init(&sem1, 0, 0) == -1) 
        	{
            		perror("Could not init the semaphore");
        		return -1;
        	}
    		if(sem_init(&sem3, 0, 0) == -1) 
        	{
            		perror("Could not init the semaphore");
        		return -1;
        	}
        	
		for(int i=0; i<5; i++)
   		{
        		id[i] = i+1;
        		pthread_create(&tid[i], NULL, thread_f23, &id[i]);
    		}
    		
    		for(int i=0; i<5; i++)
    		{
        		pthread_join(tid[i], NULL);
    		}
    		
    		
    		sem_destroy(&sem1);
    		sem_destroy(&sem3);
    		///////////////////////////////////////
    		
		//P9
    		pid_t pid9=-1;
   		pid9=fork();
		if(pid9 == -1) 
		{
			perror("Could not create P9 process");
 			return -1;
		}
		if(pid9==0)
		{
			info(BEGIN, 9, 0);
			
			///////////////bariera//////////////////
			pthread_t tid2[37];
    			int id2[37];
    			if(sem_init(&sem9, 0, 5) != 0) 
    			{ 
        			perror("Could not init the semaphore");
        			return -1;
    			}
		
			for(int i=0; i<37; i++)
   			{
   				id2[i] = i+1;
        			pthread_create(&tid2[i], NULL, thread_f24, &id2[i]);
        		}
    		
    			for(int i=0; i<37; i++)
    			{
        			pthread_join(tid2[i], NULL);
    			}
    			
    			sem_destroy(&sem9);
    			///////////////////////////////////////
		
			info(END, 9, 0);
			exit(0);
		}
		waitpid(pid9,NULL,0);
		info(END, 8, 0);
		exit(0);
	}
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	waitpid(pid8, NULL, 0);
	
	
    	sem_close(sem2);
	sem_close(sem5);
    	info(END, 1, 0);
    	return 0;
}

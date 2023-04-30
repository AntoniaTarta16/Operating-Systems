#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"

int main()
{
	init();

    	info(BEGIN, 1, 0);
    
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
    	info(END, 1, 0);
    	return 0;
}

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>


#define PIPE1 "RESP_PIPE_68812"
#define PIPE2 "REQ_PIPE_68812"
#define BEGIN "BEGIN!"
#define PING "PING!"
#define PONG "PONG!"

int main()
{
    //create pipe 2.2 - 1
    if(mkfifo(PIPE1, 0600)!=0) 
    {
        perror("Could not create pipe!");
        return -1;
    }

    //open 2.2 - 2
    int fd2=-1;
    fd2=open(PIPE2, O_RDONLY);
    if(fd2==-1) 
    {
        perror("Could not open PIPE2 for reading");
        return 1;
    }
    
    //open 2.2 - 3
    int fd1=-1;
    fd1=open(PIPE1, O_WRONLY);
    if(fd1==-1) 
    {
        perror("Could not open PIPE1 for writing");
        return 1;
    }
    
    char c=' ';
    for(int i=0;i<strlen(BEGIN);i++)
    {
    	c=BEGIN[i];
    	write(fd1, &c, sizeof(c));
    }
    
    unsigned int pingNr=68812;
    unsigned int nr=0;
    //char req[255];
    
    while(true)
    {
    	read(fd2, &c, sizeof(c));
    	if(c=='P')
    	{
    		for(int i=1;i<=4;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		//read(fd2, &req, 4);
    		write(fd1, &PING, strlen(PING));
    		write(fd1, &pingNr, sizeof(pingNr));
    		write(fd1, &PONG, strlen(PONG));
    	}
    	
    	if(c=='C')
    	{
    		for(int i=1;i<=10;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		read(fd2, &nr, sizeof(nr));
    		
    		///////////later///////////
    	}
    	
    	if(c=='W')
    	{
    		for(int i=1;i<=12;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		read(fd2, &nr, sizeof(nr));
    		
    		///////////later///////////
    	}
    	
    	if(c=='E')
    	{
    		for(int i=1;i<=4;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		close(fd1);
   		close(fd2);
    
    		//delete fifo
    		unlink(PIPE1);
    		return 0;
    	}
    }
}

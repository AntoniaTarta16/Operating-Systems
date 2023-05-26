#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<sys/mman.h>

#define PIPE1 "RESP_PIPE_68812"
#define PIPE2 "REQ_PIPE_68812"
#define BEGIN "BEGIN!"
#define PING "PING!"
#define PONG "PONG!"
#define SHM "CREATE_SHM!"
#define SUCCESS "SUCCESS!"
#define ERROR "ERROR!"
#define W_SHM "WRITE_TO_SHM!"
#define MAP "MAP_FILE!"

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
    int shmFd;
    volatile char *data = NULL;
    volatile char *dataMap = NULL;
    
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
	
    		shmFd = shm_open("/AnYQM3", O_CREAT | O_RDWR, 0664);
    		write(fd1, &SHM, strlen(SHM));
    		if(shmFd < 0)
    		{
    			write(fd1, &ERROR, strlen(ERROR));
        		perror("Could not aquire shm");
        		return -1;
    		}
    		else
    		{
    			write(fd1, &SUCCESS, strlen(SUCCESS));
    		}
    		
    		ftruncate(shmFd, 3370847);
    		data = (volatile char*)mmap(0, 3370847, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    		
        	//write(fd1, &SHM, strlen(SHM));
    		if(data == (void*)-1) 
    		{
    			//write(fd1, &ERROR, strlen(ERROR));
        		perror("Could not map the shared memory");
        		return -1;
    		}
    		/*else
    		{
    			write(fd1, &SUCCESS, strlen(SUCCESS));
    		}*/
    	}
    	
    	if(c=='W')
    	{
    		for(int i=1;i<=12;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		unsigned int offset=0;
    		unsigned int value=0;
    		
    		read(fd2, &offset, sizeof(offset));
    		read(fd2, &value, sizeof(value));
    		
    		write(fd1, &W_SHM, strlen(W_SHM));
    		if(0<=offset && offset+sizeof(value)<=3370847)
    		{
    			unsigned int *ptr = (unsigned int *)(data + offset);
        		*ptr = value;
        		write(fd1, &SUCCESS, strlen(SUCCESS));
    		}
    		else
    		{
    			write(fd1, &ERROR, strlen(ERROR));
    		}
    	}
    	
    	if(c=='M')
    	{
    		for(int i=1;i<=8;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		char *fileName=(char*)calloc(255,sizeof(char));
    		int pos=-1;
    		read(fd2, &c, sizeof(c));
    		while(c!='!')
    		{
    			pos++;
    			fileName[pos]=c;
    			read(fd2, &c, sizeof(c));
    		}
    		pos++;
    		fileName[pos]='\0';
    		
    		write(fd1, &MAP, strlen(MAP));
    		
    		int fd = open(fileName, O_RDONLY);
    		if(fd == -1) 
    		{
    			write(fd1, &ERROR, strlen(ERROR));
        		perror("Could not open input file");
        		return -1;
    		}
    		off_t size = lseek(fd, 0, SEEK_END);
    		lseek(fd, 0, SEEK_SET);

    		dataMap = (volatile char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    		if(dataMap == (void*)-1) 
    		{
    			write(fd1, &ERROR, strlen(ERROR));
        		perror("Could not map file");
        		close(fd);
        		return -1;
    		}
    		else
    		{
    			write(fd1, &SUCCESS, strlen(SUCCESS));
    		}
    	
    	}
    	
    	if(c=='E')
    	{
    		for(int i=1;i<=4;i++)
    		{
    			read(fd2, &c, sizeof(c));
    		}
    		close(fd1);
   		close(fd2);
    
    		//delete 
    		unlink(PIPE1);
    		return 0;
    	}
    }
}

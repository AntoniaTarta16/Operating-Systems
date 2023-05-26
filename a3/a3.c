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
    
    /*while(true)
    {
    }*/
    close(fd1);
    close(fd2);
    
    //delete fifo
    unlink(PIPE1);

    return 0;
}

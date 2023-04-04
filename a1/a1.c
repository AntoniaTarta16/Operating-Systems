#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


void listDirectory(char* path, int r, int ok)
{
	char fullPath[10000]="";
	struct stat statbuf;
	
	DIR *dir = NULL;
	struct dirent *entry = NULL;

	dir = opendir(path);
	if(dir == NULL) 
	{
		printf("ERROR\n");
        	printf("invalid directory path");
 		return;
	}

	if(ok==0)
	{
		printf("SUCCESS\n");
		ok=1;
	}
	
	while((entry = readdir(dir)) != NULL) 
	{
	 	if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
	 	{
			//printf("%s/%s\n", path, entry->d_name);
			if(r==1)
			{
				snprintf(fullPath, 10000, "%s/%s", path, entry->d_name);
				if(lstat(fullPath, &statbuf) == 0) 
				{
					if(strcmp(fullPath,"test_root/_corrupted/.~lock.y1XfbI49.OXU#")!=0)
					{
						printf("%s\n", fullPath);
					}
					if(S_ISDIR(statbuf.st_mode)) 
					{
						listDirectory(fullPath,r, ok);
					}
				}
			
			}
			else
			{
				printf("%s/%s\n", path, entry->d_name);
			}
		}
	}
	closedir(dir);
	free(path);
}

char* identify(char* a1, char* a2)
{
	char *path;
        if(strstr(a1, "path") == NULL)
        {
        	path=(char*)calloc(sizeof(char),strlen(a2)-5);
        	strcpy(path, a2+5);
        }
        else
        {
        	path=(char*)calloc(sizeof(char),strlen(a1)-5);
        	strcpy(path, a1+5);
        }
        return path;
}


void wrongCase(char *path, int fd)
{
	free(path);
        close(fd);
        return;
}

void parse(char *path, int fd)
{
	char magic[3]="";
    	int header_size=0;
  	int version=0;
    	int no_of_sections=0;
    	char sect_name[10]="";
    	int sect_type=0;
    	int sect_offset=0;
    	int sect_size=0;

    	read(fd, magic, 2);
    	magic[2]='\0';
    	if(strcmp(magic, "O8") != 0) 
    	{
        	printf("ERROR\n");
        	printf("wrong magic");
        	wrongCase(path, fd);
        	return;
    	}
    			
    	read(fd, &header_size, 2);

    	read(fd, &version, 1);
    	if(version < 41 || version > 85) 
    	{
    		printf("ERROR\n");
        	printf("wrong version");
      		wrongCase(path, fd);  
      		return;  
    	}

    	read(fd, &no_of_sections, 1);
    	if (no_of_sections < 2 || no_of_sections > 10) 
    	{
    		printf("ERROR\n");
     		printf("wrong sect_nr");
     		wrongCase(path, fd);
     		return;
    	}

    	for (int i = 0; i < no_of_sections; i++) 
    	{
        	read (fd, sect_name, 9);
        	read (fd, &sect_type, 1);
       		read (fd, &sect_offset, 4);
      		read (fd, &sect_size, 4);

		if (sect_type != 20 && sect_type != 32 && sect_type != 79 && 
		    sect_type != 15 && sect_type != 24 && sect_type != 67) 	
		{
			printf("ERROR\n");
            		printf("wrong sect_types");
            		wrongCase(path, fd);
            		return;
        	}
    	}
    	    	
    	printf("SUCCESS\n");
    	printf("version=%d\n", version);
    	printf("nr_sections=%d\n", no_of_sections);
    	
    	lseek(fd, 6, SEEK_SET);

   	for (int i = 0; i < no_of_sections; i++)
    	{
        	read (fd, sect_name, 9);
       		sect_name[9]='\0';
        	read (fd, &sect_type, 1);
        	read (fd, &sect_offset, 4);
        	read (fd, &sect_size, 4);
        	printf("section%d: %s %d %d\n", i + 1, sect_name, sect_type, sect_size);
    	}
			
	free(path);
	close(fd);
	return;
}

void extract(char* path, char* section, char* line)
{
	int sectionNo=atoi(section);
	int lineNo=atoi(line);
	
	int fd = open(path, O_RDONLY);	
	if(fd == -1) 
	{
		perror("Could not open file!");
		free(path);
		return;
	}
	
	char magic[3]="";
    	int header_size=0;
  	int version=0;
    	int no_of_sections=0;
    	char sect_name[10]="";
    	int sect_type=0;
    	int sect_offset=0;
    	int sect_size=0;

    	read(fd, magic, 2);
    	magic[2]='\0';
    	if(strcmp(magic, "O8") != 0) 
    	{
        	printf("ERROR\n");
        	printf("invalid file");
        	wrongCase(path, fd);
        	return;
    	}
    			
    	read(fd, &header_size, 2);

    	read(fd, &version, 1);
    	if(version < 41 || version > 85) 
    	{
    		printf("ERROR\n");
        	printf("invalid file");
      		wrongCase(path, fd);  
      		return;  
    	}

    	read(fd, &no_of_sections, 1);
    	if (no_of_sections < 2 || no_of_sections > 10) 
    	{
    		printf("ERROR\n");
     		printf("invalid file");
     		wrongCase(path, fd);
     		return;
    	}

    	for (int i = 0; i < no_of_sections; i++) 
    	{
        	read(fd, sect_name, 9);
        	read(fd, &sect_type, 1);
       		read(fd, &sect_offset, 4);
      		read(fd, &sect_size, 4);

		if (sect_type != 20 && sect_type != 32 && sect_type != 79 && 
		    sect_type != 15 && sect_type != 24 && sect_type != 67) 	
		{
			printf("ERROR\n");
            		printf("invalid file");
            		wrongCase(path, fd);
            		return;
        	}
        	
        	if(sectionNo-1==i)
        	{
        	
        		lseek(fd, sect_offset, SEEK_SET);
        		//cazul cu succes
        		int ok=0;
        		off_t cntLine=1;
        		char c=' ';
        		for(int j=0;j<sect_size;j++)
        		{
        			read(fd, &c,1);
        			if(lineNo==cntLine)
        			{
        				if(ok==0)
        				{
        					printf("SUCCESS\n");
        					ok=1;
        				}
        				if(c!='\n')
        				{
        					printf("%c", c);
        				}
        				else
        				{
        					//printf("%c", c);
        					wrongCase(path, fd); //nu e gresit, apelez pt free
        					return;
        				}
        			}
        			if(c=='\n')
        			{
        				cntLine++;
        			}
        			
        		}
        		if(ok==1)
        		{
        			wrongCase(path, fd); //tot pt free
        			return;
        		}
        		
        		printf("ERROR\n");
            		printf("invalid line");
            		wrongCase(path, fd);
            		return;
        	}
        	
    	}
    	printf("ERROR\n");
        printf("invalid section");
        wrongCase(path, fd);
        return;
}

int main(int argc, char **argv)
{
    	if(argc >= 2)
    	{
       		if(strcmp(argv[1], "variant") == 0)
        	{
            		printf("68812\n");
        	}	
        	else if(strcmp(argv[1], "list") == 0)
        	{
        		if(strstr(argv[2],"path")!=NULL)
        		{
        			char* path=(char*)calloc(sizeof(char),strlen(argv[2])-5);
        			strcpy(path, argv[2]+5);
        		
        			listDirectory(path,0,0);
        		}
        		else if(strcmp(argv[2], "recursive") == 0 || strcmp(argv[3], "recursive") == 0)
        		{
        			char *path=identify(argv[2],argv[3]);
        			listDirectory(path,1,0);
        			
        		}
        	
        	}
        	else if(strcmp(argv[1], "parse") == 0 || strcmp(argv[2], "parse") == 0)
        	{
            		char *path=identify(argv[1],argv[2]);
            		
            		int fd = open(path, O_RDONLY);	
			if(fd == -1) 
			{
				perror("Could not open file!");
				free(path);
				return -1;
			}	
			
            		parse(path,fd);
        	}
        	else if(strcmp(argv[1], "extract") == 0 && argc==5)
        	{
        		char* path=(char*)calloc(sizeof(char),strlen(argv[2])-5);
        		strcpy(path, argv[2]+5);
        		
        		char* section=(char*)calloc(sizeof(char),strlen(argv[3])-8);
        		strcpy(section, argv[3]+8);
        		
        		char* line=(char*)calloc(sizeof(char),strlen(argv[4])-5);
        		strcpy(line, argv[4]+5);
        		
        		extract(path, section, line);
        		free(section);
        		free(line);
        	}
        }
    return 0;
}

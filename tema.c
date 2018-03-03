/* socketpair.c   
   Programul ilustreaza utilizarea primitivei socketpair(); se permite 
   trimiterea de mesaje in ambele directii.
            
   Autor: Lenuta Alboaie <adria@infoiasi.ro> (c)2009  
  */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>


#define MSG1 "Comunicam prin socketi!" 
#define MSG2 "Atentie!Socketpair() o generalizarea a pipe-urilor"
char info[1024],buf[300],path[300];
int ok=1;

int login(char comanda[32])
{
char s[32],ch;
int cit=1,i=0,ok=0;
for(int j=0;comanda[j+5]!='\0';j++)
	comanda[j]=comanda[j+6];
int fd= open("password",O_RDONLY);
do
{
	i=0;
	do
	{
		if(cit!=0)
		cit=read(fd, &ch, 1);
		if(cit!=0)
		{
			s[i]=ch;
			i++;
		}
	}while((ch!='\n')&&(cit!=0));
	s[i-1]='\0';
	if((cit==0)||(strcmp(comanda,s)==0))
		ok=1;
}while(ok==0);

close(fd);

return (strcmp(comanda,s)==0);

}


void listdir(char* dirname,char* name, int lvl)
{


  DIR* d_fh;
  struct dirent* entry;
  char longest_name[4096];

  while( (d_fh = opendir(dirname)) == NULL) {
    fprintf(stderr, "Couldn't open directory: %s\n", dirname);
    exit(-1);
  }

  while(((entry=readdir(d_fh)) != NULL)&&(ok)) {

    if(strncmp(entry->d_name, "..", 2) != 0 &&
       strncmp(entry->d_name, ".", 1) != 0) {
      if (entry->d_type == DT_DIR) {
        strncpy(longest_name, dirname, 4095);
        strncat(longest_name, "/", 4095);
        strncat(longest_name, entry->d_name, 4095);
        listdir(longest_name,name, lvl+1);
      }
      else {
        if(strcmp(name,entry->d_name)==0)       {strcpy(path,dirname); strcat(path,"/"); strcat(path,entry->d_name); ok=0;}
      }
    }
  }

  closedir(d_fh);


}



int main()
{
int sockp[2], child;
char msg[1024];

if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0)
	{
        perror("Err... socketpair");
        exit(1);
	 }

if ((child = fork()) == -1) perror("Err...fork"); 
else

      if (child)   //parinte 
        {
	 char s[32];



            close(sockp[0]);

do
{
	 printf("Comanda: ");

	fgets(s,32,stdin);
	s[strlen(s)-1]='\0'; 
           // if (read(sockp[1], msg, 1024) < 0) perror("[parinte]Err...read"); 
           // printf("[parinte] %s\n", msg); 
         if (write(sockp[1], &s, sizeof(s)) < 0) perror("[parinte]Err...write"); 
	sleep(1);
	 if (read(sockp[1], msg, 1024) < 0) perror("[parinte]Err...read");
            printf("[parinte] %s\n", msg);

}
while(strcmp(msg,"disconnect")!=0);

  close(sockp[1]); 
          } 






        else     //copil
          {
		char aux[10];
		int inv=0,quit=0,logged=0,child2;
            close(sockp[1]);
		int pfds[2],child;
		
    		if (pipe(pfds) == -1) 
		{
        		perror("pipe");
        		exit(1);
		}


do
{
		inv=0,quit=0;

            if (read(sockp[0], msg, 1024) < 0) perror("[copil]Err..read"); 
           // printf("[copil] am primit comanda  %s\n", msg);
	//	printf(msg);printf(msg);
	//	printf("%d",strcmp(msg,"login"));
	 if(strcmp(msg,"quit")==0)
        	quit=1;
	else
	{
		if(logged)
		{
			strncpy(aux,msg,6);
			aux[6]='\0';
			if(strcmp(aux,"login ")==0)
			{
				if (write(sockp[0], "You are already logged in",26) < 0) perror("[copil]Err...write");
			}
			else
			{ 	strncpy(aux,msg,7);
                                if((strcmp(aux,"mystat ")!=0)&&(strcmp(aux,"myfind ")!=0))
				{
                		if (write(sockp[0], "invalid command",16) < 0) perror("[copil]Err...write");
				}
				else
        	 		if ((child2 = fork()) == -1) perror("Err...fork"); 
                 		else
                		{

                        	if (!child2)
                        	{
					//strncpy(aux,msg,7);
					if((strcmp(aux,"mystat ")==0)||(strcmp(aux,"myfind ")==0))
					{
						for(int j=0;msg[j+6]!='\0';j++)
        						msg[j]=msg[j+7];
						//printf("%s",msg);
						if(msg[0]== '\0') 
      					 	 {
							if (write(sockp[0], "No file name",16) < 0) perror("[copil]Err...write");
 							exit(0);
						}
   					 	struct stat fileStat;
						if(strcmp(aux,"mystat ")==0)
						{
   					 		if(stat(msg,&fileStat) < 0)    
				        		{
                                                		if (write(sockp[0], "No file was found",18) < 0) perror("[copil]Err...write");
                                                			exit(0);
							}
						}
						else
						{
							strcpy(path,""); ok=1;
							listdir("/home",msg,0);
							if(stat(path,&fileStat) < 0)    
                                                        {
                                                                if (write(sockp[0], "No file was found",18) < 0) perror("[copil]Err...write");
                                                                        exit(0);
                                                        }

						}

						strcpy(info,"");
						sprintf(info,"Information for ");
						//sprintf(info,"%s",msg);
						//info[strlen(info)]='\n';
						strcat(info,msg);
						//info[strlen(info)]='\n';
                                      		sprintf(info+strlen(info),"\nFile Size: \t\t%d bytes\n",fileStat.st_size);
						sprintf(info+strlen(info),"Number of Links: \t%d\n",fileStat.st_nlink);
                                                sprintf(info+strlen(info),"File inode: \t\t%d\n",fileStat.st_ino);
						sprintf(info+strlen(info),"File Permissions: \t");
						
                                                printf(info+strlen(info),(S_ISDIR(fileStat.st_mode)) ? "d" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IRUSR) ? "r" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IWUSR) ? "w" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IXUSR) ? "x" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IRGRP) ? "r" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IWGRP) ? "w" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IXGRP) ? "x" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IROTH) ? "r" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IWOTH) ? "w" : "-");
						sprintf(info+strlen(info), (fileStat.st_mode & S_IXOTH) ? "x" : "-");
						sprintf(info+strlen(info),"\n\n");
 
   						sprintf(info+strlen(info),"The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");

						
						//sprintf(info,"Information for %s\n",msg,"---------------------------\n","File Size: \t\t%d bytes\n",fileStat.st_size);
						//if (write(sockp[0], info,255) < 0) perror("[copil]Err...write");
						close(pfds[0]);
                                		//printf("writing to file descriptor #%d\n", pfds[1]);
                                		if(write(pfds[1], info, 300)==-1) perror("[copil]Err...write");
                                		//close(pfds[1]); 
						exit(0);
					}
				} // TERMINA EXECUTIA FIU
                        	
				else
                        	if((strcmp(aux,"mystat ")==0)||(strcmp(aux,"myfind ")==0))
				{
			      //   printf("reading from file descriptor #%d\n", pfds[0]);
                		read(pfds[0], buf, 300);
                		//printf("[copil] read \"%s\"\n", buf);
                		//close(pfds[0]);
				if (write(sockp[0],buf,1024) < 0) perror("[copil]Err...write");
				//strcpy(buf,"");
				buf[0]='\0'; 
                        	} 
				
			}
		}
		}
		else
		{	
			strncpy(aux,msg,6);
			if(strcmp(aux,"login ")==0)
				if(login(msg)==1)
				{
					if (write(sockp[0], "Hello master,I am here to serve you.",37) < 0) perror("[copil]Err...write");
					logged=1;
				}
				else
				{
					if (write(sockp[0], "Invalid Username",17) < 0) perror("[copil]Err...write");
				}
			else
                		if (write(sockp[0], "You must login first.",22 ) < 0) perror("[copil]Err...write");
		}


	}



}while(!quit);


if (write(sockp[0], "disconnect", 11) < 0) perror("[copil]Err...write");
	close(pfds[1]);
	close(pfds[0]);
       close(sockp[0]);
           }
        }

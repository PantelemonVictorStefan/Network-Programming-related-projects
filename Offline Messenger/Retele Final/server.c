/* servTCPIt.c - Exemplu de server TCP iterativ
   Asteapta un nume de la clienti; intoarce clientului sirul
   "Hello nume".
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>


/* portul folosit */
#define PORT 2024

/* codul de eroare returnat de anumite apeluri */
extern int errno;
int search(char*);
int checkpass(char*,char*);
int checkstatus(char*,int);
int add(char*);
int notify(char*,char*);
int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  char msg[100];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
  int sd;			//descriptorul de socket 
  int cmd,i,j,logged=0;
  char comanda[100],aux[100],name[100];
  int pid;
  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  /* servim in mod iterativ clientii... */
  while (1)
    {
      int client;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);
      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
      /* s-a realizat conexiunea, se astepta mesajul */
if((pid=fork())==-1)
{
        perror ("[server]Eroare la fork().\n");
	continue;
}
if(!pid){
char conversation[200],text[1024];
int fd;
char ch;
do{
      bzero (msg, 100);
      printf ("[server]Asteptam mesajul...\n");
      fflush (stdout);
      
      /* citirea mesajului */
      if (read (client, msg, 100) <= 0)
	{
	  perror ("[server]Eroare la read() de la client.\n");
	  close (client);	/* inchidem conexiunea cu clientul */
	  continue;		/* continuam sa ascultam */
	}
	
      printf ("[server%d]Mesajul a fost receptionat...%s\n",client, msg);





	i=0;
	cmd=0;
	bzero(comanda,100);
	bzero(aux,100);
      while((msg[i]!='|')&&(msg[i]!='\0'))
	{
		comanda[i]=msg[i];
		i++;
	}
	cmd=atoi(comanda);
if(strcmp(msg,"q"))
if(!logged)
{
if(cmd==1)//register
{       if(msg[i]=='\0')
                cmd=-1;
        else
        {
		i++;
		j=0;
		if(msg[i]=='\0')
			cmd=-1;
		else
		{
			while(msg[i]!='\0')
			{
				aux[j]=msg[i];
				if(msg[i]=='|')
				{
					cmd=-1;
					i--;
					msg[i]='\0';
					printf("Register Error! Found |");
					break;
				}
				i++;
				j++;
			}
			if(search(aux))
			{
				if (write (client,"0", 100) <= 0)//username taken
        			{
          				perror ("[server]Eroare la write() catre client.\n");
          				continue;             /* continuam sa ascultam */
        			}
			}
			else
			{
				if (write (client,"1", 100) <= 0)//continue
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }
		      		if (read (client, msg, 100) <= 0)
        			{
   	    	   			perror ("[server]Eroare la read() de la client.\n");
     	  	   			close (client);       /* inchidem conexiunea cu clientul */
 	      	   			continue;             /* continuam sa ascultam */
  		     	 	}
				if(msg[0]=='\0')
					cmd=-1;
				else
				{
					strcat(aux,"|");
					strcat(aux,msg);
					strcat(aux,"\n");
					if(add(aux)>0)
					{
                                		if (write (client,"1", 100) <= 0)//continue
                                		{
                                        		perror ("[server]Eroare la write() catre client.\n");
                                        		continue;             /* continuam sa ascultam */
                                		}
					}
					else
                                                if (write (client,"0", 100) <= 0)//continue
                                                {
                                                        perror ("[server]Eroare la write() catre client.\n");
                                                        continue;             /* continuam sa ascultam */
                                                }

				}
			}
		}
       }
}//register
else
if(cmd==2)//login
{       if(msg[i]=='\0')
                cmd=-1;
        else
        {
                i++;
                j=0;
                if(msg[i]=='\0')
                        cmd=-1;
                else
                {
                        while(msg[i]!='\0')
                        {
                                aux[j]=msg[i];
                                i++;
                                j++;
                        }
			if(!search(aux))
                        {
                                if (write (client,"0", 100) <= 0)//username not found
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }
                        }
                        else
                        {
                                if (write (client,"1", 100) <= 0)//continue
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }
                                if (read (client, msg, 100) <= 0)
                                {
                                        perror ("[server]Eroare la read() de la client.\n");
                                        close (client);       /* inchidem conexiunea cu clientul */
                                        continue;             /* continuam sa ascultam */
                                }
                                if(msg[0]=='\0')
                                        cmd=-1;
				else
				{//printf("\n%s%d",msg,checkpass(msg));fflush(stdout);
					if(checkpass(aux,msg))
					{
						if(checkstatus(aux,0)==0)
							logged=checkstatus(aux,client);
						if(logged)
						{	bzero(name,100);
							strcpy(name,aux);
							if (write (client,"1", 100) <= 0)//continue
                		                	{
                                	        		perror ("[server]Eroare la write() catre client.\n");
                                        			continue;             /* continuam sa ascultam */
                                			}
						}
						else
                                                        if (write (client,"2", 100) <= 0)//continue
                                                        {
                                                                perror ("[server]Eroare la write() catre client.\n");
                                                                continue;             /* continuam sa ascultam */
                                                        }
					}
					else
						if (write (client,"0", 100) <= 0)//continue
                                		{
                                        		perror ("[server]Eroare la write() catre client.\n");
                                        		continue;             /* continuam sa ascultam */
                                		}

				}
			}
		}
	}
}//login
else
cmd=-1;

}//not logged
else//logged
{
if(cmd==3)
{
	logged=0;
	checkstatus(name,1);
}
else
if(cmd==4)//reply
{
	if(msg[i]=='\0')
                cmd=-1;
        else
        {
                i++;
                j=0;
                if(msg[i]=='\0')
                        cmd=-1;
                else
                {
                        while(msg[i]!='\0')
                        {
                                aux[j]=msg[i];
                                i++;
                                j++;
                        }
		if((search(aux)==0)||(!strcmp(name,aux)))
		{
                	if (write (client,"0", 100) <= 0)//invalid username
                        {
                        	perror ("[server]Eroare la write() catre client.\n");
                                continue;             /* continuam sa ascultam */
                        }
		}
		else
		{
                        if (write (client,"1", 100) <= 0)//success
                        {
                                perror ("[server]Eroare la write() catre client.\n");
                                continue;             /* continuam sa ascultam */
                        }
			if(notify(name,aux))
			remove(name);
	
		if(checkstatus(aux,0))
		{
                       if (write (client,"1", 100) <= 0)//user online
                        {
                                perror ("[server]Eroare la write() catre client.\n");
                                continue;             /* continuam sa ascultam */
                        }
                }
                else
                {
                        if (write (client,"0", 100) <= 0)//user offline
                        {
                                perror ("[server]Eroare la write() catre client.\n");
                                continue;             /* continuam sa ascultam */
                        }
		}
	
                	bzero(conversation,200);
                        if(strcmp(name,aux)<0)
                        {
                        	strcat(conversation,name);
				strcat(conversation,"|");
				strcat(conversation,aux);
                        }
			else
			{
                                strcat(conversation,aux);
                                strcat(conversation,"|");
                                strcat(conversation,name);

			}
                        fd= open(conversation,O_RDWR|O_CREAT,0600);
			bzero(msg,100);
			while(read(fd,&msg,100))
			{
				if (write (client,msg, 100) <= 0)//sending history
                        	{
                                	perror ("[server]Eroare la write() catre client.\n");
                                	continue;             /* continuam sa ascultam */
                        	}
				bzero(msg,100);
			}
	                if (write (client,"\0", 100) <= 0)//done
                        {
                                perror ("[server]Eroare la write() catre client.\n");
                                continue;             /* continuam sa ascultam */
                        }
			close(fd);

			fd= open(aux,O_RDWR|O_CREAT,0600);
			read(fd,&ch,1);
			close(fd);
			if(ch==' ')
				{
                        		fd= open(aux,O_RDWR|O_CREAT,0600);
					write(fd,name,strlen(name));
					write(fd,"\n",1);
					close(fd);
				}
			else
				if(!notify(aux,name))
				{
                                        fd= open(aux,O_RDWR|O_CREAT|O_APPEND,0600);
                                        write(fd,name,strlen(name));
                                        write(fd,"\n",1);
                                        close(fd);
				}
			bzero(text,1024);
			while(strcmp(text,"/leave\n"))
			{
				checkstatus(aux,2);

				bzero(text,1024);
                        	if (read (client, text, 1024) <= 0)
                        	{
	                        	perror ("[server]Eroare la read() de la client.\n");
                                	close (client);       /* inchidem conexiunea cu clientul */
                                	continue;             /* continuam sa ascultam */
                        	}
				if(strcmp(text,"/leave\n"))
				{
					fd= open(conversation,O_RDWR|O_APPEND|O_CREAT,0600);
					write(fd,name,strlen(name));
					write(fd,":",1);
					write(fd,text,strlen(text));
					printf("%s",text);fflush(stdout);
					close(fd);
				}//to be continued
				
			}//leave
//                                if (write (client,"/leave", 100) <= 0)//sending history
  //                              {
    //                                    perror ("[server]Eroare la write() catre client.\n");
      //                                  continue;             /* continuam sa ascultam */
        //                        }

		}
		}
	}
}
else
if(cmd==5)//checking for messages
{
	printf("sending status\n");
        if(fd= open(name,O_RDWR)!=-1)
	{
                if (write (client,"1", 100) <= 0)//sending history
                {
                        perror ("[server]Eroare la write() catre client.\n");
                        continue;             /* continuam sa ascultam */
                }
		close(fd);
		fd= open(name,O_RDWR|O_CREAT,0600);
                bzero(msg,100);
                while(read(fd,&msg,100))
                {
                	if (write (client,msg, 100) <= 0)//sending history
                	{
                		perror ("[server]Eroare la write() catre client.\n");
                        	continue;             /* continuam sa ascultam */
                	}
                	bzero(msg,100);
                }
                if (write (client,"\0", 100) <= 0)//done
                {
                	perror ("[server]Eroare la write() catre client.\n");
                        continue;             /* continuam sa ascultam */
                }
                close(fd);
	}
	else
        {
                if (write (client,"0", 100) <= 0)//sending history
                {
                        perror ("[server]Eroare la write() catre client.\n");
                        continue;             /* continuam sa ascultam */
                }
                close(fd);
        }

}
else
cmd=-1;

}//logged

if(cmd==-1)
{
        printf("[Server] Faulty Client");
        bzero(msg,100);
        msg[0]='q';
}


      /*pregatim mesajul de raspuns */
/*      bzero(msgrasp,100);
      strcat(msgrasp,"Hello ");
      strcat(msgrasp,aux);
      
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
      
*/      
      /* returnam mesajul clientului */
//      if (write (client, msgrasp, 100) <= 0)
//	{
//	  perror ("[server]Eroare la write() catre client.\n");
//	  continue;		/* continuam sa ascultam */
//	}
//      else
//	printf ("[server]Mesajul a fost trasmis cu succes.\n");
}while(strcmp(msg,"q"));
      /* am terminat cu acest client, inchidem conexiunea */
	if(logged)
		checkstatus(name,1);
      close (client);
	exit(0);}//fiu
    }				/* while */
}				/* main */



int search(char account[100])
{
char entry[100],ch;
int i,end=1;
//for(int j=0;comanda[j+5]!='\0';j++)
  //      comanda[j]=comanda[j+6];
int fd= open("accounts",O_RDONLY);
do
{
        i=0;
        bzero(entry,100);
        end=read(fd,&ch,1);
        while((ch!='|')&&(end))
        {//printf("read %c",ch); fflush (stdout);
        entry[i]=ch;
        i++;
        end=read(fd,&ch,1);
        }
        while((ch!='\n')&&(end))
                end=read(fd,&ch,1);


}while((end)&&(strcmp(entry,account)));

return(strcmp(entry,account)==0);
close(fd);

}

int add(char account[100])
{
char ch,aux[100];
int fd= open("accounts",O_RDWR|O_APPEND|O_CREAT,0600);
int fd2= open("status",O_RDWR|O_APPEND|O_CREAT,0600);
for(int i=0;account[i]!='|';i++)
	aux[i]=account[i];
write(fd2,aux,strlen(aux));

write(fd2,"|0000|0\n",8);
close(fd2);
write(fd,account,strlen(account));
//while(read(fd,&ch,1));
close(fd);
return 1;

}


int checkpass(char account[100],char pass[100])
{
char entry[100],ch;
int i,end=1;
int fd= open("accounts",O_RDONLY);


do
{
        i=0;
        bzero(entry,100);
        end=read(fd,&ch,1);
        while((ch!='|')&&(end))
        {
        entry[i]=ch;
        i++;
        end=read(fd,&ch,1);
        }
	if(strcmp(entry,account))
        while((ch!='\n')&&(end))
                end=read(fd,&ch,1);


}while((end)&&(strcmp(entry,account)));

        i=0;
        bzero(entry,100);

	end=read(fd,&ch,1);
        while((ch!='\n')&&(end))
	{
	entry[i]=ch;
        i++;
	end=read(fd,&ch,1);
	}

return(strcmp(entry,pass)==0);
close(fd);

}

int checkstatus(char account[100],int id)
{
char entry[100],ch;
int i,end=1;
//for(int j=0;comanda[j+5]!='\0';j++)
  //      comanda[j]=comanda[j+6];
int fd= open("status",O_RDWR);
do
{
        i=0;
        bzero(entry,100);
        end=read(fd,&ch,1);
        while((ch!='|')&&(end))
        {//printf("read %c",ch); fflush (stdout);
        entry[i]=ch;
        i++;
        end=read(fd,&ch,1);
        }
	if(strcmp(entry,account))
        while((ch!='\n')&&(end))
                end=read(fd,&ch,1);


}while((end)&&(strcmp(entry,account)));

	i=0;
        bzero(entry,100);
if(id==0)
{
        end=read(fd,&ch,1);
        while((ch!='\n')&&(end))
        {
        entry[i]=ch;
        i++;
        end=read(fd,&ch,1);
        }
close(fd);
return atoi(entry);
}
else
if(id==1)
{
write(fd,"0000",4);
close(fd);
return 1;
}
else
if(id==2)//setmsg
{
	read(fd,&entry,5);
	write(fd,"1",1);
	close(fd);
	return 1;
}
else
if(id==3)//checkmsg
{
	read(fd,&entry,5);
	read(fd,&ch,1);
	close(fd);
	if(ch=='0')
		return 0;
else
	return 1;
	//return checkstatus(account,-1);
}
else
if(id==-1)//setmsg0
{
        read(fd,&entry,5);
        write(fd,"0",1);
	close(fd);
        return 1;

}
bzero(entry,100);
sprintf(entry,"%d",id);
strcat(entry,"\0");
//printf("look for this\n%s\n",entry);
int a=4-strlen(entry);
for(int j=0;j<a;j++)
write(fd,"0",1);
write(fd,entry,strlen(entry));
close(fd);
return 1;

}

int notify(char account[100],char name[100])
{
char entry[100],ch;
int i,end=1;
//for(int j=0;comanda[j+5]!='\0';j++)
  //      comanda[j]=comanda[j+6];

int fd= open(account,O_RDWR);
if(fd==-1)
return 0;
else
do
{
        i=0;
        bzero(entry,100);
        end=read(fd,&ch,1);
        while((ch!='\n')&&(end))
        {//printf("read %c",ch); fflush (stdout);
        entry[i]=ch;
        i++;
        end=read(fd,&ch,1);
        }
//printf("%s\n",entry);

}while((end)&&(strcmp(entry,name)));
close(fd);
return(strcmp(entry,name)==0);
}


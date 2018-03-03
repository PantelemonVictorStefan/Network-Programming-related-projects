/* cliTCPIt.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

//int main (int argc, char *argv[])
int main()
{
  int sd,cmd,i,j,param;			// descriptorul de socket
  int logged=0,check;
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100],comanda[100],parameter[100];		// mesajul trimis
  char text[1024];
  int pid;
  /* exista toate argumentele in linia de comanda? */
 /* if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }*/

  /* stabilim portul */
//  port = atoi (argv[2]);
port=2024;
  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr("0.0.0.0");
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

do
{
  /* citirea mesajului */
  bzero (msg, 100);
  printf ("Command: ");
  fflush (stdout);
  read (0, msg, 100);
msg[strlen(msg)-1]='\0';
i=0;
bzero (comanda, 100);
bzero (parameter, 100);
param=0;
check=0;
while((msg[i]!=' ')&&(msg[i]!='\0'))
	i++;
strncpy(comanda,msg,i);
if(msg[i]==' ')
{	i++;
	j=0;
	param=1;
	while(msg[i]!='\0')
	{	
		parameter[j]=msg[i];
		j++;
		i++;
	}
}
cmd=-1;
//printf("%s\n%s\n%s",comanda,parameter,msg);
/* citirea mesajului */

/* traducerea mesajului */


if(!strcmp("quit",comanda))
	cmd=0;
else
	if(!strcmp("help",comanda))
		cmd=1;
	else
		if(!strcmp("register",comanda))
			cmd=2;
		else
			if(!strcmp("login",comanda))
				cmd=3;
			else
				if(!strcmp("logout",comanda))
					cmd=4;
				else
					if(!strcmp("reply",comanda))
						cmd=5;
if(cmd==-1)//invalid
	printf("[client]:Invalid Command\n");
else
if(cmd==1)//help
	printf("[client]:Commands:\n		help\n		register\n		login [username]\n		logout\n		quit\n");
else
if(cmd==2)//register
{	if(logged)
		printf("[client]:Error! You must logout first!\n");
	else
	{	do{
		printf("username:");
		fflush (stdout);
		bzero(msg,100);
		bzero(parameter,100);
		read (0, parameter, 100);
		parameter[strlen(parameter)-1]='\0';
		strcat(msg,"1|");
		if(parameter[0]=='\0')
		check=1;
		else
			for(int c=0;parameter[c]!='\0';c++)
				if(parameter[c]=='|')
				{
					parameter[c+1]='\0';
					check=1;
				}
		if(check)
			printf("[client] Error! Invalid Username!\n");
		else
		{
			strcat(msg,parameter);
			if (write (sd,msg, 100) <= 0)
    			{
      				perror ("[client]Eroare la write() spre server.\n");
      				return errno;
    			}
			bzero(msg, 100);
			if (read (sd, msg, 100) < 0)
    			{
      				perror ("[client]Eroare la read() de la server.\n");
      				return errno;
    			}
 			if(msg[0]=='0')
				printf("[cleint] Username already taken!\n");
			else
			{	do{
				printf("password:");
				fflush (stdout);
				bzero(parameter,100);
                		read (0, parameter, 100);
                		parameter[strlen(parameter)-1]='\0';
				if(parameter[0]=='\0')
					printf("[client] Error! Invalid Password!\n");
				}while(parameter[0]=='\0');
	                        if (write (sd,parameter, 100) <= 0)
        	                {
                	                perror ("[client]Eroare la write() spre server.\n");
                        	        return errno;
                        	}




				if (read (sd, msg, 100) < 0)
                        	{
                                	perror ("[client]Eroare la read() de la server.\n");
                                	return errno;
                        	}
				if(msg[0]=='1')
					printf("[Client] Register Successful\n");







			}
		}
}while((!check)&&(msg[0]!='1'));
	}
}
else
if(cmd==3)//login
	if(logged)
		printf("[client]:You are already logged in! To change account logout first\n");
	else
	{
		if(!param)
		{
			printf("username:");
			fflush (stdout);
			read (0, parameter, 100);
			parameter[strlen(parameter)-1]='\0';
		}
		bzero(msg,100);
		strcat(msg,"2|");
		strcat(msg,parameter);
                if (write (sd,msg, 100) <= 0)
                {
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                }
                bzero(msg, 100);
                if (read (sd, msg, 100) < 0)
                {
                        perror ("[client]Eroare la read() de la server.\n");
                        return errno;
                }
		if(msg[0]=='0')
		printf ("[client] Invalid Username!\n");
		else
		{
		printf("password:");
                fflush (stdout);
                read (0, parameter, 100);
                parameter[strlen(parameter)-1]='\0';
                if (write (sd,parameter, 100) <= 0)
                {
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                }
                if (read (sd,msg, 100) <= 0)
                {
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                }
		if(msg[0]=='1')
		{	
			printf("[client] Login Successful!\n");
			logged=1;
		}
		else
			if(msg[0]=='2')
				printf("[client] User already online on another machine!\n");
			else
				printf("[client] Invalid password!\n");
		}
	}
else
if(cmd==4)//logout
{
	if(!logged)
		printf("[client]:You are not logged in!\n");
	else
	{
                if (write (sd, "3", 100) <= 0)
                {
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                }
		logged=0;
	}
}
else
if(cmd==5)//reply
{
	if(!param)
        {
                printf("username:");
                fflush (stdout);
                read (0, parameter, 100);
                parameter[strlen(parameter)-1]='\0';
        }
        bzero(msg,100);
        strcat(msg,"4|");
        strcat(msg,parameter);
        if (write (sd,msg, 100) <= 0)
        {
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
        }
        bzero(msg, 100);
        if (read (sd, msg, 100) < 0)
        {
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
        }
        if(msg[0]=='0')
	        printf ("[client] Invalid Username!\n");
	else
	{

        	if (read (sd, msg, 100) < 0)
        	{
                	perror ("[client]Eroare la read() de la server.\n");
                	return errno;
        	}
        	if(msg[0]=='0')
                	printf ("[client] User Offline!\n");
		else
			printf ("[client] User Online!\n");


		while(msg[0]!='\0')
		{
        		if (read (sd, msg, 100) < 0)
        		{
                		perror ("[client]Eroare la read() de la server.\n");
                		return errno;
        		}
		printf("%s",msg);
		}
	}
//testing
              /* pid=fork();
                if(pid==0)
                {printf("\nBEGIN FIU\n");
                do{
                bzero(msg,100);
                if (read (sd, msg, 100) < 0)
                {
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
                }
                if(strcmp(msg,"/leave\n"))
                printf("%s",msg);
                }while(strcmp(msg,"/leave"));
                printf("\nEND FIU\n"); exit(0);
                }*/
//testing
	do
	{
		bzero(text,1024);
		printf("Message:");
		fflush(stdout);
		read(0,text,1024);
        	if (write (sd,text, 1024) <= 0)
        	{
                	perror ("[client]Eroare la write() spre server.\n");
                	return errno;
        	}
	}while(strcmp(text,"/leave\n"));

}
if((logged)&&(cmd))
{printf("[client] checking messages\n");
        if (write (sd, "5", 100) <= 0)
        {
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
	}

	if (read (sd,msg, 100) <= 0)
        {
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
        }
	if(msg[0]=='0')
		printf("[client]You have no new messages!\n");
	else
	{
		printf("[client]You have messages from:\n");
        	while(msg[0]!='\0')
        	{
                	if (read (sd, msg, 100) < 0)
                	{
                        	perror ("[client]Eroare la read() de la server.\n");
                        	return errno;
                	}
                	printf("%s",msg);
        	}
	}
}
  /* trimiterea mesajului la server */
 /* if (write (sd, msg, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }*/

  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
/*  if (read (sd, msg, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }*/
  /* afisam mesajul primit */
 // printf ("[client]Mesajul primit este: %s\n", msg);
}while(cmd);
  /* inchidem conexiunea, am terminat */
if (write (sd, "q", 100) <= 0)
{
	perror ("[client]Eroare la write() spre server.\n");
        return errno;
}

  close (sd);
}

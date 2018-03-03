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

/* portul folosit */
#define PORT 2024

/* codul de eroare returnat de anumite apeluri */
extern int errno;

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  char msg[100];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
  int sd;			//descriptorul de socket 
  int pid;
  char name1[100],name2[100];
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
      int client1,client2;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client1 = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client1 < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}

      /* s-a realizat conexiunea, se astepta mesajul */
      bzero (msg, 100);
      printf ("[server]Asteptam mesajul...\n");
      fflush (stdout);
      
	bzero(name1,100);
      if (read (client1, name1, 100) <= 0)
        {
          perror ("[server]Eroare la read() de la client.\n");
          close (client1);       /* inchidem conexiunea cu clientul */
          continue;             /* continuam sa ascultam */
        }
//printf("Nume primit %s%s",name1,name1);
      if (write (client1, "0", 100) <= 0)
        {
          perror ("[server]Eroare la write() catre client.\n");
          continue;             /* continuam sa ascultam */
        }

      client2 = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client2 < 0)
        {
          perror ("[server]Eroare la accept().\n");
          continue;
        }
        bzero(name2,100);
      if (read (client2, name2, 100) <= 0)
        {
          perror ("[server]Eroare la read() de la client.\n");
          close (client2);       /* inchidem conexiunea cu clientul */
          continue;             /* continuam sa ascultam */
        }

        if (write (client1, "1", 100) <= 0)
        {
          perror ("[server]Eroare la write() catre client.\n");
          continue;             /* continuam sa ascultam */
        }
        if (write (client2, "1", 100) <= 0)
        {
          perror ("[server]Eroare la write() catre client.\n");
          continue;             /* continuam sa ascultam */
        }
	pid=fork();
	if(!pid)
	{
		int pid2=fork();
		if(!pid2)
		{	
			while(strcmp(msg,"/leave\n"))
			{
				if (read (client1, msg, 100) <= 0)
        			{
          				perror ("[server]Eroare la read() de la client.\n");
          				close (client1);       /* inchidem conexiunea cu clientul */
					bzero(msg,100);
					strcpy(msg,"leave\n");
          				continue;             /* continuam sa ascultam */
        			}
				bzero(msgrasp,100);
				strcpy(msgrasp,name1);
				strcat(msgrasp,":");
				strcat(msgrasp,msg);
				if(strcmp(msg,"/leave\n"))
				{
			        if (write (client2, msgrasp, strlen(msgrasp)) <= 0)
        			{
          				perror ("[server]Eroare la write() catre client.\n");
          				continue;             /* continuam sa ascultam */
        			}
				}
				else
				{
                                if (write (client2, "\0", 100) <= 0)
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }
                                if (write (client1, "\0", 100) <= 0)
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }

				}

			}
		close(client2);
		exit(0);
		}
                        while(strcmp(msg,"/leave\n"))
                        {
                                if (read (client2, msg, 100) <= 0)
                                {
                                        perror ("[server]Eroare la read() de la client.\n");
                                        close (client2);       /* inchidem conexiunea cu clientul */
                                        bzero(msg,100);
                                        strcpy(msg,"leave\n");
                                        continue;             /* continuam sa ascultam */
                                }
                                bzero(msgrasp,100);
                                strcpy(msgrasp,name2);
                                strcat(msgrasp,":");
                                strcat(msgrasp,msg);
				if(strcmp(msg,"/leave\n"))
				{
                                if (write (client1, msgrasp, strlen(msgrasp)) <= 0)
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }
				}
				else
				{
                               if (write (client1, "\0", 100) <= 0)
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }
                                if (write (client2, "\0", 100) <= 0)
                                {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;             /* continuam sa ascultam */
                                }

				}

			}

	close(client1);
	exit(0);
	}

      close (client1);
      close(client2);

    }				/* while */

}				/* main */


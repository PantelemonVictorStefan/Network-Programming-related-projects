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

int main ()
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis
  int pid;
  /* exista toate argumentele in linia de comanda? */
  if (0)
    {
      return -1;
    }

  /* stabilim portul */
  port = 2024;

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
  server.sin_addr.s_addr = inet_addr("79.112.3.16");
//server.sin_addr.s_addr = inet_addr("0.0.0.0");
  /* portul de conectare */
  server.sin_port = htons (port);
  
 /* citirea mesajului */
  bzero (msg, 100);
  printf ("Enter your name: ");
  fflush (stdout);
  read (0, msg, 100);
  msg[strlen(msg)-1]='\0';

  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }


  if (write (sd, msg, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

  do{
  	if (read (sd, msg, 100) < 0)
    	{
      		perror ("[client]Eroare la read() de la server.\n");
      		return errno;
    	}
  	if(msg[0]=='0')
		printf("[client]: Waiting for someone to connect.\n");
  }while(msg[0]=='0');
  printf("[client]: You are connected! Write /leave to quit.\n");
  /* trimiterea mesajului la server */
  pid=fork();
  if(!pid)
  {
  	while(strcmp(msg,"\0"))
  	{
  		bzero(msg,100);
  		if (read (sd, msg, 100) < 0)
    		{
      			perror ("[client]Eroare la read() de la server.\n");
     	 		return errno;
    		}
  		printf("%s",msg);
  	}
	printf("[client] Conversation has stopped!");
	fflush(stdout);
  	exit(0);
  }
 while(strcmp(msg,"/leave\n"))
 {
  bzero (msg, 100);
  read (0, msg, 100);

  if (write (sd, msg, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
 }
  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */


  /* inchidem conexiunea, am terminat */
  close (sd);
}

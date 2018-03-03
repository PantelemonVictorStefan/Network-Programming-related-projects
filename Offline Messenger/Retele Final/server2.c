/* servTCPCSel.c - Exemplu de server TCP concurent 
   
   Asteapta un "nume" de la clienti multipli si intoarce clientilor sirul
   "Hello nume" corespunzator; multiplexarea intrarilor se realizeaza cu select().
   
   Cod sursa preluat din [Retele de Calculatoare,S.Buraga & G.Ciobanu, 2003] si modificat de 
   Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
   
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


/* portul folosit */

#define PORT 2024

extern int errno;		/* eroarea returnata de unele apeluri */
int search(char*);
int checkpass(char*);
int add(char*);

/* functie de convertire a adresei IP a clientului in sir de caractere */
char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

/* programul */
int
main ()
{
  struct sockaddr_in server;	/* structurile pentru server si clienti */
  struct sockaddr_in from;
  fd_set readfds;		/* multimea descriptorilor de citire */
  fd_set actfds;		/* multimea descriptorilor activi */
  struct timeval tv;		/* structura de timp pentru select() */
  int sd, client;		/* descriptori de socket */
  int optval=1; 			/* optiune folosita pentru setsockopt()*/ 
  int fd;			/* descriptor folosit pentru 
				   parcurgerea listelor de descriptori */
  int nfds;			/* numarul maxim de descriptori */
  int len;			/* lungimea structurii sockaddr_in */

  char msg[100];                //mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
  int cmd,i,j,logged=0;
  char comanda[100],aux[100];


  /* creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }

  /*setam pentru socket optiunea SO_REUSEADDR */ 
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }
  
  /* completam multimea de descriptori de citire */
  FD_ZERO (&actfds);		/* initial, multimea este vida */
  FD_SET (sd, &actfds);		/* includem in multime socketul creat */

  tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = sd;

  printf ("[server] Asteptam la portul %d...\n", PORT);
  fflush (stdout);
        
  /* servim in mod concurent clientii... */
  while (1)
    {
      /* ajustam multimea descriptorilor activi (efectiv utilizati) */
      bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

      /* apelul select() */
      if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
	{
	  perror ("[server] Eroare la select().\n");
	  return errno;
	}
      /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
      if (FD_ISSET (sd, &readfds))
	{
	  /* pregatirea structurii client */
	  len = sizeof (from);
	  bzero (&from, sizeof (from));

	  /* a venit un client, acceptam conexiunea */
	  client = accept (sd, (struct sockaddr *) &from, &len);

	  /* eroare la acceptarea conexiunii de la un client */
	  if (client < 0)
	    {
	      perror ("[server] Eroare la accept().\n");
	      continue;
	    }

          if (nfds < client) /* ajusteaza valoarea maximului */
            nfds = client;
            
	  /* includem in lista de descriptori activi si acest socket */
	  FD_SET (client, &actfds);

	  printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));
	  fflush (stdout);
	}
      /* vedem daca e pregatit vreun socket client pentru a trimite raspunsul */
      for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
	{
	  /* este un socket de citire pregatit? */
	  if (fd != sd && FD_ISSET (fd, &readfds))
	    {	
	      
		








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
	
      printf ("[server]Mesajul a fost receptionat...%s\n", msg);





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
				{printf("\n%s%d",msg,checkpass(msg));fflush(stdout);
					if(checkpass(msg))
					{
						if (write (client,"1", 100) <= 0)//continue
                		                {
                                	        	perror ("[server]Eroare la write() catre client.\n");
                                        		continue;             /* continuam sa ascultam */
                                		}
					logged=1;
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
	logged=0;
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












		  printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
		  fflush (stdout);
		  close (fd);		/* inchidem conexiunea cu clientul */
		  FD_CLR (fd, &actfds);/* scoatem si din multime */
		  
		
	    }
	}			/* for */
    }				/* while */
}				/* main */

/* realizeaza primirea si retrimiterea unui mesaj unui client */

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

close(fd);
return(strcmp(entry,account)==0);


}

int add(char account[100])
{
char ch;
int fd= open("accounts",O_RDWR|O_APPEND|O_CREAT|S_IRUSR);
close(fd);
return write(fd,account,strlen(account));
//while(read(fd,&ch,1));


}

int checkpass(char pass[100])
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
        end=read(fd,&ch,1);
        end=read(fd,&ch,1);
        while((ch!='\n')&&(end))
        {
        entry[i]=ch;
        i++;
        end=read(fd,&ch,1);
        }

}while((end)&&(strcmp(entry,pass)));

close(fd);
return(strcmp(entry,pass)==0);

}



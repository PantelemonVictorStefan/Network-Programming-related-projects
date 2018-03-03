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
                read(fd,&ch,1);


}while((end)&&(strcmp(entry,account)));

return(strcmp(entry,account)==0);
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
	return checkstatus(account,-1);
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
printf("%s\n",entry);

}while((end)&&(strcmp(entry,name)));
close(fd);
return(strcmp(entry,name)==0);
}


int main()
{
//char file[100];
//bzero(file,100);
//strcat(file,"fis");
//int fd= open(file,O_RDWR|S_IRWXO);
//int fd= open(file,O_CREAT|O_RDWR,0600);

if(notify("fis","Hetor"))
printf("1");
}



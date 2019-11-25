/* server.c
 * compile: gcc server.c -o server
 * build : ./server 9190
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

void error_handling(char* );
int exitCheck(char*,char*,int);
struct Player{

	int client_s; //socket number of the client
};

struct Player players[3]; //maximum players: 3 people
char *escapechar1 = "q\n";
char *escapechar2 = "Q\n";
int main(int argc, char* argv[])
{
	int serv_sock,clnt_sock;
	char *startgame = "game start!\n";
	char message[BUF_SIZE];
	int str_len,i,j;
	fd_set read_fds;
	int num_chat = 0;
	struct sockaddr_in serv_adr,clnt_adr;
	//socket address of server and client 
	int nfds;
 	int n;
	
	socklen_t clnt_adr_sz;

	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	
	puts("--------------------initialize the server--------------------------");
	//stage1. socket	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	//stage 2. bind
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	//stage3. listen
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");
	
	clnt_adr_sz = sizeof(clnt_adr);
	
	nfds = serv_sock+1;
	//maximum socket number +1

	FD_ZERO(&read_fds);

	
	while(1)
	{
		if(num_chat>=3)
			break;
      	        //stage3.5 select       
		if((num_chat-1)>=0)
			nfds = players[num_chat-1].client_s+1;

		FD_SET(serv_sock,&read_fds);

		for(i=0;i<num_chat;i++)
			FD_SET(players[i].client_s,&read_fds);
		if(select(nfds,&read_fds,(fd_set*)0,(fd_set*)0,(struct timeval*)0)<0)
			error_handling("select() error");
	
       		 //stage4.accept(for multiple clients)
        	if(FD_ISSET(serv_sock,&read_fds))
       		{
                clnt_adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

                if(clnt_sock !=-1)
               		 {	
                        //add for the new client(player)
                        players[num_chat++].client_s = clnt_sock;
                        printf("%dth player connected.\n",num_chat);
                	}
        	}


	}


	memset(message,0,sizeof(message));
	if(num_chat==3)
		printf("start bingo game!\n");
	

	//start setting the game***********************************
	//send the messages!!
	for(j=0;j<num_chat;j++)
	{
		
		send(players[j].client_s,startgame,strlen(startgame),0);
	}
//	send(players[num_chat].client_s,startgame,strlen(startgame),0);
//	send(players[num_chat-1].client_s,startgame,strlen(startgame),0);
	
while(1){;}				
	  
}

void error_handling(char* message)
{
	
fputs(message,stderr);
fputc('\n',stderr);
exit(1);
}

int exitCheck(char *rline,char* escape, int len)
{
	int i,max;
	char *tmp;

	max = strlen(rline);
	tmp = rline;
	for(i=0;i<max;i++)
	{
	if(*tmp==escape[0])
		{
		if(strncmp(tmp,escape,len)==0)
			return 1;
		}
	else
		tmp++;
	}
	return -1;
		
}


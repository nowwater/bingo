/*
 * compile: gcc client.c -o client
 * build: ./client 127.0.0.1 9190 Joonhey
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/time.h>

#define BUF_SIZE 1024

int sock;//socket number of this client
typedef struct {

	char n[20]; //name of the player
	int len; //name size	
}name;

void error_handling(char* message);

int main(int argc, char *argv[])
{

	char sendmessage[BUF_SIZE+1];
	char recvmessage[BUF_SIZE];
	char message[BUF_SIZE];
	char* escapechar1 = "q\n";
	char* escapechar2 = "Q\n";
	char player_name[30] = "name:";
	//name of the client including the token "name:"
	//I will send this to server as name structure
	name playername;
	int str_len;
	struct sockaddr_in serv_adr;
	fd_set read_fds;
	int nfds;
	int size;
	int cnt=0;

	if(argc<4)
	{
		printf("Usage : %s <IP> <port> <name>\n",argv[0]);
		exit(1);
	}
	
	sprintf(player_name,"%s%s",player_name,argv[3]);
	//player_name = "name:Joonhey
	sprintf(playername.n,"[%s]",argv[3]);
	//playername.n = "[Joonhey]"
	playername.len = strlen(playername.n);

	//stage1. socket
	sock = socket(PF_INET, SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	//stage 2. connect
	if(connect(sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	nfds = sock+1;
	FD_ZERO(&read_fds);

	//send & receive messages with server
	while(1)
	{
		
		FD_SET(0,&read_fds);
		FD_SET(sock,&read_fds);
		
		//stage3. select
		if(select(nfds,&read_fds,(fd_set *)0,(fd_set *)0,(struct timeval *)0)<0)
		{
			error_handling("select() error!");
			return -1;
		}

		memset(recvmessage,0,strlen(recvmessage));
		memset(sendmessage,0,strlen(sendmessage));

		//recv from the server
		if(FD_ISSET(sock,&read_fds))
		{
			if((size = recv(sock,recvmessage,BUF_SIZE,0))>0)
			{
				recvmessage[size] = '\0';
				if(strncmp(recvmessage,"game start!\n",strlen(recvmessage))==0)
					puts("BINGO PAN PLEASE~");
				else
					printf("%s \n",recvmessage);
				
			}
			else
			{
				error_handling("recieve error!");
			}
		}
		
		if(FD_ISSET(0,&read_fds))
		{










			if(fgets(sendmessage,BUF_SIZE,stdin)!=NULL)
			{
				size = strlen(sendmessage);
				sprintf(message,"%s %s",playername.n,sendmessage);
				if(send(sock,message,size+playername.len,0)!=(size+playername.len))
					error_handling("Written error on socket.");
				if(size == 2 && strncmp(sendmessage, escapechar1,2)==0)
				{
					printf("Good bye.\n");
					exit(1);
				}
				if(size==2 && strncmp(sendmessage, escapechar2,2)==0)
				{
					printf("Good bye.\n");
					exit(1);
				}
			}
		
		}

	} 
	close(sock);	

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

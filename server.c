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

char* escapechar1 = "q\n";
char* escapechar2 = "Q\n";

void error_handling(char*);
int exitCheck(char*, char*, int);
int str_int(char* num);

int main(int argc, char* argv[])
{
	//socket address of server and client 
	int serv_sock;
	int clnt_sock;
	int str_len, i, j;
	int num = 0;
	int client_s[5];
	struct sockaddr_in serv_adr, clnt_adr;

	int nfds;
	int n;
	int fdNum;
	int cnt_done = 0;
	int turn = 0;
	struct timeval timeout;
	char num_erase[50], num_send[50];

	char* startgame = "game start!\n";
	char* startcalling = "call the number!\n";
	char* turnmessage = "TURN\n";
	char message[BUF_SIZE];
	socklen_t clnt_adr_sz;
	fd_set read_fds, cpyReads;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	puts("--------------------------Initialize the server--------------------------");
	//stage1. socket	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	//stage 2. bind
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	//stage3. listen
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	nfds = serv_sock + 1;
	//maximum socket number +1

	FD_ZERO(&read_fds);
	FD_SET(serv_sock, &read_fds);

	while (1)
	{
		cpyReads = read_fds;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		//stage3.5 select
		if ((fdNum = select(nfds, &cpyReads, 0, 0, NULL)) == -1)
			break;
		if (fdNum == 0)
			continue;

		while (num < 3)
		{
			if (FD_ISSET(serv_sock, &cpyReads))
			{
				//connection request!
				clnt_adr_sz = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);
				FD_SET(clnt_sock, &read_fds);

				//new client socket
				if (nfds < clnt_sock)
					nfds = clnt_sock;

				client_s[num++] = clnt_sock;
				//client descriptor을 배열에 저장

				printf("%dth player connected.\n", num);

			}
		}

		printf("start bingo game!\n");

		for (j = 0; j < num; j++)
		{
			write(client_s[j], startgame, strlen(startgame));
			//write로 바꾸기&thread 사용하기
		}
		memset(message, 0, sizeof(message));

		while (1)
		{
			for (j = 0; j < num; j++)//receive messages from client
			{
				if (FD_ISSET(client_s[j], &read_fds))
				{
					if ((n = recv(client_s[j], message, BUF_SIZE, 0)) > 0)
					{
						message[n] = '\0';
						printf("message received from %dth player:%s\n", j + 1, message);
						if (strcmp(message, "DONE!\n") == 0)
							cnt_done++;
						if (cnt_done >= 3)
							break;
					}
				}
			}
			if (cnt_done >= 3) break;
		}


		//all users ended filling their own boards.

		printf("all done\n");

		/************************************************************************/

		for (j = 0; j < num; j++)
			//tell the clients that all players are ready
		{
			send(client_s[j], startcalling, strlen(startcalling), 0);
			//sleep(3);
		}


		while (1)
		{
			if (turn > 2)
				turn = 0; //turn: 0,1,2,0,1,2...

			if ((n = write(client_s[turn], turnmessage, strlen(turnmessage))) > 0)
			{
				printf("%dth player's turn-sent the signal.\n", turn + 1);
			}
			
			if ((n = read(client_s[turn], num_erase, strlen(num_erase))) == -1)
				error_handling("number to erase error");

			
			for (j = 0; j < num; j++)
			{
				if ((n = write(client_s[j], num_erase, strlen(num_erase))) > 0)
				{
					printf("player %d's number was sent.\n", turn + 1);
				}
			}

			turn++;
		}

	}//while(1)



}//main
//

int str_int(char* num)
{
	int k = strlen(num);
	int digit = 1;
	int number = 0;

	while (0 < k)
	{
		k--;
		number += ((num[k] - '0') * digit);
		digit *= 10;
	}

	return number;
}


void error_handling(char* message)
{

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int exitCheck(char* rline, char* escape, int len)
{
	int i, max;
	char* tmp;

	max = strlen(rline);
	tmp = rline;
	for (i = 0; i < max; i++)
	{
		if (*tmp == escape[0])
		{
			if (strncmp(tmp, escape, len) == 0)
				return 1;
		}
		else
			tmp++;
	}
	return -1;

}


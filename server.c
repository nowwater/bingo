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
#include <pthread.h>

#define BUF_SIZE 1024

char* escapechar1 = "q\n";
char* escapechar2 = "Q\n";

void error_handling(char*);
int exitCheck(char*, char*, int);
void* recv_bingo_message(void*);
void* recv_num_to_erase();

int str_int(char* num);
int bingo_flag = 1;
int winner = -1;
int client_s[5];
int client_s2[5];

char* startgame = "game start!\n";
char* start_signal = "call the number!\n";
char* turnmessage = "TURN\n";
char *winner_msg = "WINNER\n";
char *loser_msg = "LOSER\n";

char message[BUF_SIZE];

int main(int argc, char* argv[])
{
	//socket address of server and client 
	int serv_sock;
	int clnt_sock, clnt_bingo_sock;
	int str_len, i, j;
	int num = 0;

	struct sockaddr_in serv_adr, clnt_adr;
	int nfds;
	int n, cnt = 0;
	int fdNum;
	int cnt_done = 0;
	int turn = 0;
	struct timeval timeout;
	pthread_t number_thread, bingo_thread[3];

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

	cpyReads = read_fds;
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;

	//stage3.5 select
	/*if ((fdNum = select(nfds, &cpyReads, 0, 0, NULL)) == -1)
	break;
	if (fdNum == 0)
	continue;
	*/
	while (num < 3)
	{
		if (FD_ISSET(serv_sock, &cpyReads))
		{
			//connection request!
			clnt_adr_sz = sizeof(clnt_adr);
			clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);
			clnt_bingo_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);
			FD_SET(clnt_sock, &read_fds);
			FD_SET(clnt_bingo_sock, &read_fds);
			//new client socket
			if (nfds < clnt_sock)
				nfds = clnt_sock;

			client_s[num] = clnt_sock;
			client_s2[num++] = clnt_bingo_sock;
			//client descriptor을 배열에 저장

			printf("%dth player connected.\n", num);

		}
	}

	printf("start bingo game!\n");

	for (j = 0; j < num; j++)
	{
		send(client_s[j], startgame, strlen(startgame), 0);
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
		n = send(client_s[j], start_signal, strlen(start_signal), 0);
		printf("j: %d n : %d\n", j, n);
	}

	/* Start to input the number to erase */
	pthread_create(&number_thread, NULL, recv_num_to_erase, NULL);
	for (i = 0; i < 3; i++)
		pthread_create(&bingo_thread[i], NULL, recv_bingo_message, (void*)&client_s2[i]);

	printf("Create Done in a given time\n");
	i = 0;
	while (1)
	{
		if (bingo_flag == 0)
		{
			printf("client_s2[%d] : %d\n", i, client_s2[i]);
			if (i == (winner / 2) - 2)
			{
				if ((n = write(client_s2[i], winner_msg, strlen(winner_msg))) == -1)
					error_handling("write error()");
				printf("%d WINNER\n", client_s2[i]);
			}
			else
			{
				if ((n = write(client_s2[i], loser_msg, strlen(loser_msg))) == -1)
					error_handling("write error()");
				printf("%d LOSER\n", client_s2[i]);
			}
			i++;
			cnt++;
			sleep(0.5);
		}
		if (cnt == 3)
			break;
	}
	pthread_join(number_thread, NULL);
	printf("num_thread join Done in a given time\n");

	for (i = 0; i < 3; i++)
		pthread_join(bingo_thread[i], NULL);
	printf("bingo thread join Done in a given time\n");
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

void* recv_bingo_message(void* client)
{
	int n;
	int *clnt_num = client;
	char temp_msg[10];
	while (((n = read(*clnt_num, temp_msg, BUF_SIZE)) != -1))
	{
		temp_msg[n] = '\0';
		printf("%d said %s!\n", *clnt_num, temp_msg);
		if (bingo_flag)
		{
			winner = *clnt_num;
			bingo_flag = 0;
			printf("bingo flag changed : %d\n", bingo_flag);
			printf("Winner : %d\n", winner);
			break;
		}
	}
	if (n < 0)
		error_handling("read errer()");

	return NULL;
}

void* recv_num_to_erase()
{
	int turn = 0, n;
	char temp_msg[10];
	while (1)
	{
		if (turn > 2)
			turn = 0; //turn: 0,1,2,0,1,2...
					  /*if (bingo_flag == 0) //it's time when there's already winner.
					  {
					  for (int i = 0; i < 3; i++)
					  {
					  if (i == (winner/2)-2) continue;
					  if ((n = write(client_s[i], loser_msg, strlen(loser_msg))) == -1)
					  error_handling("write error()");
					  }
					  }*/
		if ((n = write(client_s[turn], turnmessage, strlen(turnmessage))) > 0)
		{
			//tell the turn to that user
			printf("%dth player's turn-sent the signal.\n", turn + 1);
		}
		//waiting->while?

		// socket + thread
		if ((n = recv(client_s[turn], temp_msg, BUF_SIZE, 0)) > 0)
		{
			temp_msg[n] = '\0';
			//got someting(num from the turn user)?
			printf("number received from %dth player: %s\n", turn + 1, temp_msg);

			//send it to all client..
			for (int j = 0; j < 3; j++)
			{
				if (j == turn) continue;
				if ((n = write(client_s[j], temp_msg, strlen(temp_msg))) > 0)
					printf("sent %s to %dth client completed\n", temp_msg, j);
			}
		}
		turn++;
	}
	return NULL;
}
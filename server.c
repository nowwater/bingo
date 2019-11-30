/* server.c
* compile: gcc server.c -o server -lpthread
* build : ./server 9190
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 1024

void error_handling(char*);
void* recv_bingo_message(void*);
void* recv_num_to_erase();

int str_int(char* num);
int bingo_flag = 1;
int winner = -1;
int client_s[3];
int client_s2[3];

char* startgame_msg = "game start!\n";
char* call_msg = "call the number!\n";
char* turn_msg = "TURN\n";
char* winner_msg = "WINNER\n";
char* loser_msg = "LOSER\n";

char message[BUF_SIZE];

int main(int argc, char* argv[])
{
	//socket address of server and client 
	int serv_sock;
	int clnt_sock, clnt_bingo_sock;
	int str_len, i, j;
	int num = 0;

	struct sockaddr_in serv_adr, clnt_adr;
	int n, cnt = 0;
	int cnt_done = 0;
	int turn = 0;
	pthread_t number_thread, bingo_thread[3];
	socklen_t clnt_adr_sz;

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
	if (listen(serv_sock, 3) == -1)
		error_handling("listen() error");

	//stage4. accept
	while (num < 3)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);
		clnt_bingo_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);

		client_s[num] = clnt_sock;
		client_s2[num++] = clnt_bingo_sock;
		//client descriptor을 배열에 저장

		printf("player %d connected.\n", num);
	}

	printf("Start bingo game!\n");

	for (j = 0; j < num; j++)
	{
		send(client_s[j], startgame_msg, strlen(startgame_msg), 0);
		//write로 바꾸기&thread 사용하기
	}
	memset(message, 0, sizeof(message));

	while (1)
	{
		for (j = 0; j < num; j++)//receive messages from client
		{

			if ((n = read(client_s[j], message, BUF_SIZE)) == -1)
				error_handling("everybody done error");

			message[n] = '\0';
			printf("message received from player %d : %s", j + 1, message);
			if (!strcmp(message, "DONE!\n"))
				cnt_done++;
			if (cnt_done >= 3)
				break;
		}
		if (cnt_done >= 3) break;
	}
	//all users ended filling their own boards.

	printf("All done!\n");

	//tell the clients that all players are ready
	for (j = 0; j < num; j++)
	{
		if (n = write(client_s[j], call_msg, strlen(call_msg)) == -1)
			error_handling("ready error");
	}

	/* Start to input the number to erase */
	pthread_create(&number_thread, NULL, recv_num_to_erase, NULL);
	for (i = 0; i < 3; i++)
		pthread_create(&bingo_thread[i], NULL, recv_bingo_message, (void*)& client_s2[i]);

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
		}
		if (cnt == 3)
			break;
	}
	pthread_join(number_thread, NULL);
	printf("number_thread_join Done\n");

	for (i = 0; i < 3; i++)
		pthread_join(bingo_thread[i], NULL);
	printf("bingo_thread_join Done\n");

	close(clnt_sock);
	close(clnt_bingo_sock);
}

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
	int* clnt_num = client;
	char temp_msg[10];
	while ((n = read(*clnt_num, temp_msg, BUF_SIZE)) != -1)
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
		error_handling("read error()");

	return NULL;
}

void* recv_num_to_erase()
{
	int turn = 0, n;
	char temp_msg[10];
	while (1)
	{
		if (turn > 2)
			turn = 0;

		if ((n = write(client_s[turn], turn_msg, strlen(turn_msg))) > 0)
		{
			//tell the turn to that user
			printf("player%d's turn sent the signal.\n", turn + 1);
		}

		// socket + thread
		if ((n = recv(client_s[turn], temp_msg, BUF_SIZE, 0)) == -1)
			error_handling("recv_num_error()");

		temp_msg[n] = '\0';
		//got someting(num from the turn user)?
		printf("number received from %dth player: %s\n", turn + 1, temp_msg);

		//send it to all client..
		for (int j = 0; j < 3; j++)
		{
			if (j == turn) continue;
			if ((n = write(client_s[j], temp_msg, strlen(temp_msg))) > 0)
				printf("sent %s to %d client completed\n", temp_msg, j+1);
		}
		turn++;
	}
	return NULL;
}
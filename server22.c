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

struct Player{

	int client_s; 
	//socket number of the client
};

//manage players' bingo board
typedef struct {
	int client_s;
	int board[5][5];
}Player_board;
Player_board playerboards[3];

//maximum players: 3people
struct Player players[3];


char *escapechar1 = "q\n";
char *escapechar2 = "Q\n";

void error_handling(char* );
int exitCheck(char*,char*,int);
int str_int(char* num);

//managing client's bingo board
void board_managing(char*, int);

int main(int argc, char* argv[])
{
	int serv_sock,clnt_sock;
	char *startgame = "game start!\n";
	char *startcalling = "call the number!\n";
	char message[BUF_SIZE];
	int str_len,i,j;
	fd_set read_fds,cpyReads;
	int num_chat = 0;
	struct sockaddr_in serv_adr,clnt_adr;
	//socket address of server and client 
	int nfds;
 	int n;
	struct timeval timeout;
	int fdNum;
	int cnt_done = 0;
	int turn =0;
	int k;
	char* winmessage = "WIN!\n";
	char* losemessage = "LOSE\n";
	char* turnmessage = "TURN\n";
	char* this_turn_number = "erase this number-";
	char* ptr;

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
	
	
	nfds = serv_sock+1;
	//maximum socket number +1

	FD_ZERO(&read_fds);
	FD_SET(serv_sock,&read_fds);
	
	while(1)
	{
		
		cpyReads = read_fds;
		timeout.tv_sec = 5;
		timeout.tv_usec - 5000;

      	        //stage3.5 select   
	    	if((fdNum = select(nfds,&cpyReads,0,0,NULL))==-1)
			break;
		if(fdNum==0)
			continue;
		
		while(1)
		{ 
			if(FD_ISSET(serv_sock,&cpyReads))
			{
				
				
			           //connection request!
				   clnt_adr_sz = sizeof(clnt_adr);
			           clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
			  	   FD_SET(clnt_sock,&read_fds);
				   if(nfds<clnt_sock)
					nfds= clnt_sock;
				    players[num_chat++].client_s = clnt_sock;
				    printf("%dth player connected.\n",num_chat);
				    if(num_chat>=3) 
					break;
				
			}//fd_isset
		   }//while(1)
		   
		  if(num_chat==3)
	          {
		    printf("start bingo game!\n");
        	 
			for(j=0;j<num_chat;j++)
       			 {	
               		 send(players[j].client_s,startgame,strlen(startgame),0);
       			 }
		    memset(message,0,sizeof(message));

		    while(1)
		    {
			for(j=0;j<num_chat;j++)
			//receive messages from client
			{
				if(FD_ISSET(players[j].client_s,&read_fds))
				{
					if((n = recv(players[j].client_s,message,BUF_SIZE,0))>0)
					{
					  message[n] = '\0';
					  printf("message received from %dth player:%s\n",j,message);
					  if(strcmp(message,"DONE!\n")==0)
						cnt_done++;
					   if(cnt_done>=3)
						break;	
					}
				}
			}
			if(cnt_done>=3) break;
		   }

		    
 		  //all users ended filling their own boards.
		 if(cnt_done==3)
		   printf("all done\n");	
		}//if(num_chat==3)
		
	for(j=0;j<num_chat;j++)
	//tell the clients that all players are ready
	{
	 send(players[j].client_s,startcalling,strlen(startcalling),0);
	}
	/*what server has to do - 
	0. got the bingo message - > send lose/win
	1. turn :got the message - number
	2. got the bingo message ->send lose/win
	*/
	if(cnt_done==3)
	{
	winmessage[strlen(winmessage)] = '\0';
	losemessage[strlen(losemessage)] = '\0';
	turnmessage[strlen(turnmessage)]= '\0';
	i = 0;
	while(1)
	{
	 //any bingos?
	 for(j=0;j<num_chat;j++)
	 {
		if(FD_ISSET(players[j].client_s,&read_fds))
		{
			if((n = recv(players[j].client_s,message,BUF_SIZE,0))>0)
			{
			 message[n] = '\0';
			 if(strcmp(message,"BINGO\n")==0)
			 { //jth player is the winner
				for(k=0;k<num_chat;k++)
				{
				 if(k==j)
					send(players[k].client_s,winmessage,strlen(winmessage),0);
				 else
					send(players[k].client_s,losemessage,strlen(losemessage),0);
				 //closesocket()...
				}
			  sleep(5);
			  return 0;	
			 }
			// sleep(5);
			 
			}
		}
	 }
	 //no bingo->let's got the number!
	  if(turn>2)
		turn==1;
	  printf("%d's turn!\n",turn);
	  send(players[turn].client_s,turnmessage,strlen(turnmessage),0);
	  if(FD_ISSET(players[turn].client_s,&read_fds))
	  {
	  if((n = recv(players[turn].client_s,message,BUF_SIZE,0))>0)
		{
		 message[n] = '\0';
		 printf("what %dth player sent: %s\n",turn,message);
		   for(k=0;k<num_chat;k++)
		   {
			//except me
			if(k!=turn)
			{
			if((n= send(players[k].client_s,message,strlen(message),0))>0)
				printf("sent to %dth player completed.\n",k);
		   	}
		   }

		}
	   } 
	// recv number, send to all
	  turn++;
	  
	    
	 }//while(1)
	}//cnt_done==3

   }//while(1)

		

}//main

//manage the ith client's bingo board based on 'i'th client's message.
void board_managing(char* message, int i)
{
puts("managing start");
	char* ptr;

	char* row;
        char* col;
        char* string_num;

        int input_row,input_col,input_num;

	ptr = strtok(message," ");

        //row
        strcpy(row,ptr);
        
	//col
        ptr = strtok(NULL," ");
        strcpy(col,ptr);
                        
	//num
        ptr = strtok(NULL," ");
      	strcpy(string_num,ptr);

       	//string->int
        input_row = str_int(row);
        input_col = str_int(col);
       	input_num = str_int(string_num);

        playerboards[i].client_s = players[i].client_s;
	playerboards[i].board[input_row][input_col] = input_num;

	printf("%dth user's managing board updated: playerboards[%d].board[%d][%d] = %d\n, socket num: %d\n",i,i,input_row,input_col,playerboards[i].board[input_row][input_col],playerboards[i].client_s);

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


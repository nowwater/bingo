/*
 * compile: gcc client.c -o client
 * build: ./client 127.0.0.1 9190
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
#include <curses.h>
#include <termios.h>

#define BUF_SIZE 1024
#define MAX 5

int titlename[5][31] =
{
		{ 1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0 },
		{ 1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1 },
		{ 1,1,1,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,0,0,1 },
		{ 1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0 }
};
int board_picture[22][36] =
{
		{ 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
		{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
};


//socket number of this client
int sock;

//contents of bingo board of this client
int board_number[5][5];

//check if it has already written.
int check_double_number[51];

//count the number of filled block
int filled_block = 0;

fd_set read_fds;

//wait screen
void waitscreen();

//function for title screen
void title();
void select_menu(int y, int x, int menu);

//function for play, explanation, quit
void play(int y, int x);
void explain();
void end_game();
void display_mainboard(int startX, int startY);
void display_message(int startX, int startY, int letterX, int letterY);
void display_board(int startX, int startY, int row, int col, char* num);
void erase_notice(int letterX, int letterY);
void input_number(int startX, int startY, int letterX, int letterY);
void ready_to_fight(int letterX, int letterY);
int str_int(char* num);
int check(int int_num);
void display_message2(int startX, int startY, int letterX, int letterY);
void tempscreen();
void tempscreen2();
void ready_to_fight2(int letterX, int letterY);
void input_number2(int startX,int startY, int letterX,int letterY);
int check_and_erase(int erase_num);
//error handling - exit
void error_handling(char* message);

//needed variable
char sendmessage[BUF_SIZE + 1];
char recvmessage[BUF_SIZE];
char message[BUF_SIZE];
int str_len;
struct sockaddr_in serv_adr;
int nfds;
int size;
int cnt = 0;
int PORTNUM;
char IP[20];
char* startgame = "game start!\n";
char* startcalling = "call the number!\n";
char* turnmessage = "TURN\n";

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>  \n", argv[0]);
		exit(1);
	}

	//IP = "\0";
	strcpy(IP, argv[1]);
	PORTNUM = atoi(argv[2]);

	title();

	return 0;
}

void tempscreen()
{
	clear();
	mvprintw(10, 33, "Yeeeeeeeeeeeeeeeeeah!!!!!!!!");
	refresh();
}
void tempscreen2()
{
	clear();
	mvprintw(10,33,"SHIIIIIIIIIITTTTTTTTTTTTTTTTTTTTTT");
	refresh();
}
void waitscreen()
{
	clear();
	mvprintw(10, 33, "Please wait...");
	refresh();

}
void title()
{
	int ch, cnt = 1;
	int y, x, i, j;

	stdscr = initscr();
	clear();

	memset(board_number, 0, sizeof(int) * MAX * MAX);
	memset(check_double_number, 0, sizeof(int) * 51);

	y = 5;
	x = 26;
	keypad(stdscr, TRUE);//can get keypad
	noecho();

	curs_set(0);//make cursor invisible

	clear();

	for (i = 0; i < 5; i++)
	{
		move(y + i, x);

		for (j = 0; j < 31; j++)
		{
			if (titlename[i][j] == 1)
				addch('@');
			else
				addch(' ');
		}
	}

	mvprintw(19, x - 2, "**Made by Korean Bingo Association**");
	mvprintw(20, x + 9, "Dear Prof. Hong");
	mvprintw(23, x - 13, "HD.Lee 2016114965  HS.Kwon 2016115613  JH.Shin 2017112110");

	refresh();
	select_menu(y, x, 1);


	while (1)
	{
		ch = getch();
		if (ch == KEY_DOWN || ch == KEY_UP)
		{
			if (ch == KEY_DOWN)
			{
				if (cnt == 3)
					cnt = 1;
				else
					cnt++;

			}
			else
			{

				if (cnt == 1)
					cnt = 3;
				else
					cnt--;

			}

			if (cnt == 1)
				select_menu(y, x, 1);
			else if (cnt == 2)
				select_menu(y, x, 2);
			else
				select_menu(y, x, 3);
		}

		else if (ch == 10)
			break;
		else
			continue;
	}

	//for each case screen

	switch (cnt)
	{
	case 1:
		play(y, x);
		break;
	case 2:
		explain();
		break;
	case 3:
		end_game();
	}
	title();
}
void select_menu(int y, int x, int menu)
{

	standend();
	mvprintw(y + 6, x, "*******************************");
	if (menu == 1)
		standout();
	mvprintw(y + 7, x, "           Start Game          ");
	if (menu == 1)
		standend();
	if (menu == 2)
		standout();
	mvprintw(y + 8, x, "           Game  Rule          ");
	if (menu == 2)
		standend();
	if (menu == 3)
		standout();
	mvprintw(y + 9, x, "              Exit             ");
	if (menu == 3)
		standend();
	mvprintw(y + 10, x, "*******************************");
	refresh();
}

void play(int y, int x)
{
	int ch;
	int startY = 1, startX = 3;
	int letterY = startY + 5;
	int letterX = startX + 38;
	char* donemessage = "DONE!\n";
	int n;
	char recvmessage[BUF_SIZE];
	int erase_num;
	
	clear();
	waitscreen();

	//stage1. socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(IP);
	serv_adr.sin_port = htons(PORTNUM);

	//stage 2. connect
	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	nfds = sock + 1;
	FD_ZERO(&read_fds);
	//send & receive messages with server

	while (1)
	{
		FD_SET(0, &read_fds);
		FD_SET(sock, &read_fds);

		//stage3. select
		if (select(nfds, &read_fds, (fd_set*)0, (fd_set*)0, (struct timeval*)0) < 0)
		{
			error_handling("select() error!");
			return;
		}

		memset(recvmessage, 0, strlen(recvmessage));
		memset(sendmessage, 0, strlen(sendmessage));

		//recv from the server
		if (FD_ISSET(sock, &read_fds))
		{
			if ((size = read(sock, recvmessage, BUF_SIZE)) > 0)
			{
				recvmessage[size] = '\0';
				if (!strncmp(recvmessage, "game start!\n", strlen(recvmessage)))
				{
					sleep(3);
					break;
				}
			}
			else
			{
				error_handling("receive error!");
			}
		}

	}
	//erase wait sentence
	mvprintw(10,33,"				");
	display_mainboard(startX, startY);
	refresh();
	

	while (filled_block < 25)
	{
		display_message(startX, startY, letterX, letterY);
		input_number(startX, startY, letterX, letterY);
		filled_block++;
	}

	if ((n = send(sock, donemessage, strlen(donemessage), 0)) > 0)
	{ // tell the server I'm DONE!

		erase_notice(letterX, letterY);
		while(1)
		{
		// all done?
			ready_to_fight(letterX,letterY);
			sleep(3);
			if((n = read(sock,recvmessage,BUF_SIZE))>0)
			{	
				recvmessage[n] = '\0';
				if(strncmp(recvmessage,startcalling,strlen(recvmessage))==0)
				{	
				// all done!
					break;
			
				}
			}
		}
	}
	
	erase_notice(letterX,letterY);
	
	while (1)
	{
		//default: no messages->waiting for my turn.
		ready_to_fight2(letterX, letterY);
		sleep(2);
		//ANY MESSAGES FROM SERVER?
		if ((n = read(sock, recvmessage, BUF_SIZE)) > 0)
		{
			recvmessage[n] = '\0';

			if (strncmp(recvmessage, turnmessage, strlen(recvmessage)) == 0)
			{
				//MY TURN? -> tell my number to sever!
				
				input_number2(startX,startY,letterX,letterY);
			//	while(1){sleep(3);}
			}
			else
			{
			 //any number from server -> erase and update the board !
				erase_num = str_int(recvmessage);
				if(check_and_erase(erase_num)>0)
				{ //YEAH! EXIST!
				  tempscreen();
				  while(1){sleep(3);}
				}
				else
				{
					//should be modified - just for checking
					tempscreen2();
					while(1){sleep(3);}
					
				}
				
			}
		}//if read
		
	}//while

	close(sock);
}//all boards set complete

/*
		while (bingo_check() < 3)
		{
				display_message2(startX, startY, letterX, letterY);
				input_number2(startX, startY, letterX, letterY);
		}
		ready_to_bingo(letterX, letterY);
		clear();
		endwin();
		return;*/
int check_and_erase(int erase_num)
{
	int i,j;
	
	//1. check if it exist in my board
	for(i=0;i<5;i++)
	{
	 for(j=0;j<5;j++)
	 {
		if(board_number[i][j]==erase_num)
			return 1;
	 }
	}
	return -1;
	//2. then delete -> X mark.
}
void input_number(int startX, int startY, int letterX, int letterY)
{
	int row, col, int_num;
	char num[50];
	char sendmessage[BUF_SIZE] = "";
	int n;

	curs_set(1);
	echo();

	while (1)
	{
		mvscanw(letterY + 5, letterX, "%d %d %s", &row, &col, num);
		row -= 1;
		col -= 1;
		int_num = str_int(num);
		if (check(int_num) || (int_num > 50) || (int_num < 1))
		{
			mvprintw(letterY + 5, letterX, "You can't input the number !");
			refresh();
			sleep(2);
			mvprintw(letterY + 5, letterX, "                            ");
			// erase the input field
			refresh();
		}
		else
		{
			check_double_number[int_num] = int_num;
			if (board_number[row][col] > 0)
			{
				check_double_number[board_number[row][col]] = 0;
				filled_block--;

			}

			/*	sprintf(sendmessage, "%d %d %d", row,col,int_num);
				sendmessage[strlen(sendmessage)] = '\0';
				if((n=send(sock,sendmessage,strlen(sendmessage),0))>0)
				{mvprintw(letterY+7,letterX,"%d %d %d",row,col,int_num);			refresh();sleep(3);}
				//send to server=>"row col num"
			*/
			board_number[row][col] = int_num;
			mvprintw(letterY + 5, letterX, "                ");
			// erase the input field
			display_board(startX, startY, row, col, num);
			break;
		}
	}
}
//
void display_mainboard(int startX, int startY)
{
	int i, j, row, col;
	char num;

	for (i = 0; i < 22; i++)
	{
		move(startY + 2 + i, startX);
		for (j = 0; j < 36; j++)
		{
			if (board_picture[i][j] == 0 || board_picture[i][j] == 3)
				addch(' ');
			else if (board_picture[i][j] == 1)
				addch('|');
			else if (board_picture[i][j] == 2)
				addch('-');
		}
	}
	refresh();
}
void display_message(int startX, int startY, int letterX, int letterY)
{
	mvprintw(startY, startX + 8, "<<YOUR BINGO BOARD>>");
	mvaddstr(letterY - 1, letterX, "Make your own board right now!");
	mvaddstr(letterY, letterX, "All numbers must be between 1 and 50.");
	mvaddstr(letterY + 1, letterX, "You can input a number only once.");
	mvaddstr(letterY + 3, letterX, "Input this format -> row column number");
	refresh();
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
int check(int int_num)
{
	int i;
	for (i = 0; i < 51; i++)
	{
		if (check_double_number[int_num] != 0)
			return 1;//exist
		else
			return 0;//no exist
	}
}


void display_board(int startX, int startY, int row, int col, char* num)
{
	int i, j;
	int boardX, boardY;
	boardY = startY + 4;
	boardX = startX + 3;

	if (strlen(num) == 1)
	{
		mvprintw(boardY + row * 4, boardX + col * 7, " ");
		mvprintw(boardY + row * 4, boardX + 1 + col * 7, num);
	}
	else
		mvprintw(boardY + row * 4, boardX + col * 7, num);

	refresh();
}
void erase_notice(int letterX, int letterY)
{
	int i;
	for (i = -1; i < 8; i++)
		mvprintw(letterY + i, letterX, "                                        ");
	refresh();
}
void ready_to_fight(int letterX, int letterY)
{
	standout();
	mvprintw(letterY + 5, letterX + 4, "We're waiting other players...");
	standend();
	refresh();
	noecho();
}
void ready_to_fight2(int letterX, int letterY)
{
	mvprintw(letterY + 5, letterX + 4, "								");
	standout();
	mvprintw(letterY + 5, letterX + 4, "Waiting for my turn....");
	standend();
	refresh();
	noecho();
}

void explain()
{
	int pointy = 6;
	int pointx = 18;
	int ch;

	clear();
	refresh();

	mvprintw(pointy + 1, pointx, "      This is a BINGO GAME by 2 people!!!       ");
	mvprintw(pointy + 3, pointx, "1. Make a bingo board with pressing a number key.");
	mvprintw(pointy + 5, pointx, "2. Put a number one by one.");
	mvprintw(pointy + 7, pointx, "3. If you make 3 lines first, you WIN.");
	mvprintw(pointy + 9, pointx, "4. Just play it now!");
	refresh();

	mvprintw(LINES - 2, 60, "Go back : q");
	refresh();

	while (1)
	{
		ch = getch();

		if (ch == 'q' || ch == 'Q')
		{
			return;
		}
	}
}
void end_game()
{
	endwin();
	exit(1);
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void input_number2(int startX, int startY, int letterX, int letterY)
{
	int int_num;
	char num[50];
	int i, j,n;
	
	curs_set(1);
	

	erase_notice(letterX,letterY);
	display_message2(startX,startY,letterX,letterY);
	while(1)
   {    //clear the buffer
	echo();
	memset(num,0,sizeof(num));
	mvscanw(letterY + 5, letterX, "%s", num);
	int_num = str_int(num);
	
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (board_number[i][j] == int_num)
			{
			       // erase the input field
			//	board_number[i][j] = -1;
			//	display_board(startX, startY, i, j, "X");
				if((n = write(sock,num,strlen(num)))>0)
			       {
				noecho();
				return;
				}
			}
		}
	}

	mvprintw(letterY + 5, letterX, "You don't have this number !");
	refresh();
	sleep(2);
	mvprintw(letterY + 5, letterX, "                            "); // erase the input field
  }	

}
void display_message2(int startX, int startY, int letterX, int letterY)
{
        mvprintw(startY, startX + 8, "<<YOUR BINGO BOARD>>");
        mvaddstr(letterY - 1, letterX, "Erase a number you want in your turn!");
        mvaddstr(letterY, letterX, "All numbers must be between 1 and 50.");
        mvaddstr(letterY + 1, letterX, "You can input a number only once.");
        mvaddstr(letterY + 3, letterX, "Input this format -> number");
        mvprintw(LINES - 2, 60, "Go back : q");
        refresh();
}


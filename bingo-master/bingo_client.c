
/*
* compile: gcc client.c -o client -lpthread
* build: ./client 127.0.0.1 9011
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
#include <termios.h>
#include <pthread.h>
#include<signal.h>
#include<ncurses.h>
#include<ncursesw/curses.h>

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
//thread
void* result_thread();

//socket number of this client
int sock;
int bingo_sock;

//contents of bingo board of this client
int board_number[5][5];

//check if it has already written.
int check_double_number[51];

//count the number of filled block
int filled_block = 0;

//screen
void waitscreen();

//print message
void waiting_turn_msg(int letterX, int letterY);
void ready_to_fight_message(int letterX, int letterY);

//function for title screen
void title();
void select_menu(int y, int x, int menu);
void winner_screen(int, int);
void loser_screen(int, int);

//function for play, explanation, quit
void play(int y, int x);
void explain();
void end_game();
void erase_notice(int letterX, int letterY);
void input_number(int startX, int startY, int letterX, int letterY);
int str_int(char* num);
int isExist(int int_num);
void input_erase_number(int startX, int startY, int letterX, int letterY);
void erase_num_board(int, int, int);
int check_num_board(int erase_num);

//display
void display_mainboard(int startX, int startY);
void display_message(int startX, int startY, int letterX, int letterY);
void display_message2(int startX, int startY, int letterX, int letterY);
void display_board(int startX, int startY, int row, int col, char* num);

//error handling - exit
void error_handling(char* message);
int bingo_check();
void ready_to_bingo();

//signal handling
void ctrl_c_handler(int signum);
int set_ticker(int n_msecs);
void hurry(int signum);

//needed variable
char send_msg[BUF_SIZE + 1];
char receive_msg[BUF_SIZE];
char message[BUF_SIZE];
char* startgame = "game start!\n";
char* start_signal = "call the number!\n";
char* turn_msg = "TURN\n";
char* winner_msg = "WINNER\n";
char* loser_msg = "LOSER\n";
int wait_flag = 0;

int str_len;
struct sockaddr_in serv_adr;

int size;
int cnt = 0;
int PORTNUM;
int quit_sign = 0;
int win, lose;
char IP[20];

pthread_t res_thread;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>  \n", argv[0]);
		exit(1);
	}
	strcpy(IP, argv[1]);
	PORTNUM = atoi(argv[2]);

	signal(SIGINT, ctrl_c_handler);//unexpected sigint prevention
	signal(SIGQUIT, SIG_IGN);

	title();

	return 0;
}

void waitscreen()
{
	int i = 0;

	clear();

	while (i < 3)
	{
		if (i == 0)
		{
			mvprintw(10, 33, "              ");
			mvprintw(10, 33, "Please wait.");
		}

		else if (i == 1)
		{
			mvprintw(10, 33, "              ");
			mvprintw(10, 33, "Please wait..");
		}

		else if (i == 2)
		{
			mvprintw(10, 33, "              ");
			mvprintw(10, 33, "Please wait...");

		}
		refresh();
		sleep(1);
		i++;
	}
}

void title()
{
	int ch, cnt = 1;
	int y, x, i, j;

	stdscr = initscr();
	clear();
	start_color();

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_CYAN);

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
				addch('@' | COLOR_PAIR(6));
			else
				addch(' ' | COLOR_PAIR(1));
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
	int ch, check;
	int startY = 1, startX = 3;
	int letterY = startY + 5;
	int letterX = startX + 38;
	char* donemessage = "DONE!\n";
	int n;
	char receive_msg[BUF_SIZE];
	int erase_num;

	clear();
	waitscreen();

	//stage1. socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	bingo_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1 || bingo_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(IP);
	serv_adr.sin_port = htons(PORTNUM);

	//stage 2. connect
	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	if (connect(bingo_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	//run the thread to receive bingo message from players.
	pthread_create(&res_thread, NULL, result_thread, NULL);
	//send & receive messages with server`

	while (1)
	{
		memset(receive_msg, 0, strlen(receive_msg));
		memset(send_msg, 0, strlen(send_msg));

		//recv from the server

		if ((size = read(sock, receive_msg, BUF_SIZE)) == -1)
			error_handling("receive error!");

		receive_msg[size] = '\0';
		if (!strncmp(receive_msg, "game start!\n", strlen(receive_msg)))
		{
			sleep(3);
			break;
		}
	}
	//erase the wait sentence
	mvprintw(10, 33, "               ");
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
		while (1)
		{
			// all done?
			ready_to_fight_message(letterX, letterY);
			sleep(1);
			mvprintw(letterX, letterY, "               ");
			if ((n = read(sock, receive_msg, BUF_SIZE)) > 0)
			{
				receive_msg[n] = '\0';
				if (strncmp(receive_msg, start_signal, strlen(receive_msg)) == 0)
				{
					// all done!
					break;
				}
			}
		}
	}
	erase_notice(letterX, letterY);


	/* Start to input the number to erase */
	while ((check = bingo_check()) < 1 && quit_sign == 0 )//can change bingo number!
	{
		mvprintw(letterY + 7, letterX, "Your bingo line : %d", check);

		//default: waiting for my turn.
		waiting_turn_msg(letterX, letterY);

		//ANY MESSAGES FROM SERVER?
		if ((n = read(sock, receive_msg, BUF_SIZE)) == -1)
			error_handling("read error()");

		receive_msg[n] = '\0';

		// When it's my turn..
		if (strncmp(receive_msg, turn_msg, strlen(receive_msg)) == 0)
		{
			//MY TURN? -> tell my number to sever!
			curs_set(1);
			erase_notice(letterX, letterY);
			input_erase_number(startX, startY, letterX, letterY);
			curs_set(0);
		}

		// When it's other player's turn..
		else
		{
			//any number from server -> erase and update the board !

			erase_num = str_int(receive_msg);
			if (check_num_board(erase_num) > 0)
				erase_num_board(startX, startY, erase_num);

			else;
			// There's nothing changed
		}
	}//while
	if (quit_sign) {
		if (win) winner_screen(11, 35);
		if (lose) loser_screen(11, 35);
		pthread_cancel(res_thread);
		pthread_join(res_thread, NULL);
		while (1);
	}
	
	ready_to_bingo(letterX, letterY);
	clear();
	endwin();
	close(sock);
	close(bingo_sock);
	return;
}//all boards set complete

int check_num_board(int erase_num)
{
	int i, j;

	//1. check if it exist in my board
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (board_number[i][j] == erase_num)
				return 1;
		}
	}
	return 0;
	//2. then delete -> X mark.
}
void input_number(int startX, int startY, int letterX, int letterY)
{
	int row, col, int_num;
	char num[50];
	char send_msg[BUF_SIZE] = "";
	int n;
	char curstring[10];


	//row = index / 5; col = index % 5;
	curs_set(1);
	echo();
	mvprintw(LINES - 2, 60, "              ");

	while (1)
	{
		mvscanw(letterY + 5, letterX, "%d %d %s", &row, &col, num);
		row -= 1;
		col -= 1;

		//mvscanw(letterY + 5, letterX, "%s", num);

		int_num = str_int(num);
		if (isExist(int_num) || (int_num > 50) || (int_num < 1))
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
			if (board_number[row][col] > 0) // 이미 숫자를 입력한 칸에 다른 숫자로 변경
			{
				check_double_number[board_number[row][col]] = 0;
				filled_block--;
			}

			board_number[row][col] = int_num;
			mvprintw(letterY + 5, letterX, "                            ");// erase the input field

			display_board(startX, startY, row, col, num);

			break;
		}
	}

}

void display_mainboard(int startX, int startY)
{
	int i, j, row, col;
	char num;

	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);

	for (i = 0; i < 22; i++)
	{
		move(startY + 1 + i, startX);
		for (j = 0; j < 36; j++)
		{
			if (board_picture[i][j] == 0 || board_picture[i][j] == 3)
			{
				addch(' ');
			}

			else if (board_picture[i][j] == 1)
			{
				addch('|' | COLOR_PAIR(4));
			}
			else if (board_picture[i][j] == 2)
			{
				addch('-' | COLOR_PAIR(4));
			}

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
int isExist(int int_num)
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
	boardY = startY + 3;
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
void ready_to_fight_message(int letterX, int letterY)
{
	curs_set(0);
	standout();
	mvprintw(letterY + 5, letterX + 4, "We're waiting other players..");
	standend();
	refresh();
	noecho();
}
void waiting_turn_msg(int letterX, int letterY)
{
	curs_set(0);
	mvprintw(letterY + 5, letterX, "                        ");
	standout();
	mvprintw(letterY + 5, letterX, "Waiting for my turn..");
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

	mvprintw(pointy + 1, pointx, "      This is a BINGO GAME by 3 people!!!       ");
	mvprintw(pointy + 3, pointx, "1. Make a bingo board with pressing 3 numbers(row, col, number).");
	mvprintw(pointy + 5, pointx, "2. Put a number one by one to erase.");
	mvprintw(pointy + 7, pointx, "3. If you make 3 lines first, you WIN.");
	mvprintw(pointy + 9, pointx, "4. Just play it now!");
	refresh();

	curs_set(0);
	mvprintw(LINES - 2, 60, "QUIT : q");
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

void input_erase_number(int startX, int startY, int letterX, int letterY)
{
	int check;
	int num_to_erase;
	char num[50];
	int i, j, n;
	curs_set(1);
	init_pair(6, COLOR_RED, COLOR_RED);
	if (quit_sign) return;
	erase_notice(letterX, letterY);
	display_message2(startX, startY, letterX, letterY);
	check = bingo_check();
	mvprintw(letterY + 7, letterX, "Your bingo line : %d", check);
	refresh();
	
	while (quit_sign == 0)
	{    //clear the buffer
		echo();
		memset(num, 0, sizeof(num));
		mvscanw(letterY + 5, letterX, "%s", num);
		num_to_erase = str_int(num);

		for (i = 0; i < MAX; i++)
		{
			for (j = 0; j < MAX; j++)
			{
				if (board_number[i][j] == num_to_erase)
				{
					// erase the input field
					board_number[i][j] = -1;
					attron(COLOR_PAIR(6));
					display_board(startX, startY, i, j, "X");
					attroff(COLOR_PAIR(6));
					if ((n = write(sock, num, strlen(num))) > 0)
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

void erase_num_board(int startX, int startY, int erase_num)
{
	init_pair(6, COLOR_RED, COLOR_RED);
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (board_number[i][j] == erase_num)
			{
				// erase the input field
				board_number[i][j] = -1;
				attron(COLOR_PAIR(6));
				display_board(startX, startY, i, j, "X");
				attroff(COLOR_PAIR(6));
			}
		}
	}
}

void display_message2(int startX, int startY, int letterX, int letterY)
{
	mvprintw(startY, startX + 8, "<<YOUR BINGO BOARD>>");
	mvaddstr(letterY - 1, letterX, "Erase a number you want in your turn!");
	mvaddstr(letterY, letterX, "All numbers must be between 1 and 50.");
	mvaddstr(letterY + 1, letterX, "You can input a number only once.");
	mvaddstr(letterY + 3, letterX, "Input this format -> number");
	refresh();
}

void ready_to_bingo()
{
	int ch, n;
	int letterX = 25;
	int letterY = 12;
	char answer[10];
	char temp_msg[20];
	clear();
	if (quit_sign) return;
	mvprintw(letterY, letterX, "Write \"BINGO\" first!!!!");

	while (1)
	{
		echo();
		mvscanw(letterY + 1, letterX, "%s", answer);
		refresh();

		if (!strcmp(answer, "BINGO"))
			break;
		mvprintw(letterY + 1, letterX, "         ");
	}
	if ((n = write(bingo_sock, answer, strlen(answer))) == -1)
		error_handling("write error()");

	noecho();
	curs_set(0);
}

// Check if it is BINGO
int bingo_check()
{
	int garo = 0, sero = 0, diagL = 0, diagR = 0;
	int i, j, bingo = 0;

	for (i = 0; i < MAX; i++)
	{
		garo = 0;
		for (j = 0; j < MAX; j++)
		{
			if (board_number[i][j] == -1) garo++;

			if (i == j)
				if (board_number[i][j] == -1)
					diagL++;

			if ((i + j) == 4)
				if (board_number[i][j] == -1)
					diagR++;
		}

		if (garo == MAX)
			bingo++;
	}
	for (j = 0; j < MAX; j++)
	{
		sero = 0;
		for (i = 0; i < MAX; i++)
			if (board_number[i][j] == -1) sero++;

		if (sero == 5)
			bingo++;
	}

	bingo += (diagL / 5) + (diagR / 5);

	return bingo;
}

void winner_screen(int letterY, int letterX)
{
	curs_set(0);
	clear();
	mvprintw(letterY - 1, letterX - 2, "*****************");
	mvprintw(letterY, letterX - 2, "*    YOU WIN!   *");
	mvprintw(letterY + 1, letterX - 2, "*****************");
	mvprintw(LINES - 2, 58, "QUIT : Ctrl + C");
	refresh();
}

void loser_screen(int letterY, int letterX)
{
	curs_set(0);
	clear();
	mvprintw(letterY - 1, letterX - 2, "******************");
	mvprintw(letterY, letterX - 2, "*    YOU LOSE!   *");
	mvprintw(letterY + 1, letterX - 2, "******************");
	mvprintw(LINES - 2, 58, "QUIT : Ctrl + C");
	refresh();
}

void* result_thread()
{
	int n, ch;
	char temp_msg[10];
	while ((n = read(bingo_sock, temp_msg, BUF_SIZE)) != -1)
	{
		temp_msg[n] = '\0';
		refresh();
		quit_sign = 1;

		if (!strcmp(temp_msg, winner_msg))
		{
			winner_screen(11, 35);
			win = 1;
			while (1) sleep(1);
		}
		else
		{
			loser_screen(11, 35);
			lose = 1;
			while (1) sleep(1);
		}
		
	}
}

void ctrl_c_handler(int signum)
{
	clear();
	mvprintw(12, 25, "         Quit...   ");
	refresh();
	sleep(2);
	endwin();
	close(bingo_sock);
	close(sock);
	exit(1);
}
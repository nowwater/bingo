#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include <pthread.h>
#include<ncurses.h>
//#include<curses.h>

#define LEFTEDGE 20
#define RIGHTEDGE 18
#define ROW 20
#define SPACE 32
#define WHITESPACE 30
#define TAB 9
#define q 113
#define Q 81
#define BUF_SIZE 1024

int board_p1[5][5];
int board_p2[5][5];
int check_double_number[51]; // if we check the double number input, gonna ask them to input a different number.
//board of each person

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

//really int_number in board
int board_number[5][5];

//function for  title screen
void title();
void game_mode(int y, int x);
void explain_mode(int, int);
void quit_mode(int, int);

//function for play, explanation, quit
void play(int, int);
void explain();
void end_game();
void display_mainboard(int startX, int startY);
void display_board(int startX, int startY, int row, int col, char* num);
void input_number(int startX, int startY, int letterX, int letterY);
void erase_notice(int letterX, int letterY);
void ready_to_fight(int letterX, int letterY);
int str_int(char* num);
int check(int int_num);
int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	stdscr = initscr();

	title();

	return 0;
}

void title()
{
	//char rec[] = "бс";
	int ch, cnt = 1;
	int y, x, i, j;
	//getmaxyx(stdscr, y, x);//get screensize
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

	game_mode(y, x);

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
				game_mode(y, x);
			else if (cnt == 2)
				explain_mode(y, x);
			else
				quit_mode(y, x);
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

void game_mode(int y, int x)
{
	standend();
	mvprintw(y + 6, x, "*******************************");
	standout();
	mvprintw(y + 7, x, "           Start Game          ");
	standend();
	mvprintw(y + 8, x, "           Game  Rule          ");
	mvprintw(y + 9, x, "              Exit             ");
	mvprintw(y + 10, x, "*******************************");
	refresh();
}

void explain_mode(int y, int x)
{
	standend();
	mvprintw(y + 6, x, "*******************************");
	mvprintw(y + 7, x, "           Start Game          ");
	standout();
	mvprintw(y + 8, x, "           Game  Rule          ");
	standend();
	mvprintw(y + 9, x, "              Exit             ");
	mvprintw(y + 10, x, "*******************************");
	refresh();
}


void quit_mode(int y, int x)
{
	standend();
	mvprintw(y + 6, x, "*******************************");
	mvprintw(y + 7, x, "           Start Game          ");
	mvprintw(y + 8, x, "           Game  Rule          ");
	standout();
	mvprintw(y + 9, x, "              Exit             ");
	standend();
	mvprintw(y + 10, x, "*******************************");
	refresh();
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
void display_message(int startX, int startY, int letterX, int letterY)
{
	mvprintw(startY, startX+10, "<<YOUR BINGO BOARD>>");
	mvaddstr(letterY-1, letterX, "Make your own board right now!");
	mvaddstr(letterY , letterX, "All numbers must be between 1 and 50.");
	mvaddstr(letterY+1, letterX, "You can input a number only once.");
	mvaddstr(letterY + 3, letterX, "Input this format -> row column number");
	mvprintw(LINES - 2, 60, "Go back : q");
	refresh();
}

void display_mainboard(int startX, int startY)
{
	int i, j, row, col;
	char num;

	for (i = 0; i < 22; i++)
	{
		move(startY + 2 + i, startX);
		for (j = 0; j < 36; j++)
		{
			if (board_picture[i][j] == 0 || board_picture[i][j]==3)
				addch(' ');
			else if (board_picture[i][j] == 1)
				addch('|');
			else if (board_picture[i][j] == 2)
				addch('-');
		}
	}
	refresh();
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
		mvprintw(boardY + row*4, boardX + col*7, num);

	refresh();
}

void input_number(int startX, int startY, int letterX, int letterY)
{
	int row, col, int_num;
	char num[50];
	curs_set(1);
	echo();

	while (1)
	{
		mvscanw(letterY + 5, letterX, "%d %d %s", &row, &col, num);
		row -= 1;
		col -= 1;
		int_num = str_int(num);
		if (check(int_num)||(int_num>50)||(int_num<1))
		{
			mvprintw(letterY + 5, letterX, "You can't input the number !");
			refresh();
			sleep(1);
			mvprintw(letterY + 5, letterX, "                            "); // erase the input field
			refresh();
		}
		else
		{
			check_double_number[int_num] = int_num;
			if (board_number[row][col] > 0)
				check_double_number[board_number[row][col]] = 0;
			board_number[row][col] = int_num;
			mvprintw(letterY + 5, letterX, "                "); // erase the input field
			display_board(startX, startY, row, col, num);
			break;
		}
	}
}
void erase_notice(int letterX, int letterY)
{
	mvprintw(letterY, letterX,     "                                  ");
	mvprintw(letterY + 1, letterX, "                                  ");
	mvprintw(letterY + 3, letterX, "                                  ");
	refresh();
}
void ready_to_fight(int letterX, int letterY)
{
	erase_notice(letterX, letterY);
	standout();
	mvprintw(letterY + 5, letterX+4, "We're waiting other players.");
	standend();
	refresh();
	noecho();
}
void play(int y, int x)
{
	int cnt = 0, ch;
	int startY = 1, startX = 3;
	int letterY = startY + 5;
	int letterX = startX + 38;
	clear();
	refresh();

	display_mainboard(startX, startY);
	refresh();

	while (cnt < 25)
	{
		display_message(startX, startY, letterX, letterY);
		input_number(startX, startY, letterX, letterY);
		cnt++;
	}
	ready_to_fight(letterX, letterY);

	while (1)
	{
		ch = getch();

		if (ch == 'q' || ch == 'Q')
		{
			return;
		}
	}
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
			return 1;
		else
			return 0;
	}
}
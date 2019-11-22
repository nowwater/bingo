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

/*int Board[20][12] = {//첫번째 판 초기화
		{1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1}
};
*/

int board_p1[5][5];
int board_p2[5][5];
//board of each person

//int titlename[10][60] =
//{
//	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
//	{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
//	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0},
//	{1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
//	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1},
//	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
//	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
//	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0},
//	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
//	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
//};

int titlename[5][31] =
{
	{1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0},
	{1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1},
	{1,1,1,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,0,0,1},
	{1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0}
};

int board_picture[22][32] =
{
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1}
};


//function for  title screen
void title();
void game_mode(int y, int x);
void explan_mode(int, int);
void quit_mode(int, int);

//function for play, explanation, quit
void play(int, int);
void explan();
void end_game();

//시작 높이 y=5, x=22
int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	stdscr=initscr();

	title();

	return 0;
}

void title()
{
	//char rec[] = "■";
	int ch, cnt=1;
	int y,x,i,j;
	//getmaxyx(stdscr, y, x);//get screensize
	y = 5;
	x = 26;
	keypad(stdscr, TRUE);//can get keypad
	noecho();

	curs_set(0);//make cursor invisible
	
	clear();

	for (i = 0; i < 5; i++)
	{
		move(y+i, x);

		for (j = 0; j < 31; j++)
		{
			if (titlename[i][j] == 1)
				addch('@');
			else
				addch(' ');
		}
	}
	
	mvprintw(19, x-2, "**Made by Korean Bingo Association**");
	mvprintw(22, x-13, "HD.Lee 2016114965  HS.Kwon 2016115613  JH.Shin 2017112110");

	refresh();

	game_mode(y,x);
	
	while(1)
	{
		ch = getch();
		if(ch == KEY_DOWN||ch == KEY_UP)
		{
			if(ch == KEY_DOWN)
			{
				if(cnt==3)
					cnt=1;
				else
					cnt++;
			}
			else
			{
				if(cnt==1)
					cnt=3;
				else
					cnt--;
			}

			if(cnt == 1)
				game_mode(y,x);
			else if(cnt == 2)
				explan_mode(y,x);	
			else
				quit_mode(y,x);
		}

		else if(ch == 10)
			break;
		else
			continue;
	}
	//for each case screen
	
	switch(cnt)
	{
		case 1:
			play(y,x);
			break;
		case 2:
			explan();
			break;
		case 3:
			end_game();		
	}
	title();
}

void game_mode(int y, int x)
{
	standend();
	mvprintw(y+6, x,    "*******************************");
	standout();
	mvprintw(y+7,x,     "           Start Game          ");
	standend();
	mvprintw(y + 8, x,  "           Game  Rule          ");
	mvprintw(y + 9, x,  "              Exit             ");
	mvprintw(y + 10, x, "*******************************");
	refresh();
}

void explan_mode(int y, int x)
{
	standend();
	mvprintw(y + 6, x, "*******************************");
	mvprintw(y + 7, x, "           Start Game          ");
	standout();
	mvprintw(y + 8, x, "           Game  Rule          ");
	standend();
	mvprintw(y + 9, x, "              Exit             ");
	mvprintw(y + 10, x,"*******************************");
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
	mvprintw(y + 10, x,"*******************************");
	refresh();
}

void explan()
{
	int pointy = 6;
	int pointx = 18;
	int ch;

	clear();
	refresh();

	mvprintw(pointy+1, pointx, "      This is a BINGO GAME by 2 people!!!       ");
	mvprintw(pointy+3, pointx, "1. Make a bingo board with pressing a number key." );
	mvprintw(pointy+5, pointx, "2. Put a number one by one." );
	mvprintw(pointy+7, pointx, "3. If you make 3 lines first, you WIN." );
	mvprintw(pointy+9, pointx, "4. Just play it now!");
	refresh();

	mvprintw(LINES-2, 60, "Go back : q");
	refresh();

	while(1)
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

void play(int y, int x)
{
	int ch;
	int startY=1;
	int startX=3;
	int letterX;
	int letterY;
	int i, j;
	char str[10];

	clear();
	refresh();
	
	mvprintw(startY,startX, "     Make your own board!");
	refresh();	
	
	mvprintw(LINES - 2, 60, "Go back : q");
	refresh();

	for (i = 0; i<22; i++)
	{
		move(startY + 2 + i, startX);
		for (j = 0; j < 32; j++)
		{
			if (board_picture[i][j] == 0)
				addch(' ');
			else if (board_picture[i][j] == 1)
				addch('|');
			else
				addch('-');
		}
	}
	refresh();

	letterY = startY + 5;
	letterX = startX+34;

	mvprintw(letterY, letterX, "Press a number(1~50) : ");
	refresh();

	move(letterY, letterX+23);
	curs_set(1);
	refresh();

	//to do
	while (1)
	{
		ch = getch();

		if (ch == 'q' || ch == 'Q')
		{
			return;
		}
	}
}
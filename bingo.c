#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <pthread.h>
#include <ncurses.h>
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

/*int Board[20][12] = {//첫번째 판 초기화화
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

int titlename[10][60] =
{
	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
	{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0},
	{1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
};

int board_picture[22][32] =
{
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1,0,0,3,0,0,1},
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
void explan(int, int);
void end_game();

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
	getmaxyx(stdscr, y, x);//get screensize
	keypad(stdscr, TRUE);//can get keypad
	noecho();

	curs_set(0);//make cursor invisible
	
	clear();

	for (i = 0; i < 10; i++)
	{
		move((y / 6) + i, (x / 3)+5);

		for (j = 0; j < 60; j++)
		{
			if (titlename[i][j] == 1)
				addch('@');
			else
				addch(' ');
		}
	}
	
	mvprintw((y/7)*6,(x/7)*5,      "Made by Korean Bingo Association");
	mvprintw((y/7)*6 + 1, (x/7)*5, "          Hyodong Lee 2016114965");
	mvprintw((y/7)*6+2,(x/7)*5,    "          Hyunsoo Kwon ~~");
	mvprintw((y/7)*6+3,(x/7)*5,    "          Junhee Shin ~~");

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
			explan(y,x);
			break;
		case 3:
			end_game();		
	}
	title();
}

void game_mode(int y, int x)
{
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	standout();
	mvprintw((y/2)+1, (x/3)+17,"           Start Game          ");
	standend();
	mvprintw((y/2)+2, (x/3)+17,"           Game  Rule          ");
	mvprintw((y/2)+3, (x/3)+17,"              Exit             ");
	mvprintw((y/2)+4,(x/3)+17, "*******************************");
	refresh();
}

void explan_mode(int y, int x)
{
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	mvprintw((y/2)+1, (x/3)+17,"           Start Game          ");
	standout();
	mvprintw((y/2)+2, (x/3)+17,"           Game  Rule          ");
	standend();
	mvprintw((y/2)+3, (x/3)+17,"              Exit             ");
	mvprintw((y/2)+4,(x/3)+17, "*******************************");
	refresh();
}


void quit_mode(int y, int x)
{
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	mvprintw((y/2)+1, (x/3)+17,"           Start Game          ");
	mvprintw((y/2)+2, (x/3)+17,"           Game  Rule          ");
	standout();
	mvprintw((y/2)+3, (x/3)+17,"              Exit             ");
	standend();
	mvprintw((y/2),(x/3)+17,   "*******************************");
	refresh();
}

void explan(int y, int x)
{
	int pointx=x/3;
	int pointy=y/3;
	int ch;

	clear();
	refresh();

	mvprintw(pointy+1, pointx, "      This is a BINGO GAME by 2 people!!!       ");
	mvprintw(pointy+3, pointx, "1. Make a bingo board with pressing a number key." );
	mvprintw(pointy+5, pointx, "2. Put a number one by one." );
	mvprintw(pointy+7, pointx, "3. If you make 3 lines first, you WIN." );
	mvprintw(pointy+9, pointx, "4. Just play it now!");
	refresh();

	mvprintw(4*(y/5), 2*pointx, "Go back : q");
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
void display_board(int y, int x, int flag) // if flag is 1, then it's time to fill the number into the board.
{
	int startY = 10, startX = 70;
	int i, j;
	for (i = 0; i<22; i++)
	{
		move(startY + 2 + i, startX);
		for (j = 0; j < 32; j++)
		{
			if (board_picture[i][j] == 0)
				addch(' ');
			else if (board_picture[i][j] == 1)
				addch('|');
			else if (board_picture[i][j] == 2)
				addch('-');
			else; // board_picture[i][j] == 3 숫자출력
		}
	}
	refresh();
}
void play(int y, int x)
{
	int ch;
	int startY=10;
	int startX=70;
	int letterX, letterY;
	int i, j, row, col, num;


	clear();
	refresh();
	
	mvprintw(startY,startX, "Make your own board!");
	refresh();	
	
	mvprintw(4*(y/5), 2*x/3, "Go back : q");
	refresh();

	display_board(0, 0, 0);

	letterY = startY + 5 + 22;
	letterX = startX;

	display_board(0, 0, 1);
	mvaddstr(letterY, letterX, "Input the (row , column , number) to fill the number you want");
	echo();
	refresh();
	scanf("%d %d %d", &row, &col, &num);

	curs_set(1);
	refresh();

	move(letterY+1, letterX);
	printw("%d %d %d", row, col, num);

	noecho();

	while (1)
	{
		ch = getch();

		if (ch == 'q' || ch == 'Q')
		{
			return;
		}
	}
}

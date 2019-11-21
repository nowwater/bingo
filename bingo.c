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

int Board[20][12] = {//첫번째 판 초기화화
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


//function for  title screen
void title();
void game_mode(int y, int x);
void explan_mode(int, int);
void quit_mode(int, int);

//function for play, explanation, quit
void play();
void explan();

int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	stdscr=initscr();

	title();

	return 0;
}

void title()
{
	int ch, cnt=1;
	int y,x,i,j;
	getmaxyx(stdscr, y, x);
	keypad(stdscr, TRUE);
	noecho();

	curs_set(0);
	
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
			play();
			break;
		case 2:
			explan();
			break;
		case 3:
			endwin();
	}

	endwin();
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
	mvprintw((y/2),(x/3)+17,    "*******************************");
	refresh();
}

void play()
{
}

void explan(int y, int x)
{
	int pointx=x/3;
	int pointy=y/3;
	int ch;

	clear();

	mvprintw(pointy, pointx,    "This is a BINGO GAME!");
	mvprintw(pointy+1, pointx, "1. Make a bingo board with pressing a number key." );
	mvprintw(pointy+2, pointx, "2. Put a number one by one." );
	mvprintw(pointy+3, pointx, "3. If you make 3 lines first, you WIN." );
	mvprintw(pointy+4, pointx, "4. Just play it now!");
	
	mvprintw(3*y/4, pointx-17, "Go back : q");
	refresh();

	while(1)
	{
		ch = getch();

		if (ch == 'q' || ch == 'Q')
		{
			title();
		}

	}
	
}

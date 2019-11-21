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

int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	stdscr=initscr();

	title();

	return 0;
}

void title()
{
	int ch;
	int y,x,i,j;
	getmaxyx(stdscr, y, x);
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	game_mode(y,x);
	refresh();

	sleep(5);
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
	mvprintw(y/2,(x/3)+17,     "*******************************");
	refresh();
}

void explan_mode(int y, int x)
{
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	mvprintw((y/2)+1, (x/3)+17,"           게 임 하 기         ");
	standout();
	mvprintw((y/2)+2, (x/3)+17,"           게 임 설 명         ");
	standend();
	mvprintw((y/2)+3, (x/3)+17,"           게 임 종 료         ");
	mvprintw(y/2,(x/3)+17,     "*******************************");
	refresh();
}


void quit_mode(int y, int x)
{
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	mvprintw((y/2)+1, (x/3)+17,"           게 임 하 기         ");
	mvprintw((y/2)+2, (x/3)+17,"           게 임 설 명         ");
	standout();
	mvprintw((y/2)+3, (x/3)+17,"           게 임 종 료         ");
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	refresh();
}

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
#include<ncursesw/curses.h>

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
void make_board();
int main()
{

}

void make_board()
{
	stdscr=initscr();
	//curses 실행
	
	
}

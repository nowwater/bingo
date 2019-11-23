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

int board_picture[22][37] =
{	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1,0,0,3,3,0,0,1 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 }
};
char board_number[6][6] = {"      ", "      ", "      ", "      ", "      ", "      "};
char board_secondnumber[6][6] = {"      ", "      ", "      ", "      ", "      ", "      "};
//function for  title screen
void title();
void game_mode(int y, int x);
void explain_mode(int, int);
void quit_mode(int, int);

//function for play, explanation, quit
void play(int, int);
void explain();
void end_game();

int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	stdscr = initscr();

	title();

	return 0;
}

void title()
{
	//char rec[] = "■";
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
	mvprintw(startY, startX, "Make your own board!");
	mvaddstr(letterY - 1, letterX, "The number is between 1 and 50.");
	mvaddstr(letterY, letterX, "You can input a number only once !");
	mvaddstr(letterY + 2, letterX, "Input the (row , column , number).");
	mvprintw(LINES - 2, 60, "Go back : q");
	refresh();
}
void display_board(int startX, int startY)
{
	int i, j, row, col, cnt = 0;
	char num;

	for (i = 0; i<22; i++)
	{
		int cnt = 0;
		
		move(startY + 2 + i, startX);
		for (j = 0; j < 37; j++)
		{
			
 			
			if (board_picture[i][j] == 0)
				addch(' ');
			else if (board_picture[i][j] == 1)
				addch('|');
			else if (board_picture[i][j] == 2)
				addch('-');
			else  // board_picture[i][j] == 3 숫자출력
				{
					if(cnt%2==0)
					//ignore the second 3 => We will handle it later
						{
						addch(board_number[( (i-3) / 4)+1 ][(j-4)/7+1]);
						cnt++;
						}
					else
						{
						addch(board_secondnumber[((i-3)/4)+1][(j-5)/7+1]);
						cnt--;
						}
				}
		}
	}
	refresh();
}
int input_number(int letterX, int letterY)
{
	int row, col,int_num;
	char num1,num2;
	curs_set(1);
	echo(); // getch() 로 받아야함
	mvscanw(letterY + 4, letterX, "%d %d %c%c", &row, &col, &num1,&num2);
	int_num = num1 - '0';
	// if(row == 81 || row == 113) // if the character 'q' or 'Q'
	// q나 Q 눌렀을 때 SIGARLM 보내서 종료시키게 만들기
	if (check_double_number[int_num])
	{
		mvprintw(letterY + 4, letterX, "You can't input the number !");
		refresh();
		sleep(1);
		mvprintw(letterY + 4, letterX, "										"); // erase the input field
		refresh();
		return int_num; // failed to input the number 'int_num'
	}
	else
	{
		check_double_number[int_num] = int_num;
		board_number[row+1][col+1] = num1; //We dont consider the 0th row and col.
		if(num2!='\0')
			board_secondnumber[row+1][col+1] = num2;
		mvprintw(letterY + 4, letterX, "                "); // erase the input field
		refresh();
		return 0; // success
	}
}
void erase_notice(int letterX, int letterY)
{
	mvprintw(letterY - 1, letterX, "									");
	mvprintw(letterY , letterX, "									");
	mvprintw(letterY + 2, letterX, "									");
	mvprintw(letterY + 4, letterX, "									");
	refresh();
}
void ready_to_fight(int letterX, int letterY)
{
	erase_notice(letterX, letterY);
	standout();
	mvprintw(letterY + 4, letterX, "We're waiting other players.");
	standend();
}
void play(int y, int x)
{
	int ch, cnt = 0, res = 0;
	int startY = 1, startX = 4;
	int letterY = startY + 5;
	int letterX = startX + 38;
	clear();
	refresh();

	while (cnt < 25)
	{
		display_message(startX, startY, letterX, letterY);
		display_board(startX, startY);
		input_number(letterX, letterY);
		if (res)
			continue;
		cnt++;
	}
	ready_to_fight(letterX, letterY);
	refresh();
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

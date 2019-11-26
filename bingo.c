#include <stdio.h>
#include<ncurses.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>

//빙고의 크기 5X5
#define MAX 5

//빙고게임 제목 BINGO
int titlename[5][31] =
{
	{ 1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0 },
	{ 1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1 },
	{ 1,1,1,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1 },
	{ 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,0,0,1 },
	{ 1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0 }
};

//빙고판 그림
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

//숫자가 중복되지 않도록 확인용 숫자를 저장해두는 배열
int check_double_number[51];

//실제 내부적으로 구현되있는 int형 보드판
int board_number[5][5];

//function for title screen
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
void display_message2(int startX, int startY, int letterX, int letterY);
void input_number2(int startX, int startY, int letterX, int letterY);
void ready_to_bingo();
int bingo_check();

int main()
{
	stdscr = initscr();

	title();

	return 0;
}

void title()
{
	//char rec[] = "■";
	int ch, cnt = 1;
	int y, x, i, j;

	//매번 새 게임을 위한 초기화
	memset(board_number, 0, sizeof(int)*MAX*MAX);
	memset(check_double_number, 0, sizeof(int)*51);

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
	mvprintw(startY, startX + 8, "<<YOUR BINGO BOARD>>");
	mvaddstr(letterY - 1, letterX, "Make your own board right now!");
	mvaddstr(letterY, letterX, "All numbers must be between 1 and 50.");
	mvaddstr(letterY + 1, letterX, "You can input a number only once.");
	mvaddstr(letterY + 3, letterX, "Input this format -> row column number");
	refresh();
}

//기본 보드판 틀 출력
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

//숫자가 추가될때마다 출력
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

//숫자받기
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
		if (check(int_num) || (int_num > 50) || (int_num < 1))
		{
			mvprintw(letterY + 5, letterX, "You can't input the number !");
			refresh();
			sleep(2);
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

//오른쪽에 화면 청소
void erase_notice(int letterX, int letterY)
{
	int i;
	for (i = -1; i < 8; i++)
		mvprintw(letterY + i, letterX, "                                        ");
	refresh();
}

//대결준비 완료
void ready_to_fight(int letterX, int letterY)
{
	erase_notice(letterX, letterY);
	standout();
	mvprintw(letterY + 5, letterX + 4, "We're waiting other players.");
	standend();
	refresh();
	noecho();
}

//게임모드로 들어왔을 때
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

	//아래부터는 임시로 구현(서버필요)
	sleep(2);

	erase_notice(letterX, letterY);

	while (bingo_check() < 3)
	{
		display_message2(startX, startY, letterX, letterY);
		input_number2(startX, startY, letterX, letterY);
	}
	ready_to_bingo(letterX, letterY);

	clear();
	endwin();
	return;
}

//string을 int로 바꿔주는 함수
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

//중복 숫자인지 확인해주는 함수
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

//숫자 다쓰고 하나씩 부를 때 나오는 설명
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

//원하는 숫자를 하나씩 부르기
void input_number2(int startX, int startY, int letterX, int letterY)
{
	int int_num;
	char num[50];
	int i, j;
	curs_set(1);
	echo();


	mvscanw(letterY + 5, letterX, "%s", num);
	int_num = str_int(num);

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (board_number[i][j] == int_num)
			{
				mvprintw(letterY + 5, letterX, "        "); // erase the input field
				board_number[i][j] = -1;
				display_board(startX, startY, i, j, "X");

				return;
			}
		}
	}

	mvprintw(letterY + 5, letterX, "You don't have this number !");
	refresh();
	sleep(2);
	mvprintw(letterY + 5, letterX, "                            "); // erase the input field
	return;

}

//빙고상황(외치기 직전)
void ready_to_bingo()
{
	int ch;
	int letterX = 25;
	int letterY = 12;
	char answer[10];
	clear();
	mvprintw(letterY, letterX, "Write \"BINGO\" first!!!!");

	while (1)
	{
		mvscanw(letterY + 1, letterX, "%s", answer);
		refresh();

		if (!strcmp(answer, "BINGO"))
			break;
	}

	noecho();
	curs_set(0);

	clear();
	mvprintw(letterY, letterX, "YOU WIN!");
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

//빙고인지 확인
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
			{
				if (board_number[i][j] == -1)
					diagL++;
			}

			if ((i + j) == 4)
			{
				if (board_number[i][j] == -1)
					diagR++;
			}
		}

		if (garo == MAX)
		{
			bingo++;
		}
	}

	for (j = 0; j < MAX; j++)
	{
		sero = 0;

		for (i = 0; i < MAX; i++)
			if (board_number[i][j] == -1) sero++;

		if (sero == 5)
		{
			bingo++;
		}
	}

	bingo += (diagL / 5) + (diagR / 5);

	return bingo;
}

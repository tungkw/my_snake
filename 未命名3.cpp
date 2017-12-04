#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
using namespace std;

#define high 20
#define wide 20 
#define game_speed 100
#define init_len 5 
#define food_point 1
#define pic_wall "จ~"
#define pic_snake "กั"
#define pic_food "ก๎"
#define pic_null "  "

#define up 72
#define down 80
#define left 75
#define right 77

class game{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		enum mode{
			unblock,block
		};
		struct pos{
			int x;
			int y;
			pos(int _x = 0,int _y = 0){
				x = _x;
				y = _y;
			}
		};
		void gotoxy(int x, int y){
			COORD pos;
			pos.X = x*2;
			pos.Y = y;
			SetConsoleCursorPosition(hOut, pos);
		}		
		void HideCursor(){
			CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		}
		const char *lose = "LOSE";
		const char *win = "WIN ";
		const char *_stop = "STOP";
		const char *_unstop = "    ";
		const char *_null = "จ~จ~จ~จ~จ~จ~จ~จ~จ~จ~" ;
		char map[high][wide];
		int dir;
		int score;
		int length;
		pos head,tail,food;
		mode md;
public:
	game(){	
		system("mode con cols=44");
		system("mode con lines=25");
		system("greedy snake");
		system("cls");
		HideCursor();
		md = unblock;
		score = 0;
		length = init_len;
		dir = right;
		make_map();
		make_snake();
		make_food();
		show_status();
	}
	void make_map(){
		for(int i = 0 ; i < high+2;i++){
			for(int j = 0 ; j < wide+2;j++){
				if(i == 0 || j == 0 || i == high+1 || j == wide+1) cout << pic_wall;
				else{
					map[i-1][j-1] = -2;
					cout << pic_null;
				}
			}
			cout << endl;
		}
	}
	void make_snake(){
		int _x = head.x = wide/2;
		int _y = head.y = high/2;
		for(int i = 0 ; i < length;i++){
			map[_y][_x-i] = right;
			gotoxy(_x-i+1,_y+1);
			cout << pic_snake;
		}
		tail.x = head.x - length + 1;
		tail.y = head.y;
	}
	void make_food(){
		srand(time(0));
		int _x;
		int _y;
		do{
			_x = rand()%wide;
			_y = rand()%high;
		}while(map[_y][_x]!=-2);
		food.x = _x;
		food.y = _y;
		gotoxy(_x+1,_y+1);
		cout << pic_food;
	}
	void show_status(){
		gotoxy(0,high+1+1);
		cout << "score:" << score << endl;
		cout << "length:" << length;
	}
	void start(){
		while(dir = getpress()){
			check_crash();
			check_eat();
		}
	}
	int getpress() {
		char _dir = dir;
		int _start = clock();
		while(clock() - _start < game_speed){
			char temp = 0;
			if(kbhit()) temp = getch();
			switch (temp) {
				case up:	if(dir!=down) _dir = up;break;
				case down:	if(dir!=up) _dir = down;break;
				case left:	if(dir!=right) _dir = left;break;
				case right:	if(dir!=left) _dir = right;break;
				case 32:
						{
							int save_t = clock()-_start;
							gotoxy(wide-strlen(_stop),high+3);
							cout << _stop;
							while(stop());
							gotoxy(wide-strlen(_unstop),high+3);
							cout << _unstop;
							_start = clock()+save_t;
							break;
						}
				case 20:	exit(0);
				default: break;
			}
		}
		return _dir;
	}
	int stop(){
		char temp;
		temp = getch();
		switch (temp) {
			case 32:	return 0;
			case 27:	exit(0);
			default:	return 1;
		}
	}
	void check_crash(){
		int _x = head.x;
		int _y = head.y;
		switch (dir) {
			case up:	_y=(_y-1+high)%high;break;
			case down:	_y=(_y+1)%high;break;
			case left:	_x=(_x-1+wide)%wide;break;
			case right:	_x=(_x+1)%wide;break;
			default:	break;
		}
		if(map[_y][_x] != -2){
			gotoxy(wide/2-strlen(lose)/2+1,high/2+1);
			show_ms(2);
		}
		else{
			map[head.y][head.x] = dir; 
			head.x = _x;
			head.y = _y;
			gotoxy(_x+1,_y+1);
			cout << pic_snake;
		}
	}
	void check_eat(){
		if(head.x == food.x && head.y == food.y){
			score+=food_point;
			length++;
			if(length == wide*high){
				gotoxy(wide/2-strlen(win)/2+1,high/2+1);
				show_ms(1);
			}
			show_status();
			make_food();
		}
		else{
			int temp = map[tail.y][tail.x];
			map[tail.y][tail.x] = -2;
			gotoxy(tail.x+1,tail.y+1);
			cout << pic_null;
			switch(temp){
				case up:	tail.y=(tail.y-1+high)%high;break;
				case down:	tail.y=(tail.y+1)%high;break;
				case left:	tail.x=(tail.x-1+wide)%wide;break;
				case right:	tail.x=(tail.x+1)%wide;break;
				default:	break;
			}
		}
	}
	void show_ms(int x){
		for(int i = 0 ;i < 5;i++){
			gotoxy(wide/2-4,high/2-2+i);
			cout << _null;
		}
		gotoxy(wide/2,high/2);
		if(x == 1) cout << win;
		else cout << lose;
		getch();
		exit(0);
	}
};

int main(){
	game test;
	test.start();
	return 0;
}

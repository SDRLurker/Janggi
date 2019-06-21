#include"Janggi.h"


/* 콘솔 관련 전역 함수 */
#define clrscr() system("cls")

void gotoxy(int x,int y) // 콘솔에서 콘솔 커서를 이동할 수 있다.
{
	COORD Cur = { (SHORT)x-1, (SHORT) y-1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void setcolor(int color,int bgcolor) // 콘솔에서 글자색과 글자 배경색을 설정가능하다.
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor<<4) | color);
}

void xyprintf(int x, int y, const char *format,...)
{
	char buf[1024];
	va_list ap;
	va_start(ap,format);
	gotoxy(x,y);
	vsprintf(buf,format,ap);
	printf("%s",buf);
	va_end(ap);
}
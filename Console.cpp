#include"Janggi.h"


/* �ܼ� ���� ���� �Լ� */
#define clrscr() system("cls")

void gotoxy(int x,int y) // �ֿܼ��� �ܼ� Ŀ���� �̵��� �� �ִ�.
{
	COORD Cur = { (SHORT)x-1, (SHORT) y-1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void setcolor(int color,int bgcolor) // �ֿܼ��� ���ڻ��� ���� ������ ���������ϴ�.
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
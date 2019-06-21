#include<stdio.h>
#include<conio.h>
#include<windows.h>

/* 
	**** 콘솔 관련 전역 함수 ****
*/

// 콘솔 출력 관련 함수
#define clrscr() system("cls")
void gotoxy(int x,int y); // 콘솔에서 콘솔 커서를 이동할 수 있다.
void setcolor(int color,int bgcolor); // 콘솔에서 글자색과 글자 배경색을 설정가능하다.
void xyprintf(int x, int y, const char *format,...);

// 콘솔 키보드 입력 키 값 정의
#define  KEYUP		0x48   // 키보드의 화살표 위 키에 대해 정의
#define  KEYDOWN	0x50   // 키보드의 화살표 아래 키에 대해 정의
#define  KEYLEFT	0x4b   // 키보드의 화살표 왼쪽 키에 대해 정의
#define  KEYRIGHT	0x4d   // 키보드의 화살표 오른쪽 키에 대해 정의

/* 
	**** 게임 국가와 게임판에서 빈 곳 Define 	**** 
*/
#define	NONE	0
#define	HAN		1
#define	CHO		2

/* 
	**** 게임을 초기화 하기 위한 자료  ****
*/

typedef struct _Location{
	int x;
	int y;
	TCHAR engname;
	TCHAR unitname[3];
	int country;
}Location;

#define CHAR_CHA	'C'
#define CHAR_SANG	'S'
#define CHAR_MA		'M'
#define CHAR_SA		's'
#define CHAR_KING	'K'
#define CHAR_PO		'P'
#define CHAR_ZOL	'Z'

//extern Location Units[];

#define THESAME			this->x = x;this->y = y;this->alive = TRUE;this->country = country;this->pan = game;this->unitname = unitname;

/* 
	**** 동적할당 배열을 위한 자료 구조 ****
*/

typedef struct _Coord{
	int x;
	int y;
	int who;
}Coord;

class MyList{
private:
	int size;
	Coord *elements;
public:
	MyList():size(0){	elements = NULL;	}
	~MyList(){	if(elements!=NULL)		free(elements);	}
	void add(int x,int y,int who);
	Coord* get(int i) const;
	int length() const{		return size;	}
};


/* 
	**** 유닛관련 클래스 ****
*/

class Game;
class Unit{
protected:
	int country;			// 한나라인가? 초나라인가?
	BOOL alive;				// 살아있음 - TRUE, 죽었음 - FALSE
	Game *pan;				// 게임판과 연결되어 있다.
	int x;					// 게임판에서 x좌표 위치
	int y;					// 게임판에서 y좌표 위치
	LPCTSTR unitname;		// 차, 포, 마, 상 등의 이름.
	BOOL AddList(int x,int y,MyList *current);		// MoveAvailable 함수가 내부적으로 사용하는 함수. 배열 원소 하나를 추가한다.
public:
	Unit(){}
	void Move(int x,int y){	this->x = x;	this->y = y;}	// 해당 x,y 좌표로 유닛(차, 포 등)이동시킨다.
	void setAlive(BOOL alive){	this->alive = alive;	}	
	virtual void MoveAvailable(MyList *current);			// 현재 유닛이 이동 가능한 위치를 MyList 배열로 알려준다.
	int getX() const{	return x;	}
	int getY() const{	return y;	}
	int getCountry() const{	return country;	}
	LPCTSTR getUnitName() const{	return unitname;	}	
};

class Cha:public Unit{
public:
	Cha(){}
	Cha(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}	
	virtual void MoveAvailable(MyList *current);
};

class Sang:public Unit{
public:
	Sang(){}
	Sang(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}
	virtual void MoveAvailable(MyList *current);
};

class Ma:public Unit{
public:
	Ma(){}
	Ma(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}
	virtual void MoveAvailable(MyList *current);
};

class Sa:public Unit{
public:
	Sa(){}
	Sa(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}
	virtual void MoveAvailable(MyList *current){		Unit::MoveAvailable(current);	}
};

class King:public Unit{
public:
	King(){}
	King(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}
	virtual void MoveAvailable(MyList *current){		Unit::MoveAvailable(current);	}
};

class Po:public Unit{
public:
	Po(){}
	Po(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}
	virtual void MoveAvailable(MyList *current);
};

class Zol:public Unit{
public:
	Zol(){}
	Zol(int x,int y,int country,Game* game,LPCTSTR unitname){		THESAME;	}	
	virtual void MoveAvailable(MyList *current);
};

/* 
	**** 게임관련 자료 구조 ****
*/

#define ONETEAMUNITS	16
#define	PANXSIZE		9
#define	PANYSIZE		10

// status, win변수 공용
#define NODECIDED		0
// status 변수
#define SELECTED		1
#define MOVESUCCESS		2
// win변수
#define HANWIN			1
#define CHOWIN			2
#define DRAWGAME		3

typedef struct _RollBack{
	int fromx,fromy;
	int tox,toy;
	unsigned turn;
	BOOL eat;
	//BOOL jang;
	//BOOL meong;
}RollBack;

class Game{
private:
	Unit *pan[PANXSIZE][PANYSIZE];	// NULL : 말 없음. 1 : 한, 2: 초(먼저 두고 밑에 있는 게 초라 가정.)
	Unit *ChoDead[ONETEAMUNITS];	// 초나라에서 죽은 유닛을 기억.
	Unit *HanDead[ONETEAMUNITS];	// 한나라에서 죽은 유닛을 기억.
	unsigned turn;					// 몇번째 턴인지 기억.

	BOOL jang;
	BOOL meong;
	int win;		// 누가 이겼나 판단하는 변수. 0 : 아무 상태 아님. 1,2 : 초,한나라 승리. 3: 무승부.
	RollBack rb;	// 무르기 정보 기억.

	int curx;		// 커서 x 위치
	int cury;		// 커서 y 위치

	int status;		// 현재 진행상태 관련 변수. 0 : 선택하기 전. 1 : 유닛을 선택. 2 : 이동한 직후의 상태.
	int selectx;	// 선택한 유닛의 x 위치
	int selecty;	// 선택한 유닛의 y 위치

	void Swap(int a,int b);
	void DrawBGBoard(int px,int py) const;
public:
	Game();
	~Game();
	int exist(int x,int y) const;
	BOOL isPo(int x,int y) const;
	int who() const{		return (turn+1)%2+1;	}	// 누구 차례인지 확인
	int Win() const{		return win;				}	
	
	void AskStartUnits();
	void SelectUnit(BOOL enter);
	BOOL MoveUnit();
	void CheckMeong();
	void CheckJang();
	void NextTurn();

	void Undo();

	// 콘솔 관리 함수들
	void PrintConsole()	const;
	void StatusConsole() const;
	void CursorMove(int direction);
};
#include<stdio.h>
#include<conio.h>
#include<windows.h>

/* 
	**** �ܼ� ���� ���� �Լ� ****
*/

// �ܼ� ��� ���� �Լ�
#define clrscr() system("cls")
void gotoxy(int x,int y); // �ֿܼ��� �ܼ� Ŀ���� �̵��� �� �ִ�.
void setcolor(int color,int bgcolor); // �ֿܼ��� ���ڻ��� ���� ������ ���������ϴ�.
void xyprintf(int x, int y, const char *format,...);

// �ܼ� Ű���� �Է� Ű �� ����
#define  KEYUP		0x48   // Ű������ ȭ��ǥ �� Ű�� ���� ����
#define  KEYDOWN	0x50   // Ű������ ȭ��ǥ �Ʒ� Ű�� ���� ����
#define  KEYLEFT	0x4b   // Ű������ ȭ��ǥ ���� Ű�� ���� ����
#define  KEYRIGHT	0x4d   // Ű������ ȭ��ǥ ������ Ű�� ���� ����

/* 
	**** ���� ������ �����ǿ��� �� �� Define 	**** 
*/
#define	NONE	0
#define	HAN		1
#define	CHO		2

/* 
	**** ������ �ʱ�ȭ �ϱ� ���� �ڷ�  ****
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
	**** �����Ҵ� �迭�� ���� �ڷ� ���� ****
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
	**** ���ְ��� Ŭ���� ****
*/

class Game;
class Unit{
protected:
	int country;			// �ѳ����ΰ�? �ʳ����ΰ�?
	BOOL alive;				// ������� - TRUE, �׾��� - FALSE
	Game *pan;				// �����ǰ� ����Ǿ� �ִ�.
	int x;					// �����ǿ��� x��ǥ ��ġ
	int y;					// �����ǿ��� y��ǥ ��ġ
	LPCTSTR unitname;		// ��, ��, ��, �� ���� �̸�.
	BOOL AddList(int x,int y,MyList *current);		// MoveAvailable �Լ��� ���������� ����ϴ� �Լ�. �迭 ���� �ϳ��� �߰��Ѵ�.
public:
	Unit(){}
	void Move(int x,int y){	this->x = x;	this->y = y;}	// �ش� x,y ��ǥ�� ����(��, �� ��)�̵���Ų��.
	void setAlive(BOOL alive){	this->alive = alive;	}	
	virtual void MoveAvailable(MyList *current);			// ���� ������ �̵� ������ ��ġ�� MyList �迭�� �˷��ش�.
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
	**** ���Ӱ��� �ڷ� ���� ****
*/

#define ONETEAMUNITS	16
#define	PANXSIZE		9
#define	PANYSIZE		10

// status, win���� ����
#define NODECIDED		0
// status ����
#define SELECTED		1
#define MOVESUCCESS		2
// win����
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
	Unit *pan[PANXSIZE][PANYSIZE];	// NULL : �� ����. 1 : ��, 2: ��(���� �ΰ� �ؿ� �ִ� �� �ʶ� ����.)
	Unit *ChoDead[ONETEAMUNITS];	// �ʳ��󿡼� ���� ������ ���.
	Unit *HanDead[ONETEAMUNITS];	// �ѳ��󿡼� ���� ������ ���.
	unsigned turn;					// ���° ������ ���.

	BOOL jang;
	BOOL meong;
	int win;		// ���� �̰峪 �Ǵ��ϴ� ����. 0 : �ƹ� ���� �ƴ�. 1,2 : ��,�ѳ��� �¸�. 3: ���º�.
	RollBack rb;	// ������ ���� ���.

	int curx;		// Ŀ�� x ��ġ
	int cury;		// Ŀ�� y ��ġ

	int status;		// ���� ������� ���� ����. 0 : �����ϱ� ��. 1 : ������ ����. 2 : �̵��� ������ ����.
	int selectx;	// ������ ������ x ��ġ
	int selecty;	// ������ ������ y ��ġ

	void Swap(int a,int b);
	void DrawBGBoard(int px,int py) const;
public:
	Game();
	~Game();
	int exist(int x,int y) const;
	BOOL isPo(int x,int y) const;
	int who() const{		return (turn+1)%2+1;	}	// ���� �������� Ȯ��
	int Win() const{		return win;				}	
	
	void AskStartUnits();
	void SelectUnit(BOOL enter);
	BOOL MoveUnit();
	void CheckMeong();
	void CheckJang();
	void NextTurn();

	void Undo();

	// �ܼ� ���� �Լ���
	void PrintConsole()	const;
	void StatusConsole() const;
	void CursorMove(int direction);
};
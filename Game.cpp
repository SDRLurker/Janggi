#include"Janggi.h"

// �ڸ� ��ġ�� ���� �ʱ�ȭ �ϴ� �κ�.
Location Units[] = {
	{0,0,CHAR_CHA,"�",HAN},{1,0,CHAR_SANG,"��",HAN},{2,0,CHAR_MA,"ة",HAN},{3,0,CHAR_SA,"��",HAN},		//1,2 ���� �ʿ�.
	{5,0,CHAR_SA,"��",HAN},{6,0,CHAR_MA,"ة",HAN},{7,0,CHAR_SANG,"��",HAN},{8,0,CHAR_CHA,"�",HAN},		//5,6 ���� �ʿ�.
	{4,1,CHAR_KING,"��",HAN},
	{1,2,CHAR_PO,"��",HAN},{7,2,'P',"��",HAN},
	{0,3,CHAR_ZOL,"��",HAN},{2,3,CHAR_ZOL,"��",HAN},{4,3,CHAR_ZOL,"��",HAN},{6,3,CHAR_ZOL,"��",HAN},{8,3,CHAR_ZOL,"��",HAN},
	{0,6,CHAR_ZOL,"��",CHO},{2,6,CHAR_ZOL,"��",CHO},{4,6,CHAR_ZOL,"��",CHO},{6,6,CHAR_ZOL,"��",CHO},{8,6,CHAR_ZOL,"��",CHO},
	{1,7,CHAR_PO,"��",CHO},{7,7,CHAR_PO,"��",CHO},
	{4,8,CHAR_KING,"��",CHO},
	{0,9,CHAR_CHA,"�",CHO},{1,9,CHAR_SANG,"��",CHO},{2,9,CHAR_MA,"ة",CHO},{3,9,CHAR_SA,"��",CHO},		//25,26 ���� �ʿ�.
	{5,9,CHAR_SA,"��",CHO},{6,9,CHAR_MA,"ة",CHO},{7,9,CHAR_SANG,"��",CHO},{8,9,CHAR_CHA,"�",CHO}		//29,30 ���� �ʿ�.
};

// ������� �׷��ֱ� ���� �迭
const char *PaintPan[] = {
	{"������������������"},
	{"������������������"},
	{"������������������"}
};

// ���Ӱ��� ���� �������� �ʱ�ȭ�Ѵ�.
Game::Game(){
	int x,y,unit_num=0,plus=0;
	int i;
	
	status = win = NODECIDED;
	turn = 0;
	curx=4;cury=8;
	selectx = selecty = 0;
	jang = meong = FALSE;
	
	int country=CHO;
	
	for(y=0;y<PANYSIZE;y++)
		for(x=0;x<PANXSIZE;x++)
			pan[x][y]=NULL;
		
	for(i=0;i<ONETEAMUNITS*2;i++){
		if(i<ONETEAMUNITS){
			ChoDead[i]=NULL;
			HanDead[i]=NULL;
		}
		x = Units[i].x;
		y = Units[i].y;
		if(pan[x][y]==NULL){
			switch(Units[i].engname){
			case CHAR_CHA:
				pan[x][y] = new Cha(x,y,Units[i].country,this,Units[i].unitname);	break;
			case CHAR_SANG:
				pan[x][y] = new Sang(x,y,Units[i].country,this,Units[i].unitname);	break;
			case CHAR_MA:
				pan[x][y] = new Ma(x,y,Units[i].country,this,Units[i].unitname);	break;
			case CHAR_SA:
				pan[x][y] = new Sa(x,y,Units[i].country,this,Units[i].unitname);	break;
			case CHAR_KING:
				pan[x][y] = new King(x,y,Units[i].country,this,Units[i].unitname);	break;
			case CHAR_PO:
				pan[x][y] = new Po(x,y,Units[i].country,this,Units[i].unitname);	break;
			case CHAR_ZOL:
				pan[x][y] = new Zol(x,y,Units[i].country,this,Units[i].unitname);	break;
			}
		}
	}
}

// ���Ӱ��� ���� �������� �����Ѵ�. (�ı���)
Game::~Game(){
	int x,y,i;
	for(y=0;y<PANYSIZE;y++)
		for(x=0;x<PANXSIZE;x++)
			if(pan[x][y]!=NULL)
				delete pan[x][y];
	for(i=0;i<ONETEAMUNITS;i++){
		if(ChoDead[i]!=NULL)
			delete ChoDead[i];
		if(HanDead[i]!=NULL)
			delete HanDead[i];
	}
}

// �ش� x,y ��ġ�� ������ �ֳ� Ȯ���Ѵ�.
int Game::exist(int x,int y) const{
	int ret = NONE;
	if(pan[x][y]!=NULL)
		ret = pan[x][y]->getCountry();
	return ret;
}

// �ش� x,y ��ġ�� ���� �ֳ� Ȯ���Ѵ�.
BOOL Game::isPo(int x,int y) const{
	BOOL ret = FALSE;
	if(pan[x][y]!=NULL && lstrcmp(pan[x][y]->getUnitName(),"��")==0)
		ret = TRUE;
	return ret;
}

// Ŀ�� ��ġ�� ������ ������ ���� ������ �����ش�. 
// enter : ����Ű�� ���� �����϶��� ���� ������ �����ش�.
void Game::SelectUnit(BOOL enter){	
	Coord *one;
	MyList cango;
	int j;

	if(enter){
		if(jang && meong){
			jang = meong = FALSE;
		}

		if(pan[curx][cury]!=NULL && who()==pan[curx][cury]->getCountry()){
			selectx = curx;
			selecty = cury;
			if(status==NODECIDED)
				status = SELECTED;
			else
				status = NODECIDED;
		}	
	}

	if(status==SELECTED){
		pan[selectx][selecty]->MoveAvailable(&cango);
		for(j=0;j<cango.length();j++){
			one = cango.get(j);
			if(curx==one->x && cury==one->y)	setcolor(0,3);
			else								setcolor(0,8);
			DrawBGBoard(one->x,one->y);
		}
	}
}

// ������ �̵���Ų��.
BOOL Game::MoveUnit(){
	BOOL ret = FALSE;
	BOOL enemy = FALSE;
	MyList cango;
	int i;
	Coord *one;
	Unit *Decided,*Enemy;

	if(status==SELECTED){
		Decided = pan[selectx][selecty];
		Decided->MoveAvailable(&cango);
		for(i=0;i<cango.length();i++){
			one = cango.get(i);
			if(curx==one->x && cury==one->y){
				if(one->who==NONE){
					if(selectx==one->x && selecty==one->y){
						ret = TRUE;
						break;
					}
					Decided->Move(curx,cury);
					pan[curx][cury] = Decided;
					pan[selectx][selecty]=NULL;
					ret = TRUE;
				}else	enemy = TRUE;
				break;
			}
		}
	}

	if(enemy){
		Enemy = pan[curx][cury];
		Enemy->setAlive(FALSE);
		Decided->Move(curx,cury);
		if(Enemy->getCountry()==HAN){
			for(i=0;HanDead[i]!=NULL;i++);
				HanDead[i] = Enemy;
		}else{
			for(i=0;ChoDead[i]!=NULL;i++);
				ChoDead[i] = Enemy;
		}
		pan[curx][cury] = Decided;
		pan[selectx][selecty]=NULL;
		ret = TRUE;
		
		if(who()==HAN && lstrcmp(ChoDead[i]->getUnitName(),"��")==0)
			win = HANWIN;
		if(who()==CHO && lstrcmp(HanDead[i]->getUnitName(),"��")==0)
			win = CHOWIN;
	}
			
	if(ret){
		rb.fromx = selectx;
		rb.fromy = selecty;
		rb.tox = curx;
		rb.toy = cury;
		rb.turn = turn;
		rb.eat = enemy;
		status = MOVESUCCESS;
	}

	return ret;
}

// �۱����� �˻��Ѵ�.
void Game::CheckMeong(){
	int i,ix,iy;
	MyList cango;
	Coord *one;
	if(status==MOVESUCCESS && jang){
		meong=TRUE;
		turn+=1;
		for(iy=0;iy<PANYSIZE;iy++)
			for(ix=0;ix<PANXSIZE;ix++)
				if(pan[ix][iy]!=NULL && pan[ix][iy]->getCountry()==who())
					pan[ix][iy]->MoveAvailable(&cango);
		
		for(i=0;i<cango.length();i++){
			one = cango.get(i);
			if(one->who!=NULL){
				if(who() == CHO && lstrcmp(pan[one->x][one->y]->getUnitName(),TEXT("��"))==0)
					meong = FALSE;
				if(who() == HAN && lstrcmp(pan[one->x][one->y]->getUnitName(),TEXT("��"))==0)
					meong = FALSE;
			}
		}
		turn-=1;
		if(!meong)	jang = false;
	}
}

// �屺���� �˻��Ѵ�.
void Game::CheckJang(){
	int i;
	MyList cango;
	Coord *one;

	if(status==MOVESUCCESS){
		pan[curx][cury]->MoveAvailable(&cango);
		for(i=0;i<cango.length();i++){
			one = cango.get(i);
			if(one->who!=NULL){
				if(who() == CHO && lstrcmp(pan[one->x][one->y]->getUnitName(),TEXT("��"))==0)
					jang = TRUE;
				if(who() == HAN && lstrcmp(pan[one->x][one->y]->getUnitName(),TEXT("��"))==0)
					jang = TRUE;
			}
		}
	}
}

// ���� �ѱ��.
void Game::NextTurn(){
	if(status==MOVESUCCESS){
		turn++;
		status = NODECIDED;
	}
}

// ������ ����� �����Ѵ�.
void Game::Undo(){
	int i;
	Unit *Decided, *Enemy;
	if(turn-1 == rb.turn){
		if(!(rb.fromx==rb.tox && rb.fromy==rb.toy)){
			Decided = pan[rb.tox][rb.toy];
			pan[rb.fromx][rb.fromy] = Decided;
			Decided->Move(rb.fromx,rb.fromy);
			pan[rb.tox][rb.toy] = NULL;
			if(rb.eat){
				if(who()==CHO){
					for(i=0;ChoDead[i]!=NULL;i++);
					Enemy = ChoDead[i-1];
					ChoDead[i-1]=NULL;
				}else{
					for(i=0;HanDead[i]!=NULL;i++);
					Enemy = HanDead[i-1];
					HanDead[i-1]=NULL;
				}
				Enemy->setAlive(TRUE);
				Enemy->Move(rb.tox,rb.toy);
				pan[rb.tox][rb.toy] = Enemy;
			}
		}
		turn--;
	}
}

void Game::Swap(int a, int b)
{
	Location temp;
	if(a>=0 && a<ONETEAMUNITS*2 && b>=0 && b<ONETEAMUNITS*2){
		temp = Units[a];
		Units[a] = Units[b];
		Units[b] = temp;
	}
}

// ����� ����� �׸���.
void Game::DrawBGBoard(int px,int py) const{
	if(py==0){
		if(px==0) xyprintf(px*2+1,py+1,"��");
		else if(px==PANXSIZE-1) xyprintf(px*2+1,py+1,"��");
		else xyprintf(px*2+1,py+1,"��");
	}else if(py==PANYSIZE-1){
		if(px==0) xyprintf(px*2+1,py+1,"��");
		else if(px==PANXSIZE-1) xyprintf(px*2+1,py+1,"��");
		else xyprintf(px*2+1,py+1,"��");
	}else{
		if(px==0) xyprintf(px*2+1,py+1,"��");
		else if(px==PANXSIZE-1) xyprintf(px*2+1,py+1,"��");
		else xyprintf(px*2+1,py+1,"��");
	}
}

// �������� �׸���.
void Game::PrintConsole() const{
	BOOL check=FALSE;
	int i,ix,iy;	
	setcolor(15,0);
	xyprintf(1,1,"%s",PaintPan[0]);
	for(i=2;i<10;i++)
		xyprintf(1,i,"%s",PaintPan[1]);
	xyprintf(1,10,"%s",PaintPan[2]);
	
	if(pan[curx][cury]!=NULL) 
		check=TRUE;
	if(!check){
		setcolor(0,15);
		DrawBGBoard(curx,cury);
	}
	
	for(iy=0;iy<PANYSIZE;iy++){
		for(ix=0;ix<PANXSIZE;ix++){
			if(pan[ix][iy]!=NULL){
				if(pan[ix][iy]->getCountry()==HAN){
					if(ix==curx && iy==cury)		setcolor(12,15);
					else			setcolor(12,0);
				}else{
					if(ix==curx && iy==cury)		setcolor(10,15);
					else			setcolor(10,0);
				}				
				xyprintf(ix*2+1,iy+1,"%s",pan[ix][iy]->getUnitName());
			}
		}
	}
	setcolor(7,0);
}

// ����â�� �׸���.
void Game::StatusConsole() const{
	int i;
	xyprintf(40,1,"���� ��ȣ : %d",turn);
	xyprintf(40,2,"���� : ");
	if(who()==HAN)	setcolor(12,0);
	else	setcolor(10,0);
	printf("��");
	setcolor(7,0);
	xyprintf(40,3,"Ŀ�� ��ġ : (%d, %d)",curx,cury);
	xyprintf(40,4,"���� ���� :     ");
	xyprintf(40,4,"���� ���� : ");
	if(status==SELECTED){
		if(pan[selectx][selecty]->getCountry()==HAN)	setcolor(12,0);
		else	setcolor(10,0);
		printf("%s",pan[selectx][selecty]->getUnitName());
	}
	setcolor(15,0);
	xyprintf(40,5,"�ʳ��󿡼� ���� ���ε�");
	gotoxy(40,6);setcolor(12,0);
	printf("                                ");
	gotoxy(40,6);
	for(i=0;HanDead[i]!=NULL && i<ONETEAMUNITS;i++)
		printf("%s",HanDead[i]->getUnitName());
	setcolor(15,0);
	xyprintf(40,7,"�ѳ��󿡼� ���� ���ε�");
	gotoxy(40,8);setcolor(10,0);
	printf("                                ");
	gotoxy(40,8);
	for(i=0;ChoDead[i]!=NULL && i<ONETEAMUNITS;i++)
		printf("%s",ChoDead[i]->getUnitName());
	xyprintf(40,12, "             ");
	if(jang && !meong){
		if(who()==HAN)	setcolor(10,0);	// �屺 üũ �� ���� �ٲ�Ƿ�... ������ �ݴ��.
		else	setcolor(12,0);
		xyprintf(40,12, "�屺�Դϴ�!!");
	}
	if(jang && meong){
		if(who()==HAN)	setcolor(10,0);	// �屺 üũ �� ���� �ٲ�Ƿ�... ������ �ݴ��.
		else	setcolor(12,0);
		xyprintf(40,12, "�۱��Դϴ�!!");
	}
	setcolor(7,0);
}

// ȭ��ǥ �̵��� ���� Ŀ�� ��ġ�� �ٲ۴�.
void Game::CursorMove(int direction){
	switch(direction){
	case KEYLEFT: curx = max(curx-1,0);	break;
	case KEYRIGHT: curx = min(curx+1,PANXSIZE-1); break;
	case KEYUP: cury = max(cury-1,0);	break;
	case KEYDOWN: cury = min(cury+1,PANYSIZE-1); break;
	}
}
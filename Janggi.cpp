#include"Janggi.h"

BOOL CheckWin(Game *game){
	TCHAR output[255];
	LPCTSTR HanWin = TEXT("�ѳ��� �̰���ϴ�\n�ٽ� �Ͻðڽ��ϱ�?");
	LPCTSTR ChoWin = TEXT("�ʳ��� �̰���ϴ�\n�ٽ� �Ͻðڽ��ϱ�?");
	switch(game->Win()){
	case HANWIN:
		wsprintf(output,"%s",HanWin);
		break;
	case CHOWIN:
		wsprintf(output,"%s",ChoWin);
		break;
	case NODECIDED:
		return FALSE;
	}
	if(MessageBox(NULL,output,TEXT("�¸� Ȯ��"),MB_YESNO)==IDYES){
		delete game;
		game = new Game();
		return TRUE;
	}else return FALSE;
}
int main(){
	int key;
	BOOL select = FALSE;
	BOOL ok = FALSE;
	BOOL ignoreEnter = FALSE;
	Game *game = new Game();
	while(game){
		game->PrintConsole();
		game->StatusConsole();
		game->SelectUnit(FALSE);
		key = getch();
		if(key==224){			// ����Ű�� ������ ���
			key = getch();		// � ����Ű�� �������� key�� ����.
			game->CursorMove(key);		// ����Ű�� �°� ���� ó��.
		}else if(key==13 && !ignoreEnter){		// ���� ����(��۱�true�� ���� �ʱ�ȭ) -> ���� ������ -> �۱� �˻� -> �屺 �˻� -> �� �ѱ�.
			game->SelectUnit(TRUE);
			game->MoveUnit();
			if(CheckWin(game))	continue;	else if(game->Win()!=NODECIDED)	ignoreEnter = TRUE;
			game->CheckMeong();			// �۰� �� ���� �ٲ��� �� ��~!
			game->CheckJang();
			game->NextTurn();			// ����� �̵��� ���¸� ���� ������ �ѱ��.
		}else if(key==27){		// ESCŰ�� ������ ���� ����
			break;
		}else if(key==8){		// backspace�� ������ ������ ��� ����
			game->Undo();
		}
	}
	delete game;
	game = NULL;
	return 0;
}

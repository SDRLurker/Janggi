#include"Janggi.h"

BOOL CheckWin(Game *game){
	TCHAR output[255];
	LPCTSTR HanWin = TEXT("한나라가 이겼습니다\n다시 하시겠습니까?");
	LPCTSTR ChoWin = TEXT("초나라가 이겼습니다\n다시 하시겠습니까?");
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
	if(MessageBox(NULL,output,TEXT("승리 확인"),MB_YESNO)==IDYES){
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
		if(key==224){			// 방향키를 눌렀을 경우
			key = getch();		// 어떤 방향키를 눌렀는지 key에 저장.
			game->CursorMove(key);		// 방향키에 맞게 게임 처리.
		}else if(key==13 && !ignoreEnter){		// 유닛 선택(장멍군true면 변수 초기화) -> 유닛 움직임 -> 멍군 검사 -> 장군 검사 -> 턴 넘김.
			game->SelectUnit(TRUE);
			game->MoveUnit();
			if(CheckWin(game))	continue;	else if(game->Win()!=NODECIDED)	ignoreEnter = TRUE;
			game->CheckMeong();			// 멍과 장 순서 바꾸지 말 것~!
			game->CheckJang();
			game->NextTurn();			// 제대로 이동한 상태면 다음 턴으로 넘긴다.
		}else if(key==27){		// ESC키를 누르면 게임 종료
			break;
		}else if(key==8){		// backspace를 누르면 무르기 기능 수행
			game->Undo();
		}
	}
	delete game;
	game = NULL;
	return 0;
}

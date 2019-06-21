#include"Janggi.h"

BOOL Unit::AddList(int x,int y,MyList *current){
	BOOL ret = TRUE;
	if(pan->exist(x,y)==NONE)	current->add(x,y,NONE);
	else if(pan->exist(x,y)!=pan->who()){		current->add(x,y,country);	ret=FALSE;	}
	else ret = FALSE;
	return ret;
}

// 사와 왕의 이동 가능한 위치를 다음 함수가 판단한다. (사와 왕은 이동 가능한 위치가 똑같으므로...)
void Unit::MoveAvailable(MyList *current){	
	static const POINT dir[] = {{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};
	static const POINT chk_dir[][3] = {
		{{1,0},{1,1},{0,1}}, {{1,0},{0,1},{-1,0}},
		{{-1,0},{-1,1},{0,1}}, {{0,-1},{-1,0},{0,1}},
		{{0,-1},{-1,-1},{-1,0}}, {{1,0},{0,-1},{-1,0}},
		{{0,-1},{1,-1},{1,0}}, {{0,-1},{1,0},{0,1}}
	};
	static const POINT center[] = {{0,0},{4,1},{4,8}};
	
	POINT result;
	int cur_chk;
	int i,j;		
	
	for(j=HAN;j<=CHO;j++){
		if(j==country){
			result.x = x - center[j].x;
			result.y = y - center[j].y;
		}
	}
	
	for(i=0;i<8;i++){
		if(dir[i].x == result.x && dir[i].y == result.y){
			cur_chk=i;
			break;
		}
		cur_chk=-1;		// 매치 되는 게 없다.
	}
	
	if(cur_chk==-1){
		for(i=0;i<8;i++)
			AddList(x + dir[i].x, y + dir[i].y,current);
		return;
	}
	
	for(j=0;j<3;j++)
		AddList(x + chk_dir[cur_chk][j].x, y + chk_dir[cur_chk][j].y,current);
}

// 차의 이동 가능한 위치를 다음 함수가 판단한다.
void Cha::MoveAvailable(MyList *current){
	int i,j;
	int ix,iy;
	int datas[][4] = {{x,max(y-1,0),0,-1},
	{x,min(y+1,PANYSIZE-1),0,1},
	{max(x-1,0),y,-1,0},
	{min(x+1,PANXSIZE-1),y,1,0}};
	
	for(i=0;i<4;i++){
		ix = datas[i][0];
		iy = datas[i][1];
		while(datas[i][2]?(ix>=0 && ix<PANXSIZE):(iy>=0 && iy<PANYSIZE)){
			if(!AddList(ix,iy,current))
				break;
			ix += datas[i][2];
			iy += datas[i][3];
		}
	}
	
	static const POINT dir[] = {{-1,-1},{1,-1},{1,1},{-1,1}};
	static const POINT chk_dir[][2] = {
		{{1,1},{2,2}}, {{-1,1},{-2,2}},
		{{-1,-1},{-2,-2}}, {{1,-1},{2,-2}}
	};
	static const POINT center[] = {{0,0},{4,1},{4,8}};
	
	POINT result;
	int cur_chk=0;
	
	for(i=HAN;i<=CHO;i++){
		result.x = x - center[i].x;
		result.y = y - center[i].y;
		if(result.x == 0 && result.y ==0){
			cur_chk=-1;
			break;
		}else if(result.x<-1 || result.x>1 || result.y<-1 || result.y>1)
			continue;			
		
		for(j=0;j<4;j++){
			if(dir[j].x == result.x && dir[j].y == result.y){
				if(AddList(x + chk_dir[j][0].x, y + chk_dir[j][0].y,current))
					AddList(x + chk_dir[j][1].x, y + chk_dir[j][1].y,current);
				cur_chk = j;
				break;
			}
		}
	}
	
	if(cur_chk==-1){
		for(i=0;i<4;i++)
			AddList(x + dir[i].x, y + dir[i].y,current);	
	}
}

// 상의 이동 가능한 위치를 다음 함수가 판단한다.
void Sang::MoveAvailable(MyList *current){	
	static const POINT move[][3] = {
		{{0,-1},{1,-1},{1,-1}}, {{0,-1},{-1,-1},{-1,-1}},
		{{1,0},{1,-1},{1,-1}}, {{1,0},{1,1},{1,1}},
		{{0,1},{1,1},{1,1}}, {{0,1},{-1,1},{-1,1}},
		{{-1,0},{-1,-1},{-1,-1}}, {{-1,0},{-1,1},{-1,1}}
	};
	int i,j;
	int cx,cy;
	for(i=0;i<8;i++){
		cx = x;
		cy = y;
		for(j=0;j<3;j++){
			cx = cx + move[i][j].x;
			cy = cy + move[i][j].y;
			if(cx>=PANXSIZE || cx<0 || cy>=PANYSIZE || cy<0)			break;
			if(j<2){
				if(pan->exist(cx,cy)!=NONE)			break;
			}else{
				AddList(cx, cy, current);
			}
		}
	}
}

// 마의 이동 가능한 위치를 다음 함수가 판단한다.
void Ma::MoveAvailable(MyList *current){
	static const POINT straight[] = {{1,0},{0,1},{-1,0},{0,-1}};
	static const int diagonal[] = {1,-1};
	
	int i,j;
	int cx,cy;
	
	for(i=0;i<4;i++){
		cx = x + straight[i].x;
		cy = y + straight[i].y;
		if(cx>=PANXSIZE || cx<0 || cy>=PANYSIZE || cy<0)			continue;
		if(pan->exist(cx,cy)!=NONE)			continue;
		cx = cx + straight[i].x;
		cy = cy + straight[i].y;
		if(cx>=PANXSIZE || cx<0 || cy>=PANYSIZE || cy<0)			continue;
		for(j=0;j<2;j++){
			if(i%2==0){
				if(cy + diagonal[j]<0 || cy + diagonal[j]>=PANYSIZE)		continue;
				AddList(cx, cy + diagonal[j],current);
			}else{
				if(cx + diagonal[j]<0 || cx + diagonal[j]>=PANXSIZE)		continue;
				AddList(cx + diagonal[j], cy, current);
			}
		}
	}
}

// 포의 이동 가능한 위치를 다음 함수가 판단한다.
void Po::MoveAvailable(MyList *current){
	int i,j;
	int ix,iy;
	BOOL pass;
	int datas[][4] = {{x,max(y-1,0),0,-1},
	{x,min(y+1,PANYSIZE-1),0,1},
	{max(x-1,0),y,-1,0},
	{min(x+1,PANXSIZE-1),y,1,0}};
	
	for(i=0;i<4;i++){
		pass=FALSE;
		ix = datas[i][0];
		iy = datas[i][1];
		while(datas[i][2]?(ix>=0 && ix<PANXSIZE):(iy>=0 && iy<PANYSIZE)){
			if(pan->isPo(ix,iy))	break;
			if(pass){			
				if(!AddList(ix,iy,current))
					break;
			}
			if(pan->exist(ix,iy)!=NONE)
				if(!pan->isPo(ix,iy))
					pass=TRUE;
				ix += datas[i][2];
				iy += datas[i][3];
		}
	}
	
	static const POINT dir[] = {{-1,-1},{1,-1},{1,1},{-1,1}};
	static const POINT chk_dir[] = {{2,2}, {-2,2}, {-2,-2}, {2,-2}};
	static const POINT center[] = {{0,0},{4,1},{4,8}};
	
	POINT result;
	
	for(i=HAN;i<=CHO;i++){
		result.x = x - center[i].x;
		result.y = y - center[i].y;
		
		if(result.x == 0 && result.y ==0)
			break;
		else if(result.x<-1 || result.x>1 || result.y<-1 || result.y>1)
			continue;
		
		if(pan->exist(center[i].x,center[i].y)!=NONE && !pan->isPo(center[i].x,center[i].y)){
			for(j=0;j<4;j++){
				if(dir[j].x == result.x && dir[j].y == result.y){
					AddList(x + chk_dir[j].x, y + chk_dir[j].y,current);
					break;
				}
			}
		}
	}
}

// 쫄의 이동 가능한 위치를 다음 함수가 판단한다.
void Zol::MoveAvailable(MyList *current){
	static const int checkx[] = {1,-1};
	int plusy=1;
	int i,j;
	if(pan->who()==CHO)
		plusy=-1;
	
	if(y + plusy >= 0 && y + plusy < PANYSIZE)
		AddList(x, y + plusy, current);
	
	for(j=0;j<2;j++){
		if(x + checkx[j]<0 || x + checkx[j]>=PANXSIZE)		continue;
		AddList(x + checkx[j], y, current);
	}
	
	POINT dir[] = {{-1,-plusy},{1,-plusy}};
	POINT chk_dir[] = {{1,plusy}, {-1,plusy}};
	static const POINT center[] = {{0,0},{4,1},{4,8}};
	static POINT result;
	int cur_chk;
	
	for(i=HAN;i<=CHO;i++){
		result.x = x - center[i].x;
		result.y = y - center[i].y;
		if(result.x == 0 && result.y == 0){
			cur_chk=-1;
			break;
		}else if(result.x<-1 || result.x>1 || result.y<-1 || result.y>1)
			continue;
		for(j=0;j<2;j++){
			if(dir[j].x == result.x && dir[j].y == result.y){
				AddList(x + chk_dir[j].x, y + chk_dir[j].y,current);
				cur_chk = j;
				break;
			}
		}
	}
	if(cur_chk==-1){
		for(i=0;i<2;i++)
			AddList(x + chk_dir[i].x, y + chk_dir[i].y,current);	
	}
}

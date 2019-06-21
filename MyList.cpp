#include"Janggi.h"

void MyList::add(int x,int y,int who)
{
	size++;
	if(size==1)	
		elements = (Coord*)malloc(sizeof(Coord)*size);
	else
		elements = (Coord*)realloc(elements,sizeof(Coord)*size);
	elements[size-1].x = x;
	elements[size-1].y = y;
	elements[size-1].who = who;
}

Coord* MyList::get(int i) const
{
	Coord* ret=NULL;
	if(i<size)
		ret = &elements[i];
	return ret;			
}
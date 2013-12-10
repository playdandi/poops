#include "Common.h"


float Common::ComputeX(float x)
{
	return floorf(x*OBJECT_WIDTH);
}

float Common::ComputeY(float y)
{
	//return CCDirector::sharedDirector()->getWinSize().height-floorf(y*OBJECT_HEIGHT);
    return floorf(y*OBJECT_HEIGHT)+OBJECT_HEIGHT*2;
}

CCPoint Common::ComputeXY(float x, float y)
{
	return ccp(ComputeX(x), ComputeY(y));
}

int Common::ComputeBoardX(float x)
{
	return (int)(x/floorf(OBJECT_WIDTH));
}

int Common::ComputeBoardY(float y)
{
    return (int)((y-OBJECT_HEIGHT*2)/floorf(OBJECT_HEIGHT));
}
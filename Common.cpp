#include "Common.h"


float Common::ComputeX(float x)
{
	return floorf(x*OBJECT_WIDTH);
}

float Common::ComputeY(float y)
{
	//return CCDirector::sharedDirector()->getWinSize().height-floorf(y*OBJECT_HEIGHT);
    return floorf(y*OBJECT_HEIGHT+25)+OBJECT_HEIGHT;
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
    return (int)((y-OBJECT_HEIGHT-25)/floorf(OBJECT_HEIGHT));
}
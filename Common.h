#ifndef PuzzleGame_Common
#define PuzzleGame_Common

#include "cocos2d.h"

USING_NS_CC;

#define DESIGN_WIDTH 768.0f
#define DESIGN_HEIGHT 1024.0f

#define START_ROW 0
#define ROW_COUNT 8
#define COLUMN_COUNT 8

#define TYPE_COUNT 6
#define TYPE_DIA_COUNT 3
#define TYPE_SP_COUNT 1

#define OBJECT_WIDTH 96
#define OBJECT_HEIGHT 96
#define DIAMOND_WIDTH 56
#define DIAMOND_HEIGHT 56

using namespace cocos2d;

int iCash;
int iGold;
int iRemainingHeartTime;
int iRemainingHeartNum;
int iRemainingObjectTime;
int iAge;
int iType;
float fWeight;
int iMaxScore;
std::string sUsername;


/*
static int iCash;
static int iGold;
static int iRemainingHeartTime;
static int iRemainingHeartNum;
static int iRemainingObjectTime;
static int iAge;
static int iType;
static float fWeight;
static int iMaxScore;
static std::string sUsername;

static void setCash(int cash)
{
    iCash = cash;
}
static int getCash()
{
    return iCash;
}
static void setGold(int gold)
{
    iGold = gold;
}
static int getGold()
{
    return iGold;
}
static void setHeartTime(int time)
{
    iRemainingHeartTime = time;
}
static int getHeartTime()
{
    return iRemainingHeartTime;
}
static void setHeartNum(int num)
{
    iRemainingHeartNum = num;
}
static int getHeartNum()
{
    return iRemainingHeartNum;
}
static void setObjectTime(int time)
{
    iRemainingObjectTime = time;
}
static int getObjectTime()
{
    return iRemainingObjectTime;
}
static void setAge(int age)
{
    iAge = age;
}
static int getAge()
{
    return iAge;
}
static void setType(int type)
{
    iType = type;
}
static int getType()
{
    return iType;
}
static void setWeight(float weight)
{
    fWeight = weight;
}
static float getWeight()
{
    return fWeight;
}
static void setMaxScore(int maxScore)
{
    iMaxScore = maxScore;
}
static int getMaxScore()
{
    return iMaxScore;
}
static void setUsername(std::string username)
{
    sUsername = username;
}
static std::string getUsername()
{
    return sUsername;
}
*/


class Common
{
public:
	static float ComputeX(float x);
	static float ComputeY(float y);
	static CCPoint ComputeXY(float x, float y);

	static int ComputeBoardX(float x);
	static int ComputeBoardY(float y);
    static bool XmlParseMoneyRaisePuzzle(char* data, int size, bool hasMoney);
};

#endif
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

#define LARGEST_MASS_LOWER_BOUND 7

using namespace cocos2d;

extern int iCash;
extern int iGold;
extern int iRemainingHeartTime;
extern int iRemainingHeartNum;
extern int iRemainingObjectTime;
extern int iAge;
extern int iType;
extern float fWeight;
extern int iMaxScore;
extern std::string sUsername;


class Common
{
public:
	static float ComputeX(float x);
	static float ComputeY(float y);
	static CCPoint ComputeXY(float x, float y);

	static int ComputeBoardX(float x);
	static int ComputeBoardY(float y);
    static bool XmlParseMoneyRaisePuzzle(char* data, int size, bool hasMoney);
    static bool XmlParsePuzzleDone(char* data, int size);
};

#endif
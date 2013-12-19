#include "Common.h"
#include "pugixml/pugixml.hpp"

using namespace pugi;


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


bool Common::XmlParseMoneyRaisePuzzle(char* data, int size, bool hasMoney)
{
    // xml parsing
    xml_document xmlDoc;
    xml_parse_result result = xmlDoc.load_buffer(data, size);
    
    if (!result)
    {
        CCLog("error description: %s", result.description());
        CCLog("error offset: %d", result.offset);
        return false;
    }
    
    // get several data
    xml_node nodeResult = xmlDoc.child("response");
    int code = nodeResult.child("code").text().as_int();
    //CCLog("code = %d", code);
    
    if (code == 0)
    {
        // http 통신 성공 - data를 모두 받는다.
        if (hasMoney)
        {
            iCash = nodeResult.child("money").attribute("cash").as_int();
            iGold = nodeResult.child("money").attribute("gold").as_int();
        }
        iAge = nodeResult.child("raise").attribute("age").as_int();
        iType = nodeResult.child("raise").attribute("type").as_int();
        fWeight = nodeResult.child("raise").attribute("weight").as_float();
        iMaxScore = nodeResult.child("puzzle").attribute("max-score").as_int();
        iRemainingHeartNum = nodeResult.child("puzzle").attribute("heart-num").as_int();
        iRemainingHeartTime = nodeResult.child("puzzle").attribute("heart-remain-time").as_int();
        /*
        if (hasMoney)
        {
            setCash(nodeResult.child("money").attribute("cash").as_int());
            setGold(nodeResult.child("money").attribute("gold").as_int());
        }
        setAge(nodeResult.child("raise").attribute("age").as_int());
        setType(nodeResult.child("raise").attribute("type").as_int());
        setWeight(nodeResult.child("raise").attribute("weight").as_float());
        setMaxScore(nodeResult.child("puzzle").attribute("max-score").as_int());
        setHeartNum(nodeResult.child("puzzle").attribute("heart-num").as_int());
        setHeartTime(nodeResult.child("puzzle").attribute("heart-remain-time").as_int());
        */
        
        return true;
    }
    else if (code == 3)
    {
        // no heart msg
        return false;
    }
    
    return false;
}
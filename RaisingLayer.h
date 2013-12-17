#ifndef __poops8__RaisingLayer__
#define __poops8__RaisingLayer__

#include "Common.h"
#include "Sound.h"


class RaisingLayer : public CCLayer
{
public:
	bool init();
    virtual void keyBackClicked();
    
	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    
	static CCScene* scene();
	CREATE_FUNC(RaisingLayer);
    
    void showMoney();
    void showHeart();
    void showObject();
    void showMenu();
    
    char* getTypeName(int type);
    
protected:
    Sound* sound;
    
    CCSprite* pCashMoneySprite;
    CCSprite* pGameMoneySprite;
    CCSprite* pHeartFullSprite;
    CCSprite* pHeartEmptySprite;
    CCLabelTTF* pHeartTimeLabel;
    
    CCSprite* pBackgroundSprite;
    CCSprite* pObjectSprite;
    CCSprite* pObjectInfoSprite;
    
    CCSprite* pMenuFeeding;
    CCSprite* pMenuRanking;
    CCSprite* pMenuStartPuzzle;
    
private:
    bool isTouchLocked;
    bool isObjectInfoSpriteShown;
    
    int iRemainingHeartTime;
    int iRemainingObjectTime;
    int iAge;
    int iType;
    double iWeight;
};

#endif /* defined(__poops8__RaisingLayer__) */

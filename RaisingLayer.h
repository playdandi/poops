#ifndef __poops8__RaisingLayer__
#define __poops8__RaisingLayer__

#include "Common.h"
#include "Sound.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;

class RaisingLayer : public CCLayerColor
{
public:
	bool init();
    virtual void keyBackClicked();
    
	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    
	//static CCScene* scene(std::vector<int> data, float weight);
    static CCScene* scene();
	CREATE_FUNC(RaisingLayer);
    
    void SetData();
    
    void showMoney();
    void showHeart();
    void showObject();
    void showMenu();
    
    std::string GetHeartTimeLabel();
    std::string getTypeName(int type);
    void HeartTimer();
    
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
protected:
    CCSize winSize;
    
    CCTexture2D* pResCashGold;
    CCTexture2D* pResHeart;
    CCTexture2D* pResTopBg;
    CCTexture2D* pResRaisingTag;
    CCTexture2D* pResBtns;
    CCTexture2D* pResObject;
    
    CCSprite* pTopBackgroundSprite;
    CCSprite* pCashSprite;
    CCLabelTTF* pCashLabel;
    CCSprite* pGoldSprite;
    CCLabelTTF* pGoldLabel;
    std::vector<CCSprite*> pHeartSprite;
    CCLabelTTF* pHeartTimeLabel;
    
    //CCSprite* pBackgroundSprite;
    CCSprite* pObjectSprite;
    CCSprite* pObjectInfoSprite;
    CCLabelTTF* pObjectInfoAge;
    CCLabelTTF* pObjectInfoType;
    CCLabelTTF* pObjectInfoWeight;
    
    CCSprite* pMenuFeeding;
    CCSprite* pMenuRanking;
    CCSprite* pMenuStartPuzzle;
    
private:
    bool isTouchLocked;
    bool isObjectInfoSpriteShown;
    /*
    int iRemainingHeartTime;
    int iRemainingHeartNum;
    
    int iRemainingObjectTime;
    int iAge;
    int iType;
    float iWeight;
    
    //int iCash;
    int iGold;
    int iMaxScore;
     */
};

#endif /* defined(__poops8__RaisingLayer__) */

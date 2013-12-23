#ifndef __poops8__GameEndLayer__
#define __poops8__GameEndLayer__

#include "Common.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class GameEndLayer : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    //static cocos2d::CCScene* scene(int score, int levelBonus, int typeBonus, float weight);
    static cocos2d::CCScene*scene(int score, int levelBonus, int typeBonus, int weight, std::vector<int> material);
    CREATE_FUNC(GameEndLayer);
    
    void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
    void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    CCSize winSize;
    CCSprite* pBackground;
    CCSprite* pPopup;
    
    CCLabelTTF* pYourScore;
    CCLabelTTF* pHighScore;
    CCLabelTTF* pScore;
    CCSprite* pScoreUnderline;
    
    CCLabelTTF* pLevelBonus;
    CCLabelTTF* pLevelBonusCapacity;
    CCLabelTTF* pTypeBonus;
    CCLabelTTF* pTypeBonusCapacity;
    CCLabelTTF* pAcquiredExp;
    CCLabelTTF* pAcquiredExpCapacity; // kg
    
    CCSprite* pIngredientBg1;
    CCSprite* pIngredientBg2;
    std::vector<CCSprite*> pPieces;
    std::vector<CCLabelTTF*> pAcquired;
    
    CCSprite* pConfirmBtn;
    
    void doClose(CCObject* pSender);
};

#endif /* defined(__poops8__GameEndLayer__) */

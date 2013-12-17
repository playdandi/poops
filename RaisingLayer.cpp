#include "RaisingLayer.h"
#include "GameLayer.h"

// replace scene
//CCScene* pPuzzleScene = GameLayer::scene();
//CCDirector::sharedDirector()->replaceScene(CCtransitionProgressRadialCW::create(0.5f, pPuzzleScene));
// 위에서 화면 transition 방법은 p.174 참조.

CCScene* RaisingLayer::scene()
{
	CCScene* pScene = CCScene::create();
    
	RaisingLayer* pLayer = RaisingLayer::create();
	pScene->addChild(pLayer);
    
	return pScene;
}

void RaisingLayer::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}


bool RaisingLayer::init()
{
	if (CCLayer::init() == false)
	{
		return false;
	}
    
    showMoney();
    showHeart();
    showObject();
    showMenu();
    isTouchLocked = false;
    
    // enabling keypad & touch
    this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
    
	return true;
}

void RaisingLayer::showMoney()
{
//    pCashMoneySprite->initWithTexture('cashmoney_preloadtexture2D',
//                                      CCRectMake(x위치, y위치, width, height));
    pCashMoneySprite->autorelease();
    
//    pGameMoneySprite->initWithTexture('gamemoney_preloadtexture2D',
//                                      CCRectMake(x위치, y위치, width, height));
    pGameMoneySprite->autorelease();
    
}

void RaisingLayer::showHeart()
{
    //    pHeartFullSprite->initWithTexture('heartfull_preloadtexture2D',
    //                                      CCRectMake(x위치, y위치, width, height));
    pHeartFullSprite->autorelease();

    //    pHeartEmptySprite->initWithTexture('heartempty_preloadtexture2D',
    //                                      CCRectMake(x위치, y위치, width, height));
    pHeartEmptySprite->autorelease();
    
    
    //pHeartTimeLabel = CCLabelTTF::create("", const char *fontName, float fontSize);
}

void RaisingLayer::showObject()
{
//    CCTexture2D* pBackgroundSprite;
//    CCTexture2D* pObjectSprite;
//    CCTexture2D* pObjectInfoSprite;
    
    char ctxAge[15];
    char ctxType[20];
    char ctxWeight[22];
    sprintf(ctxAge, "Age : %d", iAge);
    sprintf(ctxType, "Type : %s", getTypeName(iType));
    sprintf(ctxWeight, "Weight : %.3lf kg", iWeight);
    CCLabelTTF* pAgeLabel = CCLabelTTF::create(ctxAge, "Arial", 15);
    CCLabelTTF* pTypeLabel = CCLabelTTF::create(ctxType, "Arial", 15);
    CCLabelTTF* pWeightLabel = CCLabelTTF::create(ctxWeight, "Arial", 15);
    
    pAgeLabel->setPosition(ccp(50, 300));
    pTypeLabel->setPosition(ccp(50, 200));
    pWeightLabel->setPosition(ccp(50, 100));
    pObjectInfoSprite->addChild(pAgeLabel);
    pObjectInfoSprite->addChild(pTypeLabel);
    pObjectInfoSprite->addChild(pWeightLabel);
}

char* RaisingLayer::getTypeName(int type)
{
    char* name;
    switch (type)
    {
        case 0: sprintf(name, "A");
        case 1: sprintf(name, "B");
        case 2: sprintf(name, "C");
        case 3: sprintf(name, "D");
        case 4: sprintf(name, "E");
        case 5: sprintf(name, "F");
    }
    return name;
}

void RaisingLayer::showMenu()
{
//    CCSprite* pMenuFeeding;
//    CCSprite* pMenuRanking;
//    CCSprite* pMenuStartPuzzle;
}


void RaisingLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    if (isTouchLocked)
        return;
    isTouchLocked = true;
    
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
    // process scene
    if (pMenuStartPuzzle->boundingBox().containsPoint(point))
    {
        // start puzzle scene & transition of new scene from right to left.
        CCScene* pPuzzleScene = GameLayer::scene();
        CCDirector::sharedDirector()->pushScene(CCTransitionMoveInR::create(1.0f, pPuzzleScene));
    }
    else if (pObjectInfoSprite->boundingBox().containsPoint(point))
    {
        // show & hide object information.
        CCActionInterval* move = CCMoveBy::create(0.5f, ccp(400, 0));

        if (isObjectInfoSpriteShown)
        {
            // hide
            pObjectInfoSprite->runAction(move);
        }
        else
        {
            // show
            pObjectInfoSprite->runAction(move->reverse());
        }
        isObjectInfoSpriteShown = !isObjectInfoSpriteShown;
    }
}

void RaisingLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
    
}

void RaisingLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    
}





#include "GameEndLayer.h"

using namespace cocos2d;

CCScene* GameEndLayer::scene()
{
    CCScene* pScene = CCScene::create();
    GameEndLayer* pLayer = GameEndLayer::create();
    pScene->addChild(pLayer);
    
	return pScene;
}

bool GameEndLayer::init()
{
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    // layer (or layerColor) 의 기본 anchorPoint는 (0, 0)이며, set해도 바뀌지 않는다.
    // 바꾸는 방법이 있지만 [ignoreAnchorPointForPosition(false) 호출], 바꾸지 않는게 낫다.
    // cf) CCLayer's init method calls ignoreAnchorPointForPosition(true);.
    
    CCString* popParam = CCString::create("0");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("noti", popParam);
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    backLayer = CCLayerColor::create(ccc4(0, 0, 0, 150), winSize.width, winSize.height);
//    backLayer->setAnchorPoint(ccp(0, 0));
    backLayer->setPosition(ccp(0, 0));
    this->addChild(backLayer);
    
    popupLayer = CCLayerColor::create(ccc4(255, 255, 255, 255), 200, 200);
    popupLayer->setPosition(ccp(winSize.width/2, winSize.height/2));
//    popupLayer->setAnchorPoint(ccp(0.5f, 0.5f));
    this->addChild(popupLayer);
    
    
    
    return true;
}

void GameEndLayer::doClose(CCObject* pSender)
{
    CCString* popParam = CCString::create("1");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("noti", popParam);
    this->removeFromParentAndCleanup(true);
}



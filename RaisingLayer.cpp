#include "RaisingLayer.h"
#include "GameLayer.h"
#include <sstream>

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
	if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
	{
		return false;
	}
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    // init (load res)
    pResCashGold = CCTextureCache::sharedTextureCache()->addImage("images/cashgold.png");
    pResHeart = CCTextureCache::sharedTextureCache()->addImage("images/heart.png");
    pResTopBg = CCTextureCache::sharedTextureCache()->addImage("images/topbg.png");
    pResRaisingTag = CCTextureCache::sharedTextureCache()->addImage("images/raising_tag.png");
    pResBtns = CCTextureCache::sharedTextureCache()->addImage("images/raising_buttons.png");
    pResObject = CCTextureCache::sharedTextureCache()->addImage("images/main_character.png");
    
    SetData();
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


void RaisingLayer::SetData()
{
    /*
    iCash = viData[0];
    iGold = viData[1];
    iAge = viData[2];
    iType = viData[3];
    iMaxScore = viData[4];
    iRemainingHeartNum = viData[5];
    iWeight = fWeight;
     */
}

void RaisingLayer::showMoney()
{
    pTopBackgroundSprite = new CCSprite();
    pTopBackgroundSprite->initWithTexture(pResTopBg, CCRectMake(0, 0, 728, 150));
    pCashSprite = new CCSprite();
    pCashSprite->initWithTexture(pResCashGold, CCRectMake(0, 0, 60, 60));
    pGoldSprite = new CCSprite();
    pGoldSprite->initWithTexture(pResCashGold, CCRectMake(60, 0, 60, 60));
    
    char temp[8];
    sprintf(temp, "%d", iCash);
    pCashLabel = CCLabelTTF::create(temp, "Arial", 35);
    sprintf(temp, "%d", iGold);
    pGoldLabel = CCLabelTTF::create(temp, "Arial", 35);
    
    pTopBackgroundSprite->setPosition(ccp(winSize.width/2, winSize.height-85));
    pCashSprite->setAnchorPoint(ccp(0, 1));
    pCashSprite->setPosition(ccp(30, winSize.height-20));
    pGoldSprite->setAnchorPoint(ccp(0, 1));
    pGoldSprite->setPosition(ccp(30, winSize.height-90));
    pGoldLabel->setAnchorPoint(ccp(0, 0.5));
    pGoldLabel->setPosition(ccp(100, pGoldSprite->getContentSize().height/2));
    pGoldLabel->setColor(ccc3(0, 0, 0));
    pCashLabel->setAnchorPoint(ccp(0, 0.5));
    pCashLabel->setPosition(ccp(100, pCashSprite->getContentSize().height/2));
    pCashLabel->setColor(ccc3(0, 0, 0));
    
    addChild(pTopBackgroundSprite, 1);
    addChild(pCashSprite, 2);
    addChild(pGoldSprite, 2);
    pGoldSprite->addChild(pGoldLabel, 3);
    pCashSprite->addChild(pCashLabel, 3);
}

void RaisingLayer::showHeart()
{
    for (int i = 0 ; i < iRemainingHeartNum ; i++)
    {
        CCSprite* sprite = new CCSprite();
        sprite->initWithTexture(pResHeart, CCRectMake(0, 0, 60, 60));
        sprite->setAnchorPoint(ccp(0, 1));
        sprite->setPosition(ccp(350+80*i, winSize.height-20));
        pHeartSprite.push_back(sprite);
        addChild(sprite);
    }
    

    pHeartTimeLabel = CCLabelTTF::create(GetHeartTimeLabel().c_str(), "Arial", 35);
    pHeartTimeLabel->setAnchorPoint(ccp(1, 1));
    pHeartTimeLabel->setColor(ccc3(0, 0, 0));
    pHeartTimeLabel->setPosition(ccp(winSize.width-35, winSize.height-100));
    addChild(pHeartTimeLabel);
    
    if (iRemainingHeartNum < 5)
    {
        this->schedule(schedule_selector(RaisingLayer::HeartTimer), 1.0f);
    }
}

std::string RaisingLayer::GetHeartTimeLabel()
{
    std::string time;
    
    if (iRemainingHeartNum >= 5)
    {
        time = "FULL";
    }
    else
    {
        int min = iRemainingHeartTime / 60;
        int sec = iRemainingHeartTime % 60;
        std::ostringstream osm;
        osm << min;
        std::string sMin = osm.str();
        std::ostringstream oss;
        oss << sec;
        std::string sSec = oss.str();
        if (min < 10)
            sMin = "0" + sMin;
        if (sec < 10)
            sSec = "0" + sSec;
        
        time = sMin + " : " + sSec;
    }
    
    return time;
}

void RaisingLayer::HeartTimer(float f)
{
    iRemainingHeartTime--;
    if (iRemainingHeartTime < 0)
    {
        iRemainingHeartTime = 720;
        
        CCSprite* sprite = new CCSprite();
        sprite->initWithTexture(pResHeart, CCRectMake(0, 0, 60, 60));
        sprite->setAnchorPoint(ccp(0, 1));
        sprite->setPosition(ccp(350+80*iRemainingHeartNum, winSize.height-20));
        pHeartSprite.push_back(sprite);
        addChild(sprite);
        
        iRemainingHeartNum++;
        if (iRemainingHeartNum >= 5)
        {
            this->unschedule(schedule_selector(RaisingLayer::HeartTimer));
        }
    }
    
    pHeartTimeLabel->setString(GetHeartTimeLabel().c_str());
}

void RaisingLayer::showObject()
{
    pObjectSprite = new CCSprite();
    pObjectSprite->initWithTexture(pResObject, CCRectMake(0, 0, 300, 300));
    pObjectInfoSprite = new CCSprite();
    pObjectInfoSprite->initWithTexture(pResRaisingTag, CCRectMake(0, 0, 350, 201));
    
    pObjectSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    pObjectInfoSprite->setAnchorPoint(ccp(0, 0.5));
    pObjectInfoSprite->setPosition(ccp(winSize.width-30, winSize.height/2));
    
    char ctxAge[15];
    char ctxWeight[12];
    char ctxType[10];
    sprintf(ctxAge, "%d Age(s)", iAge);
    sprintf(ctxWeight, "%d.%d Kg", iWeight/1000, iWeight%1000);
    sprintf(ctxType, "%s Type", getTypeName(iType).c_str());
    CCLabelTTF* pAgeLabel = CCLabelTTF::create(ctxAge, "Arial", 35);
    CCLabelTTF* pTypeLabel = CCLabelTTF::create(ctxType, "Arial", 35);
    CCLabelTTF* pWeightLabel = CCLabelTTF::create(ctxWeight, "Arial", 35);
    
    pAgeLabel->setColor(ccc3(0, 0, 0));
    pTypeLabel->setColor(ccc3(0, 0, 0));
    pWeightLabel->setColor(ccc3(0, 0, 0));
    pAgeLabel->setAnchorPoint(ccp(0, 0));
    pTypeLabel->setAnchorPoint(ccp(0, 0));
    pWeightLabel->setAnchorPoint(ccp(0, 0));
    pAgeLabel->setPosition(ccp(100, 12*3+51*2));
    pTypeLabel->setPosition(ccp(100, 12*2+51*1));
    pWeightLabel->setPosition(ccp(100, 12));
    
    pObjectInfoSprite->addChild(pAgeLabel);
    pObjectInfoSprite->addChild(pTypeLabel);
    pObjectInfoSprite->addChild(pWeightLabel);
    addChild(pObjectSprite);
    addChild(pObjectInfoSprite);
}

std::string RaisingLayer::getTypeName(int type)
{
    switch (type)
    {
        case 0: return "AA";
        case 1: return "BB";
        case 2: return "CC";
        case 3: return "DD";
        case 4: return "EE";
        case 5: return "FF";
    }
    return "NONE";
}

void RaisingLayer::showMenu()
{
    pMenuFeeding = new CCSprite();
    pMenuFeeding->initWithTexture(pResBtns, CCRectMake(186*0, 0, 186, 120));
    pMenuRanking = new CCSprite();
    pMenuRanking->initWithTexture(pResBtns, CCRectMake(186*1, 0, 186, 120));
    pMenuStartPuzzle = new CCSprite();
    pMenuStartPuzzle->initWithTexture(pResBtns, CCRectMake(186*2, 0, 186, 120));
    
    pMenuFeeding->setAnchorPoint(ccp(0, 0));
    pMenuRanking->setAnchorPoint(ccp(0, 0));
    pMenuStartPuzzle->setAnchorPoint(ccp(0, 0));
    pMenuFeeding->setPosition(ccp(51, 51));
    pMenuRanking->setPosition(ccp(51*2+186*1, 51));
    pMenuStartPuzzle->setPosition(ccp(51*3+186*2, 51));
    
    addChild(pMenuFeeding);
    addChild(pMenuRanking);
    addChild(pMenuStartPuzzle);
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
        if (iRemainingHeartNum == 0)
        {
            // no heart msg;
        }
        else
        {
            // post request
            CCHttpRequest* req = new CCHttpRequest();
            req->setUrl("http://14.63.225.203/poops/game/puzzle_start.php");
            req->setRequestType(CCHttpRequest::kHttpPost);
            req->setResponseCallback(this, callfuncND_selector(RaisingLayer::onHttpRequestCompleted));
            //req->setResponseCallback(this, httpresponse_selector(RaisingLayer::onHttpRequestCompleted));
            // write data
            char postData[25];
            sprintf(postData, "user_name=%s", sUsername.c_str());
            req->setRequestData(postData, strlen(postData));
            CCHttpClient::getInstance()->send(req);
            req->release();
        }
    }
    else if (pObjectInfoSprite->boundingBox().containsPoint(point))
    {
        // show & hide object information.
        CCActionInterval* move = CCMoveBy::create(0.2f, ccp(320, 0));

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
    
    isTouchLocked = false;
}

void RaisingLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
    
}

void RaisingLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    
}


void RaisingLayer::onHttpRequestCompleted(CCNode *sender, void *data)
{
    CCHttpResponse* res = (CCHttpResponse*) data;
    
    if (!res)
        return;
    
    int statusCode = res->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "Http Status Code: %d, tag = %s", statusCode, res->getHttpRequest()->getTag());
    CCLog("response code : %d", statusCode);
    
    if (!res->isSucceed())
    {
        CCLog("res failed. error buffer: %s", res->getErrorBuffer());
        return;
    }
    
    // dump data
    std::vector<char> *buffer = res->getResponseData();
    char dumpData[buffer->size()+1];
    for (unsigned int i = 0 ; i < buffer->size() ; i++)
    {
        dumpData[i] = (*buffer)[i];
    }
    dumpData[buffer->size()] = NULL;
    //CCLog("%s", dumpData);
    //CCLog("=============================");
    
    
    // xml
    if (Common::XmlParseMoneyRaisePuzzle(dumpData, buffer->size(), false))
    {
        CCScene* pPuzzleScene = GameLayer::scene();
        //	CCDirector::sharedDirector()->setDepthTest(true);
        //CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(0.5f, pPuzzleScene));
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pPuzzleScene));
    }
}




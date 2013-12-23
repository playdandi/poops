#include "GameEndLayer.h"
#include "GameLayer.h"
#include "RaisingLayer.h"

using namespace cocos2d;

// global temporary variables
static int iScore;
static int iLevelBonus;
static int iTypeBonus;
//static float fAcquiredWeight;
static int iAcquiredWeight;
static std::vector<int> vMaterial;
bool isConfirming = false;


CCScene* GameEndLayer::scene(int score, int levelBonus, int typeBonus, int weight, std::vector<int> material)//float weight)
{
    iScore = score;
    iLevelBonus = levelBonus;
    iTypeBonus = typeBonus;
    iAcquiredWeight = weight;
    vMaterial = material;
    
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
    
    //CCString* popParam = CCString::create("0");
    //CCNotificationCenter::sharedNotificationCenter()->postNotification("noti", popParam);
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    // background sprite
    pBackground = CCSprite::create("images/progressbar.png", CCRectMake(0, 0, winSize.width, winSize.height));
    pBackground->setPosition(ccp(0, 0));
    pBackground->setAnchorPoint(ccp(0, 0));
    pBackground->setColor(ccc3(0, 0, 0));
    pBackground->setOpacity(150);
    addChild(pBackground);
    
    // popup sprite
    pPopup = CCSprite::create("images/progressbar.png", CCRectMake(0, 0, winSize.width-150, winSize.height-100));
    //pPopup->setPosition(ccp(winSize.width/2, 0));//-winSize.height/2));
    pPopup->setPosition(ccp(winSize.width/2, winSize.height/2));
    pPopup->setColor(ccc3(151, 223, 240));
    addChild(pPopup);
    
    CCActionInterval* action = CCMoveTo::create(0.5f, ccp(winSize.width/2, winSize.height/2));
    pPopup->runAction(action);
    
    int pWidth = pPopup->getContentSize().width;
    int pHeight = pPopup->getContentSize().height;
    
    pYourScore = CCLabelTTF::create("Your Score", "Arial", 30);
    pYourScore->setColor(ccc3(0, 0, 255));
    pYourScore->setRotation(-15);
    pYourScore->setAnchorPoint(ccp(0, 0));
    pYourScore->setPosition(ccp(30, pHeight-100));
    pPopup->addChild(pYourScore);
    
    if (iScore > iMaxScore)
    {
        pHighScore = CCLabelTTF::create("High Score!", "Arial", 30);
        pHighScore->setColor(ccc3(255, 0, 0));
        pHighScore->setRotation(-10);
        pHighScore->setAnchorPoint(ccp(0.5, 0));
        pHighScore->setPosition(ccp(pWidth/2, pHeight-70));
        pPopup->addChild(pHighScore);
    }
    
    char score[8];
    sprintf(score, "%d", iScore);
    pScore = CCLabelTTF::create(score, "Arial", 50);
    pScore->setAnchorPoint(ccp(0.5, 1.0));
    pScore->setColor(ccc3(0, 0, 0));
    pScore->setPosition(ccp(pWidth/2, pHeight-100+30));
    pPopup->addChild(pScore);
    
    pScoreUnderline = CCSprite::create("images/progressbar.png",
                                       CCRectMake(0, 0, pScore->getContentSize().width, 10));
    pScoreUnderline->setColor(ccc3(255, 0, 0));
    pScoreUnderline->setAnchorPoint(ccp(0, 0));
    pScoreUnderline->setPosition(ccp(0, -5));
    pScore->addChild(pScoreUnderline);
    
    
    pLevelBonus = CCLabelTTF::create("Level Bonus", "Arial", 30);
    pLevelBonus->setColor(ccc3(0, 0, 255));
    pLevelBonus->setRotation(-15);
    pLevelBonus->setAnchorPoint(ccp(0, 0));
    pLevelBonus->setPosition(ccp(30, pHeight-250));
    pPopup->addChild(pLevelBonus);
    
    char levelBonus[12];
    sprintf(levelBonus, "+ %d", 0);
    pLevelBonusCapacity = CCLabelTTF::create(levelBonus, "Arial", 40);
    pLevelBonusCapacity->setColor(ccc3(0, 0, 0));
    pLevelBonusCapacity->setAnchorPoint(ccp(0, 0));
    pLevelBonusCapacity->setPosition(ccp(30+pLevelBonus->getContentSize().width+30, pHeight-250));
    pPopup->addChild(pLevelBonusCapacity);

    pTypeBonus = CCLabelTTF::create("Type Bonus", "Arial", 30);
    pTypeBonus->setColor(ccc3(0, 0, 255));
    pTypeBonus->setRotation(-15);
    pTypeBonus->setAnchorPoint(ccp(0, 0));
    pTypeBonus->setPosition(ccp(30, pHeight-350));
    pPopup->addChild(pTypeBonus);
    
    char typeBonus[12];
    sprintf(typeBonus, "+ %d", 0);
    pTypeBonusCapacity = CCLabelTTF::create(typeBonus, "Arial", 40);
    pTypeBonusCapacity->setColor(ccc3(0, 0, 0));
    pTypeBonusCapacity->setAnchorPoint(ccp(0, 0));
    pTypeBonusCapacity->setPosition(ccp(30+pTypeBonus->getContentSize().width+30, pHeight-350));
    pPopup->addChild(pTypeBonusCapacity);
    
    pAcquiredExp = CCLabelTTF::create("Acquired Exp", "Arial", 30);
    pAcquiredExp->setColor(ccc3(0, 0, 255));
    pAcquiredExp->setRotation(-15);
    pAcquiredExp->setAnchorPoint(ccp(0, 0));
    pAcquiredExp->setPosition(ccp(30, pHeight-450));
    pPopup->addChild(pAcquiredExp);
    
    char acqExp[12];
    sprintf(acqExp, "+ %d.%d kg", iAcquiredWeight/1000, iAcquiredWeight%1000);
    pAcquiredExpCapacity = CCLabelTTF::create(acqExp, "Arial", 40);
    pAcquiredExpCapacity->setColor(ccc3(0, 0, 0));
    pAcquiredExpCapacity->setAnchorPoint(ccp(0, 0));
    pAcquiredExpCapacity->setPosition(ccp(30+pAcquiredExp->getContentSize().width+30, pHeight-450));
    pPopup->addChild(pAcquiredExpCapacity);
    
    pIngredientBg1 = CCSprite::create("images/progressbar.png", CCRectMake(0, 0, pWidth-60, 300));
    pIngredientBg1->setColor(ccc3(255, 0, 0));
    pIngredientBg1->setAnchorPoint(ccp(0, 1));
    pIngredientBg1->setPosition(ccp(30, pHeight-480));
    pPopup->addChild(pIngredientBg1);
    pIngredientBg2 = CCSprite::create("images/progressbar.png", CCRectMake(0, 0, pWidth-60-10, 300-10));
    pIngredientBg2->setColor(ccc3(255, 255, 255));
    pIngredientBg2->setAnchorPoint(ccp(0, 1));
    pIngredientBg2->setPosition(ccp(30+5, pHeight-480-5));
    pPopup->addChild(pIngredientBg2);
    
    CCSize bg2wh = pIngredientBg2->getContentSize();
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("images/all_pieces_score.png");
    std::vector<CCPoint> pos;
    pos.push_back(ccp(60, bg2wh.height-60));
    pos.push_back(ccp(bg2wh.width/2, bg2wh.height-60));
    pos.push_back(ccp(bg2wh.width-60, bg2wh.height-60));
    pos.push_back(ccp(60, 60));
    pos.push_back(ccp(bg2wh.width/2, 60));
    pos.push_back(ccp(bg2wh.width-60, 60));
    for (int i = 0 ; i < TYPE_COUNT ; i++)
    {
        CCSprite* piece = new CCSprite();
        piece->initWithTexture(texture, CCRectMake(i*OBJECT_WIDTH, 0, OBJECT_WIDTH, OBJECT_HEIGHT));
        piece->setPosition(pos[i]);
        piece->setOpacity(150);
        pIngredientBg2->addChild(piece);
        pPieces.push_back(piece);
        
        char materialNum[3];
        sprintf(materialNum, "%d", vMaterial[i]);
        CCLabelTTF* number = CCLabelTTF::create(materialNum, "Arial", 60);
        number->setPosition(ccp(piece->getContentSize().width/2, piece->getContentSize().height/2));
        number->setColor(ccc3(0, 0, 0));
        piece->addChild(number);
        pAcquired.push_back(number);
    }
    
    pConfirmBtn = CCSprite::create("images/progressbar.png", CCRectMake(0, 0, 200, 100));
    pConfirmBtn->setColor(ccc3(0, 0, 0));
    pConfirmBtn->setAnchorPoint(ccp(0.5, 0));
    pConfirmBtn->setPosition(ccp(pWidth/2, 20));
    pPopup->addChild(pConfirmBtn);
    
    setTouchEnabled(true);
    
    return true;
}


void GameEndLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
    if (pConfirmBtn->boundingBox().containsPoint(point))
    {
        isConfirming = true;
    }
}

void GameEndLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
    if (isConfirming && pConfirmBtn->boundingBox().containsPoint(point))
    {
        // post request
        CCHttpRequest* req = new CCHttpRequest();
        req->setUrl("http://14.63.225.203/poops/game/puzzle_end.php");
        req->setRequestType(CCHttpRequest::kHttpPost);
        req->setResponseCallback(this, callfuncND_selector(GameEndLayer::onHttpRequestCompleted));
        //req->setResponseCallback(this, httpresponse_selector(GameEndLayer::onHttpRequestCompleted));
        // write data
        char postData[1024];
        sprintf(postData, "user_name=%s&score=%d&weight=%d&material_a=%d&material_b=%d&material_c=%d&material_d=%d&material_e=%d&material_f=%d", sUsername.c_str(), iScore, iAcquiredWeight, vMaterial[0], vMaterial[1], vMaterial[2], vMaterial[3], vMaterial[4], vMaterial[5]);
        req->setRequestData(postData, strlen(postData));
        CCHttpClient::getInstance()->send(req);
        req->release();
    }
    
    isConfirming = false;
}

void GameEndLayer::onHttpRequestCompleted(CCNode *sender, void *data)
{
    //CCLog("http done");
    CCHttpResponse* res = (CCHttpResponse*) data;
    
    if (!res)
        return;
    
    int statusCode = res->getResponseCode();
    //char statusString[64] = {};
    //sprintf(statusString, "Http Status Code: %d, tag = %s", statusCode, res->getHttpRequest()->getTag());
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
    //CCLog("==================================");
    
    
    // xml
    if (Common::XmlParsePuzzleEnd(dumpData, buffer->size()))
    {
        // go to RaisingLayer scene
        CCScene* pNextScene = RaisingLayer::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pNextScene));
    }
}


void GameEndLayer::doClose(CCObject* pSender)
{
    CCString* popParam = CCString::create("1");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("noti", popParam);
    this->removeFromParentAndCleanup(true);
}





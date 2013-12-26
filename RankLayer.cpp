#include "RankLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;


CCScene* RankLayer::scene()
{    
    CCScene* pScene = CCScene::create();
    RankLayer* pLayer = RankLayer::create();
    pScene->addChild(pLayer);
    
	return pScene;
}

bool RankLayer::init()
{
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    winSize = CCDirector::sharedDirector()->getWinSize();

	pResRankBox = CCTextureCache::sharedTextureCache()->addImage("images/score_label.png");

	pResThema = CCTextureCache::sharedTextureCache()->addImage("images/rank_layout.png");
	pObjectThemaSprite = new CCSprite();
	pObjectThemaSprite->initWithTexture(pResThema, CCRectMake(0, 0, 705, 874));
	pObjectThemaSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(pObjectThemaSprite);

    setTouchEnabled(true);
    
	const int MAX_ITEMS = vScoreList.size();
	const int BASE_TAG = 100;
	const int HEIGHT = 100;

	pRankBoxList.clear();

	char menuName[50];
	int i;
    
	CCLayer* scrollContainer = CCLayer::create();
    
    scrollContainer->setPosition(ccp(winSize.width/2, winSize.height/2));
	
    int ssize = 15;
    for (i = 0 ; i < ssize ; i++)
    {
        pRankBoxList.push_back(new CCSprite());
        pRankBoxList[i]->initWithTexture(pResRankBox, CCRectMake(0, 0, 650, 100));
		pRankBoxList[i]->setAnchorPoint(ccp(0.5, 1));
        pRankBoxList[i]->setPosition(ccp(325, (ssize-i)*HEIGHT));
        scrollContainer->addChild(pRankBoxList[i]);
    }

	scrollContainer->setContentSize(CCSizeMake(705, ssize*100));
    
    CCScrollView* scrollView = CCScrollView::create();
    scrollView->retain();
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    scrollView->setViewSize(CCSizeMake(650, 874));
    scrollView->setContentSize(scrollContainer->getContentSize());
    scrollView->setAnchorPoint(ccp(0, 0));
    scrollView->setPosition(ccp((winSize.width/2)-325, (1024-874)/2));
    scrollView->setContainer(scrollContainer);
    scrollView->setDelegate(this);

    scrollView->setContentOffset(ccp(0, 874-(ssize*100)), false);
    addChild(scrollView);
    

	return true;
}

void RankLayer::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}

void RankLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
   // if (pConfirmBtn->boundingBox().containsPoint(point))
   // {
   //     isConfirming = true;
   // }
}

void RankLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    /*
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
	*/
}

void RankLayer::onHttpRequestCompleted(CCNode *sender, void *data)
{
	/*
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
    char dumpData[BUFFER_SIZE];
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
	*/
}

void RankLayer::doClose(CCObject* pSender)
{
    CCString* popParam = CCString::create("1");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("noti", popParam);
    this->removeFromParentAndCleanup(true);
}

void RankLayer::scrollViewDidScroll(CCScrollView* view)
{

}
void RankLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}
#include "RankLayer.h"

using namespace cocos2d;

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
    
    setTouchEnabled(true);
    
    /////////////////

	/////////////////////////////
	// 3. add your codes below...
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCMenu* menu = CCMenu::create();

	const int MAX_ITEMS = 20;
	const int BASE_TAG = 100;
	const int HEIGHT = 40;

	char menuName[50];
	int i;
	for (i = 0 ; i < vScoreList.size() ; i++)
	{
		snprintf(menuName, sizeof(menuName), "%s - %d", vScoreList[i].name, vScoreList[i].score);
		CCNode* menuLabel = CCLabelTTF::create(menuName, "Arial", 24);
		CCSize menuSize = menuLabel->getContentSize();

		CCMenuItem* menuItem = CCMenuItemLabel::create(menuLabel, this, menu_selector(RankLayer::menuItemCallback));
		menuItem->setTag(BASE_TAG+i);
		menuItem->setPosition(ccp(0, (MAX_ITEMS-i)*HEIGHT - winSize.height/2));

		menu->addChild(menuItem);
	}

	CCRect rect(0, -HEIGHT, 100, MAX_ITEMS * HEIGHT);


	// Setup scroll container
	CCLayer* scrollContainer = CCLayer::create();
	scrollContainer->addChild(menu);

	CCSize contentSize(rect.size.width-rect.origin.x, rect.size.height-rect.origin.y);
	scrollContainer->setContentSize(contentSize);


	// Setup scroll view
	extension::CCScrollView* scrollView = extension::CCScrollView::create(winSize, scrollContainer);
	scrollView->setDirection(extension::kCCScrollViewDirectionVertical);

	// Scroll to bottom
	scrollView->setContentOffset( CCPointZero, true );

	this->addChild(scrollView);


	return true;
}

void RankLayer::menuItemCallback(CCObject* sender)
{
	CCMenuItem* menuItem = (CCMenuItem*) sender;
	CCLog("menu %d", menuItem->getTag());
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
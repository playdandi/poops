#include "RankLayer.h"
#include "cocos-ext.h"
#include "RaisingLayer.h"

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
	pObjectThemaSprite->setPosition(ccp(winSize.width/2, winSize.height/2+50));
	this->addChild(pObjectThemaSprite);

	pResReturnButton = CCTextureCache::sharedTextureCache()->addImage("images/return.png");
	pObjectReturnButtonSprite = new CCSprite();
	pObjectReturnButtonSprite->initWithTexture(pResReturnButton, CCRectMake(0, 0, 300, 100));
	pObjectReturnButtonSprite->setPosition(ccp(winSize.width/2, 70));
	this->addChild(pObjectReturnButtonSprite);

    setTouchEnabled(true);
    
	const int MAX_ITEMS = vScoreList.size();
	const int BASE_TAG = 100;
	const int HEIGHT = 100;

	pRankBoxList.clear();

	int i;
    
	CCLayer* scrollContainer = CCLayer::create();    
    scrollContainer->setPosition(ccp(winSize.width/2, winSize.height/2+25));
	
	for (i = 0 ; i < MAX_ITEMS ; i++)
    {
		pRankBoxList.push_back(new CCSprite());
		pRankBoxList[i]->initWithTexture(pResRankBox, CCRectMake(0, 0, 650, 100));
		pRankBoxList[i]->setAnchorPoint(ccp(0.5, 1));
        pRankBoxList[i]->setPosition(ccp(325, (MAX_ITEMS-i)*HEIGHT));
		scrollContainer->addChild(pRankBoxList[i]);

		char labelMsg[30];

		sprintf(labelMsg, "   %2d", i+1);
        CCLabelTTF* labelRank = CCLabelTTF::create(labelMsg, "Arial", 42);
		labelRank->setColor(ccc3(0, 0, 0));
		labelRank->setAnchorPoint(ccp(0, 0.5));
		labelRank->setPosition(ccp(5, pRankBoxList[i]->getContentSize().height/2-10));
		pRankBoxList[i]->addChild(labelRank);

		sprintf(labelMsg, "%s", vScoreList[i].name);
        CCLabelTTF* labelName = CCLabelTTF::create(labelMsg, "Arial", 42);
		labelName->setColor(ccc3(0, 0, 0));
		labelName->setAnchorPoint(ccp(0, 0.5));
		labelName->setPosition(ccp(pRankBoxList[i]->getContentSize().width/2-200, pRankBoxList[i]->getContentSize().height/2-10));
		pRankBoxList[i]->addChild(labelName);

		sprintf(labelMsg, "%d", vScoreList[i].score);
        CCLabelTTF* labelScore = CCLabelTTF::create(labelMsg, "Arial", 40);
		labelScore->setColor(ccc3(200, 0, 0));
		labelScore->setAnchorPoint(ccp(1, 0.5));
		labelScore->setPosition(ccp(pRankBoxList[i]->getContentSize().width-75, pRankBoxList[i]->getContentSize().height/2-10));
		pRankBoxList[i]->addChild(labelScore);
    }

	scrollContainer->setContentSize(CCSizeMake(705, MAX_ITEMS*100));
    
    CCScrollView* scrollView = CCScrollView::create();
    scrollView->retain();
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    scrollView->setViewSize(CCSizeMake(650, 850));
    scrollView->setContentSize(scrollContainer->getContentSize());
    scrollView->setAnchorPoint(ccp(0, 0));
    scrollView->setPosition(ccp((winSize.width/2)-325, (1024-874)/2+12+50));
    scrollView->setContainer(scrollContainer);
    scrollView->setDelegate(this);

    scrollView->setContentOffset(ccp(0, 850-(MAX_ITEMS*100)), false);
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
    
   if (pObjectReturnButtonSprite->boundingBox().containsPoint(point))
   {
	   removeAllChildren();

	   CCScene* nextScene = RaisingLayer::scene();
	   //	CCDirector::sharedDirector()->setDepthTest(true);
	   CCScene* transition = CCTransitionFade::create(1.0f, nextScene);
	   CCDirector::sharedDirector()->replaceScene(transition);
    }
}

void RankLayer::scrollViewDidScroll(CCScrollView* view)
{

}

void RankLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}
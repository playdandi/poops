#include "Splash.h"
#include "RaisingLayer.h"
#include "GameLayer.h"
#include "pugixml/pugixml.hpp"

using namespace pugi;

bool isStarting = false;


CCScene* Splash::scene()
{
	CCScene* pScene = CCScene::create();
    
	Splash* pLayer = Splash::create();
	pScene->addChild(pLayer);
    
	return pScene;
}

void Splash::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}


bool Splash::init()
{
	if (CCLayer::init() == false)
	{
		return false;
	}
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    m_pLogo = CCSprite::create("images/logo.png", CCRectMake(0, 0, DESIGN_WIDTH, DESIGN_HEIGHT));
    m_pLogo->setAnchorPoint(ccp(0, 0));
    m_pLogo->setPosition(ccp(0, 0));
    m_pLogo->setOpacity(0);
    addChild(m_pLogo);
    CCActionInterval* action = CCSequence::create(CCFadeIn::create(0.7f),
                        CCCallFunc::create(this, callfunc_selector(Splash::LogoLoadingCompleted)), NULL);
    m_pLogo->runAction(action);


    // enabling keypad & touch
    this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
    
	return true;
}

void Splash::LogoLoadingCompleted()
{
    sUsername = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    
    if (sUsername == "")
    {
        /*
        m_pEditNameSprite = CCSprite::create("images/input_name.png");
        m_pEditNameSprite->setPosition(ccp(winSize.width/2, 284));
        addChild(m_pEditNameSprite);
        
        m_pEditName = CCTextFieldTTF::textFieldWithPlaceHolder("이름을 입력하세요(4~8자)",
                                        CCSize(384, 96), kCCTextAlignmentCenter, "Arial", 30);
        m_pEditName->setPosition(ccp(winSize.width/2, 284));
        addChild(m_pEditName);
        */
        
        // 이름이 저장되지 않은 경우 (처음 실행한 경우)
        m_pEditName = CCEditBox::create(CCSizeMake(384, 96), CCScale9Sprite::create("images/input_name.png"));
        m_pEditName->setPosition(ccp(winSize.width/2, 284));
        m_pEditName->setFont("Arial", 30);
        m_pEditName->setFontColor(ccBLACK);
        m_pEditName->setMaxLength(8);
        m_pEditName->setPlaceHolder("이름을 입력하세요 (4~8자)");
        m_pEditName->setReturnType(kKeyboardReturnTypeDone);
        m_pEditName->setDelegate(this);
        addChild(m_pEditName);
    }
    else
    {
        m_pNameLabel = CCLabelTTF::create((sUsername+"님 환영합니다.").c_str(), "Arial", 40);
        m_pNameLabel->setPosition(ccp(winSize.width/2, 284));
        m_pNameLabel->setColor(ccc3(0, 0, 0));
        addChild(m_pNameLabel);
    }
    
    m_pStartBtn = CCSprite::create("images/button_start.png", CCRectMake(0, 0, 384, 96));
    m_pStartBtn->setPosition(ccp(winSize.width/2, 168));
    addChild(m_pStartBtn);
}

/*
void Splash::keyboardWillShow(CCIMEKeyboardNotificationInfo &info)
{
    CCLog("keyboard show");
    m_pEditName->setString("");
}
void Splash::keyboardWillHide(CCIMEKeyboardNotificationInfo &info)
{
    CCLog("keyboard hide - %s", m_pEditName->getString());
}
*/
void Splash::editBoxEditingDidBegin(CCEditBox* editBox)
{
    //CCLog("%p did begin", editBox);
}
void Splash::editBoxEditingDidEnd(CCEditBox* editBox)
{
    //CCLog("%p did end", editBox);
}

void Splash::editBoxReturn(CCEditBox* editBox)
{
    std::string newUsername;
    newUsername.assign(editBox->getText());
    sUsername = newUsername;
}


void Splash::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
    if (m_pStartBtn->boundingBox().containsPoint(point))
    {
        isStarting = true;
        m_pStartBtn->setOpacity(120);
    }
    /*
    if (m_pEditName->boundingBox().containsPoint(point))
    {
        m_pEditName->attachWithIME();
    }
     */
}

void Splash::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
    
}

void Splash::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
    if (isStarting && m_pStartBtn->boundingBox().containsPoint(point))
    {
        //CCLog("username = %s", username.c_str());
        if (sUsername.size() >= 4 && sUsername.size() <= 8)
        {
            m_pLoadLabel = CCLabelTTF::create("Loading...", "Arial", 32);
            m_pLoadLabel->setPosition(ccp(winSize.width/2, 284+48+25));
            m_pLoadLabel->setColor(ccc3(0, 0, 0));
            addChild(m_pLoadLabel);
            
            // post request
            CCHttpRequest* req = new CCHttpRequest();
            req->setUrl("http://14.63.225.203/poops/game/login.php");
            req->setRequestType(CCHttpRequest::kHttpPost);
            req->setResponseCallback(this, callfuncND_selector(Splash::onHttpRequestCompleted));
            //req->setResponseCallback(this, httpresponse_selector(Splash::onHttpRequestCompleted));
            // write data
            char postData[25];
            sprintf(postData, "user_name=%s", sUsername.c_str());
            req->setRequestData(postData, strlen(postData));
            CCHttpClient::getInstance()->send(req);
            req->release();
        }
    }
 
    m_pStartBtn->setOpacity(255);
    isStarting = false;
}

void Splash::onHttpRequestCompleted(CCNode *sender, void *data)
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
    char dumpData[BUFFER_SIZE];
    for (unsigned int i = 0 ; i < buffer->size() ; i++)
    {
        dumpData[i] = (*buffer)[i];
    }
    dumpData[buffer->size()] = NULL;
    //CCLog("%s", dumpData);
    //CCLog("==================================");
    
    // save
    CCUserDefault::sharedUserDefault()->setStringForKey("username", sUsername);
 
    // xml
    if (Common::XmlParseMoneyRaisePuzzle(dumpData, buffer->size(), true))
    {
        GoToNextScene();
    }
}


//void Splash::GoToNextScene(std::vector<int> data, float weight)
void Splash::GoToNextScene()
{
    removeAllChildren();
    
    CCScene* nextScene = RaisingLayer::scene();
//	CCDirector::sharedDirector()->setDepthTest(true);
    CCScene* transition = CCTransitionFade::create(1.0f, nextScene);
	CCDirector::sharedDirector()->replaceScene(transition);
}
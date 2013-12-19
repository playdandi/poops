#ifndef __poops8__Splash__
#define __poops8__Splash__

#include "Common.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;

class Splash : public CCLayer,
                //public CCIMEDelegate
                public cocos2d::extension::CCEditBoxDelegate
{
public:
	virtual bool init();
    virtual void keyBackClicked();
    
	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    
	static CCScene* scene();
	CREATE_FUNC(Splash);
    
    void LogoLoadingCompleted();
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    //virtual void editBoxEditing(CCEditBox* editBox);
    virtual void editBoxReturn(CCEditBox* editBox);
//    void keyboardWillShow(CCIMEKeyboardNotificationInfo &info);
//    void keyboardWillHide(CCIMEKeyboardNotificationInfo &info);
    
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    //void GoToNextScene(std::vector<int> data, float weight);
    void GoToNextScene();
    
protected:
    CCSize winSize;
    
    CCSprite* m_pLogo;
    CCSprite* m_pStartBtn;
    CCLabelTTF* m_pNameLabel;
    
    //CCSprite* m_pEditNameSprite;
    //CCTextFieldTTF* m_pEditName;
    CCEditBox* m_pEditName;
    CCLabelTTF* m_pLoadLabel;
    
private:
//    std::string username;
};


#endif /* defined(__poops8__Splash__) */



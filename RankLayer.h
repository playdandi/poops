#ifndef __poops8__RankLayer__
#define __poops8__RankLayer__

#include "Common.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RankLayer : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    
    static cocos2d::CCScene*scene();
    CREATE_FUNC(RankLayer);
    
    void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
    void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    
    void onHttpRequestCompleted(CCNode *sender, void *data);
        
    void doClose(CCObject* pSender);

	void keyBackClicked();

private:
	CCSize winSize;
};

#endif /* defined(__poops8__GameEndLayer__) */

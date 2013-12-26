#ifndef __poops8__RankLayer__
#define __poops8__RankLayer__

#include "Common.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RankLayer : public cocos2d::CCLayerColor,
                    public CCScrollViewDelegate
{
public:
    virtual bool init();
    
    static cocos2d::CCScene*scene();
    CREATE_FUNC(RankLayer);
    
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);

    void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);

	void keyBackClicked();

protected:
	CCTexture2D* pResRankBox;
	std::vector<CCSprite*> pRankBoxList;

	CCTexture2D* pResThema;
	CCSprite* pObjectThemaSprite;

	CCTexture2D* pResReturnButton;
	CCSprite* pObjectReturnButtonSprite;

private:
	CCSize winSize;


};

#endif /* defined(__poops8__GameEndLayer__) */

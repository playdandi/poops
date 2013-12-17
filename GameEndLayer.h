#ifndef __poops8__GameEndLayer__
#define __poops8__GameEndLayer__

#include "cocos2d.h"

using namespace cocos2d;

class GameEndLayer : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameEndLayer);
    
    CCSize winSize;
    CCLayerColor* backLayer;
    CCLayerColor* popupLayer;
    
    void doClose(CCObject* pSender);
};

#endif /* defined(__poops8__GameEndLayer__) */

#include "GameObjectSP.h"
#include "GameLayer.h"

GameObjectSP::GameObjectSP()
: m_pGameLayer(NULL)
{
    
}

GameObjectSP::~GameObjectSP()
{
    
}


GameObjectSP* GameObjectSP::Create(GameLayer* gameLayer, int lu, int ru, int ld, int rd)
{
    GameObjectSP* pGameObjectSP = new GameObjectSP();
    pGameObjectSP->SetGameLayer(gameLayer);
    
    int r = rand()%100;
    int type = (r < 60) ? BLOCKED : CONNECTED; // 연결 diamond 확률은 40%
    if (type == CONNECTED)
    {
        int temp = rand()%100;
        type = (temp < 30) ? SPECIAL : CONNECTED; // item 확률은 연결 diamond중에서 30% (전체의 12%)
    }

    pGameObjectSP->SetType(type);
    
    if (type == SPECIAL) // special
    {
        int type_sp = rand()%TYPE_SP_COUNT+1;
        pGameObjectSP->SetTypeSP(type_sp);
        pGameObjectSP->CreateSpriteDia(gameLayer, lu, ru, ld, rd, type_sp);
    }
    else if (type == CONNECTED) // connected
    {
        pGameObjectSP->CreateSpriteDia(gameLayer, lu, ru, ld, rd, -1);
    }
    
	return pGameObjectSP;
}


void GameObjectSP::CreateSpriteDia(GameLayer* gameLayer, int lu, int ru, int ld, int rd, int type_sp)
{
    m_special = NULL;
    m_leftup = NULL;
    m_rightup = NULL;
    m_leftdown = NULL;
    m_rightdown = NULL;
    
    // special item은 대각선끼리 서로 달라 갈 수 없는 상황이라도 화면에 보여줘야 한다.
    if (type_sp != -1)
    {
        // special piece
        m_special = new CCSprite();
        m_special->initWithTexture(gameLayer->GetPuzzleSP(type_sp),
                                   CCRectMake(0, 0, DIAMOND_WIDTH, DIAMOND_HEIGHT));
        m_special->autorelease();
    }
    
    if (lu != rd && ru != ld)
        return;
    
    m_leftup = new CCSprite();
    m_rightdown = new CCSprite();
    m_rightup = new CCSprite();
    m_leftdown = new CCSprite();
    
    if (lu == rd && ru == ld)
    {
        m_leftup->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(lu*(DIAMOND_HEIGHT/2)+lu*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_rightdown->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(rd*(DIAMOND_HEIGHT/2)+rd*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_rightup->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(ru*(DIAMOND_HEIGHT/2)+ru*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_leftdown->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(ld*(DIAMOND_HEIGHT/2)+ld*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
    }
    
    else if (lu == rd)
    {
        m_leftup->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(lu*(DIAMOND_HEIGHT/2)+lu*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_rightdown->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(lu*(DIAMOND_HEIGHT/2)+lu*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_rightup->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(lu*(DIAMOND_HEIGHT/2)+lu*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_leftdown->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(lu*(DIAMOND_HEIGHT/2)+lu*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
    }
    
    else if (ru == ld)
    {
        m_leftup->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(ru*(DIAMOND_HEIGHT/2)+ru*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_rightdown->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(ru*(DIAMOND_HEIGHT/2)+ru*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_rightup->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(ru*(DIAMOND_HEIGHT/2)+ru*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
        m_leftdown->initWithTexture(gameLayer->GetPuzzleDia(),
                            CCRectMake(ru*(DIAMOND_HEIGHT/2)+ru*2+1, 0, DIAMOND_WIDTH/2, DIAMOND_HEIGHT/2));
    }
    
    m_leftup->autorelease();
    m_rightdown->autorelease();
    m_rightup->autorelease();
    m_leftdown->autorelease();
}

void GameObjectSP::SetPositions(int x, int y)
{
    if (m_type != BLOCKED)
    {
        if (m_leftup != NULL)
        {
            m_leftup->setAnchorPoint(ccp(0.0f, 0.0f));
            m_leftup->setRotation(-90);
            m_leftup->setPosition(Common::ComputeXY(x, y));
        }
        if (m_rightup != NULL)
        {
            m_rightup->setAnchorPoint(ccp(0.0f, 0.0f));
            m_rightup->setPosition(Common::ComputeXY(x, y));
        }
        if (m_leftdown != NULL)
        {
            m_leftdown->setAnchorPoint(ccp(0.0f, 0.0f));
            m_leftdown->setRotation(180);
            m_leftdown->setPosition(Common::ComputeXY(x, y));
        }
        if (m_rightdown != NULL)
        {
            m_rightdown->setAnchorPoint(ccp(0.0f, 0.0f));
            m_rightdown->setRotation(90);
            m_rightdown->setPosition(Common::ComputeXY(x, y));
        }
        
        if (m_type == SPECIAL && m_special != NULL)
        {
            m_special->setPosition(Common::ComputeXY(x, y));
        }
    }
}

void GameObjectSP::AddChildren(GameLayer* gameLayer, int zOrder)
{
    if (m_type != BLOCKED)
    {
        if (m_leftup != NULL)
            gameLayer->addChild(m_leftup, zOrder);
        if (m_rightup != NULL)
            gameLayer->addChild(m_rightup, zOrder);
        if (m_leftdown != NULL)
            gameLayer->addChild(m_leftdown, zOrder);
        if (m_rightdown != NULL)
            gameLayer->addChild(m_rightdown, zOrder);
        
        if (m_type == SPECIAL && m_special != NULL)
        {
            gameLayer->addChild(m_special, zOrder);
        }
    }
}

void GameObjectSP::AddChildrenWithAction(GameLayer* gameLayer, int zOrder)
{
    gameLayer->addChild(m_special, zOrder);
    
    m_special->setScale(0);
    CCActionInterval* scale = CCScaleTo::create(1.5f, 1.0);
    CCActionInterval* action = CCEaseElasticOut::create((CCActionInterval*)scale, 0.4f);
    m_special->runAction(action);
}

void GameObjectSP::RemoveChildren(bool isItemRemoved)
{    
    if (m_type != BLOCKED)
    {
        if (m_leftup != NULL)
            GetGameLayer()->removeChild(m_leftup, true);
        if (m_rightup != NULL)
            GetGameLayer()->removeChild(m_rightup, true);
        if (m_leftdown != NULL)
            GetGameLayer()->removeChild(m_leftdown, true);
        if (m_rightdown != NULL)
            GetGameLayer()->removeChild(m_rightdown, true);
        m_leftup = NULL;
        m_rightup = NULL;
        m_leftdown = NULL;
        m_rightdown = NULL;
        
        if (isItemRemoved && m_type == SPECIAL && m_special != NULL)
        {
            GetGameLayer()->removeChild(m_special, true);
            m_special = NULL;
        }
    }
}


int GameObjectSP::GetType()
{
	return m_type;
}

void GameObjectSP::SetType(int type)
{
	m_type = type;
}

int GameObjectSP::GetTypeSP()
{
    return m_type_sp;
}

void GameObjectSP::SetTypeSP(int type_sp)
{
    m_type_sp = type_sp;
}

int GameObjectSP::GetTargetBoardX()
{
	return m_targetBoardX;
}

void GameObjectSP::SetTargetBoardX(int x)
{
	m_targetBoardX = x;
}

int GameObjectSP::GetTargetBoardY()
{
	return m_targetBoardY;
}

void GameObjectSP::SetTargetBoardY(int y)
{
	m_targetBoardY = y;
}

void GameObjectSP::SetGameLayer(GameLayer* pGameLayer)
{
    m_pGameLayer = pGameLayer;
}

GameLayer* GameObjectSP::GetGameLayer()
{
    return m_pGameLayer;
}

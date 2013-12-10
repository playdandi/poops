#include "GameObject.h"
#include "GameLayer.h"

GameObject::GameObject()
: m_pGameLayer(NULL)
{

}

GameObject::~GameObject()
{

}


GameObject* GameObject::Create(int type, GameLayer* gameLayer)
{
    GameObject* pGameObject = new GameObject();
    pGameObject->SetGameLayer(gameLayer);
    
    pGameObject->initWithTexture(gameLayer->GetPuzzlePiece(),
                    CCRectMake(type*OBJECT_WIDTH, 0, OBJECT_WIDTH, OBJECT_HEIGHT));
    
    pGameObject->autorelease();
	pGameObject->SetType(type);

	return pGameObject;
}

int GameObject::GetType()
{
	return m_type;
}

void GameObject::SetType(int type)
{
	m_type = type;
}

int GameObject::GetTargetBoardX()
{
	return m_targetBoardX;
}

void GameObject::SetTargetBoardX(int x)
{
	m_targetBoardX = x;
}

int GameObject::GetTargetBoardY()
{
	return m_targetBoardY;
}

void GameObject::SetTargetBoardY(int y)
{
	m_targetBoardY = y;
}

void GameObject::SetGameLayer(GameLayer* pGameLayer)
{
	m_pGameLayer = pGameLayer;
}


void GameObject::ProcessFalling()
{
	CCPoint position = getPosition();

	m_prevBoardX = Common::ComputeBoardX(position.x);
    m_prevBoardY = Common::ComputeBoardY(position.y);
	//m_prevBoardY = MAX_ROW_COUNT-Common::ComputeBoardY(position.y);

	CCPoint targetPosition = Common::ComputeXY(m_targetBoardX, m_targetBoardY);
    
	//int fallingStepCount = -(m_targetBoardY-m_prevBoardY);

	CCMoveBy* pMoveBy = CCMoveBy::create(0.1f,//0.1f*fallingStepCount,
                                         ccp(0, targetPosition.y-position.y));

	CCFiniteTimeAction* pAction = CCSequence::create(pMoveBy,
                CCCallFunc::create(this, callfunc_selector(GameObject::FallingCompleteHandler)), NULL);

	this->runAction(pAction);
}

void GameObject::FallingCompleteHandler()
{
	int x1 = m_prevBoardX;
	int y1 = m_prevBoardY;
	int x2 = m_targetBoardX;
	int y2 = m_targetBoardY;

	m_pGameLayer->FallingFinished(x1, y1, x2, y2);
}

#include "GameLayer.h"

enum
{
	zBackground = 0,
	zGameObject = 1,
    zGameObjectSP = 2,
    zPieceConn = 3,
};

bool GameLayer::init()
{
	if (CCLayer::init() == false)
	{
		return false;
	}
    
    // enabling keypad
    this->setKeypadEnabled(true);
    
    // sound
    sound->PreLoadSound();
    
    // puzzle pieces & diamonds && specials
    pPuzzlePiece = CCTextureCache::sharedTextureCache()->addImage("images/all_pieces.png");
    pPuzzleDia = CCTextureCache::sharedTextureCache()->addImage("images/all_diamond_pieces.png");
    pPuzzleSP = CCTextureCache::sharedTextureCache()->addImage("images/one_piece_sp.png");
    
    // background
	pBackgroundSprite = CCSprite::create("images/background.png");
	pBackgroundSprite->setPosition(CCPointZero);
	pBackgroundSprite->setAnchorPoint(ccp(0.0f, 0.0f));
	addChild(pBackgroundSprite, zBackground);
    
    // refresh button
    pRefreshSprite = CCSprite::create("images/refresh.jpg");
    pRefreshSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, 64));
    pRefreshSprite->setScale(0.5f);
    addChild(pRefreshSprite, zBackground);
    
    sound->playBackgroundSound();

	m_winSize = CCDirector::sharedDirector()->getWinSize();

	StartGame();

	setTouchEnabled(true);

	return true;
}

CCScene* GameLayer::scene()
{
	CCScene* pScene = CCScene::create();

	GameLayer* pLayer = GameLayer::create();
	pScene->addChild(pLayer);

	return pScene;
}

void GameLayer::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}

void GameLayer::StartGame()
{
	srand(time(NULL));

	memset(m_pBoard, NULL, sizeof(m_pBoard));
	m_numOfSlidingObjects = 0;
	m_numOfFallingObjects = 0;
	
	for (int x = 0 ; x < COLUMN_COUNT ; x++)
	{
		for (int y = 0 ; y < ROW_COUNT ; y++)
		{
			GameObject* pGameObject = NULL;

            int type = rand()%TYPE_COUNT;

            if (pGameObject != NULL)
            {
                CC_SAFE_DELETE(pGameObject);
                m_pBoard[x][y] = NULL;
            }

            pGameObject = GameObject::Create(type, this);
			m_pBoard[x][y] = pGameObject;
            
			pGameObject->setAnchorPoint(ccp(0.0f, 0.0f));
            pGameObject->setPosition(Common::ComputeXY(x, y));
			pGameObject->SetTargetBoardX(x);
			pGameObject->SetTargetBoardY(y);

			addChild(pGameObject, zGameObject);
		}
	}
    
    
    // diamond 배치
    memset(m_pBoardSP, NULL, sizeof(m_pBoardSP)); // diamond pieces
    for (int x = 1; x < COLUMN_COUNT ; x++)
    {
        for (int y = 1 ; y < ROW_COUNT ; y++)
        {
            GameObjectSP* pGameObjectSP = NULL;

            CC_SAFE_DELETE(pGameObjectSP);
            m_pBoardSP[x][y] = NULL;
            
            pGameObjectSP = GameObjectSP::Create(this,
                                                 m_pBoard[x-1][y]->GetType(),
                                                 m_pBoard[x][y]->GetType(),
                                                 m_pBoard[x-1][y-1]->GetType(),
                                                 m_pBoard[x][y-1]->GetType());
			m_pBoardSP[x][y] = pGameObjectSP;
            
            pGameObjectSP->SetTargetBoardX(x);
			pGameObjectSP->SetTargetBoardY(y);
            pGameObjectSP->SetPositions(x, y);
			pGameObjectSP->AddChildren(this, zGameObjectSP);
        }
    }
    
    
	m_bTouchStarted = false;
    m_bIsBombing = false;
    //m_bTouchMoved = false;
    //m_bTouchEnded = false;
}


bool isValidPosition(int x, int y)
{
    return 0 <= x && x < COLUMN_COUNT && 0 <= y && y < ROW_COUNT;
}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    // CCLog("touch began");
    if (m_bIsBombing)
        return;
    
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();

    // refresh button
    if (pRefreshSprite->boundingBox().containsPoint(point))
    {
        for (int x = 0 ; x < COLUMN_COUNT ; x++)
        {
            for (int y = 0 ; y < ROW_COUNT ; y++)
            {
                removeChild(m_pBoard[x][y], true);
                m_pBoard[x][y] = NULL;
                if (x > 0 && y > 0)
                {
                    m_pBoardSP[x][y]->RemoveChildren();
                    m_pBoardSP[x][y] = NULL;
                }
            }
        }
        StartGame();
    }
    
	if (!m_bTouchStarted)
	{
		m_gestureStartBoardX = Common::ComputeBoardX(point.x);
		m_gestureStartBoardY = Common::ComputeBoardY(point.y);
        
        if (!isValidPosition(m_gestureStartBoardX, m_gestureStartBoardY))
            return;
		if (m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY] == NULL)
			return;
        
        // 벡터 초기화
        hanbut.clear();
        hanbut.push_back(ccp(m_gestureStartBoardX, m_gestureStartBoardY));
        connectDia.clear();
        connect.clear();
        special.clear();
        
        m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY]->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY]->setPosition(
		ccp(Common::ComputeX(m_gestureStartBoardX)+OBJECT_WIDTH/2, Common::ComputeY(m_gestureStartBoardY)+OBJECT_HEIGHT/2));
        
        CCActionInterval* action = CCScaleTo::create(0.05f, 0.9f);
        m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY]->runAction(action);

		m_bTouchStarted = true;
        sound->playTouchSound();
	}
}


void GameLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
    // CCLog("touch moved");
    if (m_bIsBombing)
        return;
    
	if (m_bTouchStarted)
	{
		CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
        CCPoint point = pTouch->getLocation();

		int boardX = Common::ComputeBoardX(point.x);
		int boardY = Common::ComputeBoardY(point.y);

        if (!isValidPosition(boardX, boardY))
            return;
		if (m_pBoard[boardX][boardY] == NULL)
            return;
        
        int beforeX = hanbut[hanbut.size()-1].x;
        int beforeY = hanbut[hanbut.size()-1].y;
        
        // 이전 조각과 인접하며 같은 type일 경우, 한붓그리기 대상에 추가한다.
        if ( (abs(beforeX-boardX) + abs(beforeY-boardY) == 1 ||
              (abs(beforeX-boardX) == 1 && abs(beforeY-boardY) == 1)) &&
            m_pBoard[beforeX][beforeY]->GetType() == m_pBoard[boardX][boardY]->GetType())
        {
            if (AlreadySelected(boardX, boardY))
                return;
            
            if (abs(beforeX-boardX) + abs(beforeY-boardY) == 2)
            {
                // diamond가 대각선 진행 불가 type이면 못 가게 한다.
                int X = (beforeX > boardX) ? beforeX : boardX;
                int Y = (beforeY > boardY) ? beforeY : boardY;
                if (m_pBoardSP[X][Y]->GetType() == BLOCKED)
                    return;
                
                CCSprite* conn = CCSprite::create("images/connect_diagonal.png");
                if (beforeX > boardX && beforeY < boardY) // 대각선 left-up 연결됨
                {
                }
                else if (beforeX < boardX && beforeY < boardY) // right-up
                {
                    conn->setRotation(90);
                }
                else if (beforeX > boardX && beforeY > boardY) // left-down
                {
                    conn->setRotation(90);
                }
                else // right-down
                {
                }

                conn->setPosition(Common::ComputeXY(X, Y));
                addChild(conn, zPieceConn);
                connect.push_back(conn);
                
                if (m_pBoardSP[X][Y]->GetType() == CONNECTED)
                    connectDia.push_back(ccp(X, Y));
                
                // 대각선의 diamond type이 special이면 vector에 추가.
                if (m_pBoardSP[X][Y]->GetType() == SPECIAL)
                    special.push_back(ccp(X, Y));
            }
            else if (abs(beforeX-boardX) == 1) // 가로로 연결됨
            {
                CCSprite* conn = CCSprite::create("images/connect.png");
                int X = (beforeX > boardX) ? beforeX : boardX;
                conn->setPosition(ccp(Common::ComputeX(X), Common::ComputeY(beforeY)+OBJECT_HEIGHT/2));
                addChild(conn, zPieceConn);
                connect.push_back(conn);
            }
            else // 세로로 연결됨
            {
                CCSprite* conn = CCSprite::create("images/connect.png");
                int Y = (beforeY > boardY) ? beforeY : boardY;
                conn->setRotation(90);
                conn->setPosition(ccp(Common::ComputeX(beforeX)+OBJECT_HEIGHT/2, Common::ComputeY(Y)));
                addChild(conn, zPieceConn);
                connect.push_back(conn);
            }
            
            hanbut.push_back(ccp(boardX, boardY));
            
            m_pBoard[boardX][boardY]->setAnchorPoint(ccp(0.5f, 0.5f));
            m_pBoard[boardX][boardY]->setPosition(
                ccp(Common::ComputeX(boardX)+OBJECT_WIDTH/2, Common::ComputeY(boardY)+OBJECT_HEIGHT/2));
            
            CCActionInterval* action = CCScaleTo::create(0.05f, 0.9f);
            m_pBoard[boardX][boardY]->runAction(action);
            
            sound->playTouchSound();
        }
	}
}

inline bool GameLayer::AlreadySelected(int x, int y)
{
    for (int i = 0 ; i < hanbut.size() ; i++)
    {
        if (hanbut[i].x == x && hanbut[i].y == y)
            return true;
    }
    return false;
}


void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    if (m_bIsBombing)
        return;
    
    if (m_bTouchStarted)
    {
        // 연결 그림들 제거한다.
        for (int i = 0 ; i < connect.size() ; i++)
        {
            removeChild(connect[i], true);
        }
        connect.clear();
    
        
        if (hanbut.size() >= 3)
        {
            // 3개 이상 한붓그리기 했으면 터뜨려야지!
            BombObject();
        }
        else
        {
            for (int i = 0 ; i < hanbut.size() ; i++)
            {
                int X = (int)hanbut[i].x;
                int Y = (int)hanbut[i].y;
                // back to original scale
                CCFiniteTimeAction* action =
                    CCSequence::create(CCScaleTo::create(0.05f, 1.0f),
                        CCCallFunc::create(this, callfunc_selector(GameLayer::TouchCallback)), NULL);
                m_pBoard[X][Y]->runAction(action);
            }
            
            m_bTouchStarted = false;
        }
    }
}

void GameLayer::TouchCallback()
{
    for (int i = 0 ; i < hanbut.size() ; i++)
    {
        int X = (int)hanbut[i].x;
        int Y = (int)hanbut[i].y;
        m_pBoard[X][Y]->setAnchorPoint(ccp(0.0f, 0.0f));
        m_pBoard[X][Y]->setPosition(
            ccp(Common::ComputeX(X), Common::ComputeY(Y)));
    }
    hanbut.clear();
    connectDia.clear();
}



void GameLayer::BombObject()
{
    //CCLog("BombObject");
    m_bIsBombing = true;
    m_callbackCnt = 0;

    // special diamond에 대한 처리
    std::vector<CCPoint> specialBomb;
    specialBomb.clear();
    for (int i = 0 ; i < special.size() ; i++)
    {
        int X = (int)special[i].x;
        int Y = (int)special[i].y;
        int type_sp = m_pBoardSP[X][Y]->GetTypeSP();
        
        if (type_sp == 0) // 4*4 bomb
        {
            int sx = (X-2 >= 0) ? X-2 : 0;
            int sy = (Y-2 >= 0) ? Y-2 : 0;
            int ex = (X+1 < COLUMN_COUNT) ? X+1 : COLUMN_COUNT-1;
            int ey = (Y+1 < ROW_COUNT) ? Y+1 : ROW_COUNT-1;
            for (int x = sx ; x <= ex ; x++)
            {
                for (int y = sy ; y <= ey ; y++)
                {
                    bool flag = true;
                    // 한붓그리기된 조각에 4*4 범위 내의 조각이 이미 포함되어 있으면 추가하지 마라.
                    for (int j = 0 ; j < hanbut.size() ; j++)
                    {
                        if (x == hanbut[j].x && y == hanbut[j].y)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (flag)
                        specialBomb.push_back(ccp(x, y));
                }
            }
            
            // special bomb 조각들을 hanbut에 추가해서 같이 터뜨린다.
            for (int i = 0 ; i < specialBomb.size() ; i++)
                hanbut.push_back(specialBomb[i]);
            specialBomb.clear();
        }
    }
    
    
    // 한붓그리기에서 대각선에 걸린 diamond들을 없앤다.
    for (int i = 0 ; i < connectDia.size() ; i++)
    {
        int x = connectDia[i].x;
        int y = connectDia[i].y;
        m_pBoardSP[x][y]->RemoveChildren();
    }
    connectDia.clear();
    
    // bomb action start.
    for (int i = 0 ; i < hanbut.size() ; i++)
    {
        int boardX = (int)hanbut[i].x;
        int boardY = (int)hanbut[i].y;
        
        GameObject* pGameObject = m_pBoard[boardX][boardY];
        if (pGameObject)
		{
            CCFiniteTimeAction* action = CCSequence::create(
                    CCSpawn::create(CCScaleTo::create(0.15f, 1.5f), CCFadeOut::create(0.15f), NULL),
                    CCCallFunc::create(this, callfunc_selector(GameLayer::BombCallback)),
                    NULL);
            pGameObject->runAction(action);
        }
    }
    
    sound->playBombSound();
}
                                                            
void GameLayer::BombCallback()
{
    m_callbackCnt++;
    
    if (m_callbackCnt == hanbut.size())
    {
        // bomb action이 끝나면 한붓그리기 상의 모든 조각을 제거한다.
        for (int i = 0 ; i < hanbut.size() ; i++)
        {
            GameObject* pGameObject = m_pBoard[(int)hanbut[i].x][(int)hanbut[i].y];
            // remove from layer, and free memories.
			removeChild(pGameObject, true);
   			m_pBoard[(int)hanbut[i].x][(int)hanbut[i].y] = NULL;
        }
        hanbut.clear();
        
        
        ProcessFalling();
    }
}


void GameLayer::ProcessFalling()
{
	m_numOfFallingObjects = 0;
    
	for (int x = 0 ; x < COLUMN_COUNT ; x++)
	{
		int fallingStep = 0;
        
        for (int y = 0 ; y < ROW_COUNT ; y++)
		{
			GameObject* pGameObject = m_pBoard[x][y];
			if (pGameObject == NULL)
			{
				int k = 0;
				GameObject* pToBeFallingObject = NULL;
                
                for (k = y ; k < ROW_COUNT ; k++)
				{
					pToBeFallingObject = m_pBoard[x][k];
					if (pToBeFallingObject != NULL)
						break;
				}
                
				if (pToBeFallingObject != NULL)
				{
					m_pBoard[x][k]->SetTargetBoardX(x);
					m_pBoard[x][k]->SetTargetBoardY(y);
                    
					m_pBoard[x][k]->ProcessFalling();
                    
					m_pBoard[x][y] = m_pBoard[x][k];
					m_pBoard[x][k] = NULL;
                    
					m_numOfFallingObjects++;
				}
				else
				{
					int targetY = ROW_COUNT;
					
					int type = rand()%TYPE_COUNT;
                    
					pToBeFallingObject = GameObject::Create(type, this);
                    
					pToBeFallingObject->SetTargetBoardX(x);
					pToBeFallingObject->SetTargetBoardY(y);
                    
					pToBeFallingObject->setAnchorPoint(ccp(0.0f, 0.0f));
					pToBeFallingObject->setPosition(Common::ComputeXY(x, targetY+fallingStep));
                    
					addChild(pToBeFallingObject, zGameObject);
                    
					pToBeFallingObject->ProcessFalling();
                    
					m_pBoard[x][y] = pToBeFallingObject;
                    
					++fallingStep;
                    
					m_numOfFallingObjects++;
				}
			}
		}
	}
}

void GameLayer::FallingFinished(int x1, int y1, int x2, int y2)
{
	m_numOfFallingObjects--;
    
	if (m_numOfFallingObjects == 0)
	{
		for (int x = 1 ; x < COLUMN_COUNT ; x++)
        {
            for (int y = 1 ; y < ROW_COUNT ; y++)
            {
                if (m_pBoardSP[x][y]->GetType() == CONNECTED)
                {
                    m_pBoardSP[x][y]->RemoveChildren();

                    m_pBoardSP[x][y]->CreateSpriteDia(this,
                                                    m_pBoard[x-1][y]->GetType(),
                                                    m_pBoard[x][y]->GetType(),
                                                    m_pBoard[x-1][y-1]->GetType(),
                                                    m_pBoard[x][y-1]->GetType());
                    m_pBoardSP[x][y]->SetPositions(x, y);
                    m_pBoardSP[x][y]->AddChildren(this, zGameObjectSP);
                }
            }
        }
        
        m_bTouchStarted = false;
        m_bIsBombing = false;
        //m_bTouchEnded = false;
	}
}


CCTexture2D* GameLayer::GetPuzzlePiece()
{
    return pPuzzlePiece;
}

CCTexture2D* GameLayer::GetPuzzleDia()
{
    return pPuzzleDia;
}

CCTexture2D* GameLayer::GetPuzzleSP()
{
    return pPuzzleSP;
}

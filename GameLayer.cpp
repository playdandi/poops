#include "GameLayer.h"
#include "GameEndLayer.h"

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
    
    isFinished = false;
    
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
    
    sound->playBackgroundSound();

	m_winSize = CCDirector::sharedDirector()->getWinSize();

    // init values.
    iScore = 0;
    iLevelBonus = 0;
    iTypeBonus = 0;
    iAcquiredWeight = 0;
    for (int i = 0 ; i < TYPE_COUNT ; i++)
        vMaterial.push_back(0);
    
    // score & score label
    scoreLabel = CCLabelTTF::create("0", "Arial", 80);
    scoreLabel->setAnchorPoint(ccp(1.0f, 0.5f));
    scoreLabel->setPosition(ccp(m_winSize.width-20, m_winSize.height-50));
    addChild(scoreLabel);
    
    // puzzle board
	StartGame();

    FindLargestMass();
    
    // timer progressbar && puzzletime label
    CCSprite* pBar = CCSprite::create("images/progressbar.png", CCRectMake(0, 0, OBJECT_WIDTH*6, 30));
    pBar->setColor(ccc3(0, 0, 255));
    progressTimer = CCProgressTimer::create(pBar);
    progressTimer->setType(kCCProgressTimerTypeBar);
    progressTimer->setMidpoint(ccp(0, 0));
    progressTimer->setBarChangeRate(ccp(1, 0));
    progressTimer->setPercentage(100.0f);
    progressTimer->setAnchorPoint(ccp(0.0f, 1.0f));
    progressTimer->setPosition(ccp(0, OBJECT_HEIGHT));
    addChild(progressTimer);
    
    puzzleTime = CCLabelTTF::create("30", "Arial", 80);
    puzzleTime->setAnchorPoint(ccp(1.0f, 1.0f));
    puzzleTime->setPosition(ccp(m_winSize.width-OBJECT_WIDTH, OBJECT_HEIGHT+20));
    puzzleTime->setColor(ccc3(0, 0, 0));
    addChild(puzzleTime);
    
   	setTouchEnabled(false);
    
    readyTime = CCLabelTTF::create("3", "Arial", 60);
    readyTime->setPosition(ccp(m_winSize.width/2, m_winSize.height+100));
    addChild(readyTime);
    
    iStartTimer = 4;
    iRemainingPuzzleTime = 60.0f;
    this->schedule(schedule_selector(GameLayer::ReadyTimer), 0.5f);
    
    comboLabel = CCLabelTTF::create("test", "Arial", 100);
    comboLabel->setOpacity(0);
    comboLabel->setPosition(ccp(m_winSize.width/2, m_winSize.height/2));
    comboLabel->setRotation(-15);
    addChild(comboLabel, 1000);
    
    iPassedComboTime = 0;
    iNumOfCombo = 0;
    
    // set notification
    // 'noti' 라는 메시지가 오면 해당 함수(doNotification)를 실행한다.
    //CCNotificationCenter::sharedNotificationCenter()->addObserver(
    //                            this, callfuncO_selector(GameLayer::doNotification), "noti", NULL);
    
	return true;
}

/*
void GameLayer::doNotification(CCObject* obj)
{
    //노티피케이션 받기
	CCString *pParam=(CCString*)obj;
	CCLog("notification %s", pParam->getCString());
	
	if (pParam->intValue() == 1)
    {
		CCLog("noti 11");
		CCDirector::sharedDirector()->resume();   //화면 재시작
        
		//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pMenu, kCCMenuHandlerPriority,true);
        //메뉴 버튼 활성
	}
	else
    {
		CCArray* childs = this->getChildren();
        CCLog("noti 00");
        CCDirector::sharedDirector()->pause();   //화면 정지
        
        //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pMenu);
        //메뉴버튼 비활성
	}
}
*/



void GameLayer::SetOpacities(int alpha)
{
    //pBackgroundSprite->setOpacity(alpha);
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
    m_bIsCycle = false;
}


void GameLayer::ReadyTimer(float f)
{
    iStartTimer--;

    if (iStartTimer == 3)
    {
        readyTime->setString("3");
        CCActionInterval* action = CCMoveBy::create(0.1f, ccp(0, -100-OBJECT_HEIGHT/2));
        readyTime->runAction(action);
    }
    else if (iStartTimer > 0)
    {
        if (iStartTimer == 2) readyTime->setString("2");
        else readyTime->setString("1");
    }
    else // 0 sec
    {
        readyTime->setString("0");
        CCFiniteTimeAction* action =
            CCSpawn::create(CCScaleTo::create(0.15f, 1.5f), CCFadeOut::create(0.15f), NULL);
        readyTime->runAction(action);

       	this->setTouchEnabled(true);

        this->unschedule(schedule_selector(GameLayer::ReadyTimer));
        this->schedule(schedule_selector(GameLayer::PuzzleTimer), 0.1f);
    }
}

void GameLayer::PuzzleTimer(float f)
{
    iRemainingPuzzleTime -= 0.10f;
    //CCLog("puzzle time : %f", iRemainingPuzzleTime);

    float round1 = ((int)(iRemainingPuzzleTime * pow(10.0, 1))) / pow(10.0, 1);
    //CCLog("%f %f", (float)((int)iRemainingPuzzleTime), round1);
    
    if ((float)((int)iRemainingPuzzleTime) == round1)
    {
        char time[3];
        sprintf(time, "%d", (int)iRemainingPuzzleTime);
        puzzleTime->setString(time);
    }
    progressTimer->setPercentage(iRemainingPuzzleTime / 60.0f * 100.0);
    
    if (iRemainingPuzzleTime <= 0.0f)
    {
        this->unschedule(schedule_selector(GameLayer::PuzzleTimer));
        sound->StopBackgroundSound();
        isFinished = true;
        ShowPuzzleResult();
    }
}

void GameLayer::ComboTimer(float f)
{
    iPassedComboTime += 100;
    if (iPassedComboTime >= 2000)
    {
        iPassedComboTime = 0;
        iNumOfCombo = 0;
        this->unschedule(schedule_selector(GameLayer::ComboTimer));
    }
    
}

void GameLayer::ShowPuzzleResult()
{
    CCScene* pScene = GameEndLayer::scene(iScore, iLevelBonus, iTypeBonus, iAcquiredWeight, vMaterial);
    
    this->addChild(pScene, 2000, 2000);
    
//    CCDirector::sharedDirector()->pause();
}



bool isValidPosition(int x, int y, float touchX, float touchY)
{
    if (!(0 <= x && x < COLUMN_COUNT && 0 <= y && y < ROW_COUNT))
        return false;
    
    // make a circle
    int ldX = Common::ComputeX(x);
    int ldY = Common::ComputeY(y);
    int dx = abs(touchX - (ldX+OBJECT_WIDTH/2));
    int dy = abs(touchY - (ldY+OBJECT_HEIGHT/2));
    return dx*dx + dy*dy <= 47*47; // 47 = radius of the circle.
}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    // CCLog("touch began");
    if (m_bIsBombing || isFinished)
        return;
    
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();

    /*
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
    */
    
	if (!m_bTouchStarted)
	{
		m_gestureStartBoardX = Common::ComputeBoardX(point.x);
		m_gestureStartBoardY = Common::ComputeBoardY(point.y);
        
        if (!isValidPosition(m_gestureStartBoardX, m_gestureStartBoardY, point.x, point.y))
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
    if (m_bIsBombing || isFinished || m_bIsCycle)
        return;
    
	if (m_bTouchStarted)
	{
		CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
        CCPoint point = pTouch->getLocation();

		int boardX = Common::ComputeBoardX(point.x);
		int boardY = Common::ComputeBoardY(point.y);

        if (!isValidPosition(boardX, boardY, point.x, point.y))
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
            // cycle이 되는 경우
            if (boardX == hanbut[0].x && boardY == hanbut[0].y && hanbut.size() >= 6)
            {
                m_bIsCycle = true;
            }
            // 사이클이 아닌 상황에서 이미 선택된 piece일 경우
            else if (AlreadySelected(boardX, boardY))
                return;
            
            if (abs(beforeX-boardX) + abs(beforeY-boardY) == 2)
            {
                // diamond가 대각선 진행 불가 type이면 못 가게 한다.
                int X = (beforeX > boardX) ? beforeX : boardX;
                int Y = (beforeY > boardY) ? beforeY : boardY;
                if (m_pBoardSP[X][Y]->GetType() == BLOCKED)
                {
                    m_bIsCycle = false;
                    return;
                }
                
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


void GameLayer::ShowComboLabel()
{
    char n[10];
    sprintf(n, "%d Combo!", iNumOfCombo);
    comboLabel->setString(n);
    CCFiniteTimeAction* action = CCSequence::create(CCFadeIn::create(0.15f), CCFadeOut::create(0.15f), NULL);
    comboLabel->runAction(action);
}

void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    if (m_bIsBombing || isFinished)
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
            // 3개 이상 한붓그리기 했으면, combo schedule 적용하고, 터뜨려야지!
            iNumOfCombo++;
            sound->playComboSound(iNumOfCombo);
            iPassedComboTime = 0;
            if (iNumOfCombo == 1) // 1st combo
            {
                this->schedule(schedule_selector(GameLayer::ComboTimer), 0.1f);
            }
            ShowComboLabel();
            
            // 6개 이상 터뜨리면 랜덤한 위치에 diamond item,
            if (hanbut.size() >= 6)
            {
                if (m_bIsCycle)
                {
                    CCLog("Give Cycle Item!!");
                }
                else
                {
                    CCLog("Give diamond item at random position.");
                }
            }
            // 가장 큰덩어리 다 터뜨리면(7개 이상) 8각형 item 주는 부분.
            if (iNumOfPieceOfLargestMass >= LARGEST_MASS_LOWER_BOUND &&
                    hanbut.size() >= iNumOfPieceOfLargestMass)
            {
                CCLog("Give hectagon item at random position.");
            }
            
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
    
    
    // 한붓그리기에서 대각선에 걸린 diamond들을 없앤다.
    for (int i = 0 ; i < connectDia.size() ; i++)
    {
        int x = connectDia[i].x;
        int y = connectDia[i].y;
        m_pBoardSP[x][y]->RemoveChildren();
    }
    connectDia.clear();

    // special diamond에 대한 처리
    std::vector<CCPoint> specialBomb;
    specialBomb.clear();
    for (int i = 0 ; i < special.size() ; i++)
    {
        int X = (int)special[i].x;
        int Y = (int)special[i].y;
        int type_sp = m_pBoardSP[X][Y]->GetTypeSP();
        
        if (type_sp == 0) // 8각형(12piece) bomb
        {
            for (int x = X-2 ; x <= X+1 ; x++)
            {
                for (int y = Y-2 ; y <= Y+1 ; y++)
                {
                    if ((x == X-2 || x == X+1) && (y == Y-2 || y == Y+1))
                        continue;
                    if (x < 0 || x >= COLUMN_COUNT || y < 0 || y >= ROW_COUNT)
                        continue;
                    
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
        
        // 아이템을 없애고(밑에서 한다), CONNECTED diamond로 바꾼다.
        m_pBoardSP[X][Y]->RemoveChildren();
        m_pBoardSP[X][Y]->SetType(CONNECTED);
        m_pBoardSP[X][Y]->SetTypeSP(-1);
    }
    
    
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
        
        // weight 랜덤하게 추가 (0.001 ~ 0.009)
        iAcquiredWeight += rand()%9+1;
    }
    // score update
    UpdateScore();
    
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
                //if (m_pBoardSP[x][y]->GetType() == CONNECTED)
                if (m_pBoardSP[x][y]->GetType() != BLOCKED)
                {
                    int type_sp = -1;
                    if (m_pBoardSP[x][y]->GetType() == SPECIAL)
                    {
                        type_sp = m_pBoardSP[x][y]->GetTypeSP();
                    }
                    
                    m_pBoardSP[x][y]->RemoveChildren();
                    m_pBoardSP[x][y]->CreateSpriteDia(this,
                                                    m_pBoard[x-1][y]->GetType(),
                                                    m_pBoard[x][y]->GetType(),
                                                    m_pBoard[x-1][y-1]->GetType(),
                                                    m_pBoard[x][y-1]->GetType(),
                                                    type_sp);
                    m_pBoardSP[x][y]->SetPositions(x, y);
                    m_pBoardSP[x][y]->AddChildren(this, zGameObjectSP);
                }
            }
        }
        
        // 가장 큰 덩어리를 구성하는 piece 개수 구한다.
        FindLargestMass();
        
        m_bTouchStarted = false;
        m_bIsBombing = false;
        m_bIsCycle = false;
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


void GameLayer::UpdateScore()
{
    // 3개 = 기본 10점 * 3 = 30점
    // 4개 = 기본 11점 * 4 = 44점
    // 5개 = 기본 12점 * 5 = 60점
    // ...
    iScore += hanbut.size() * (10+hanbut.size()-3);
    char score[8];
    sprintf(score, "%d", iScore);
    scoreLabel->setString(score);
}

int GameLayer::GetScore()
{
    return iScore;
}


// 가장 큰 덩어리를 구성하는 piece 개수를 구한다.
void GameLayer::FindLargestMass()
{
    iNumOfPieceOfLargestMass = 0;
    int num = 0;
    int check[COLUMN_COUNT*ROW_COUNT] = {0,};
    
    for (int x = 0 ; x < COLUMN_COUNT ; x++)
    {
        for (int y = 0 ; y < ROW_COUNT ; y++)
        {
            if (check[x*COLUMN_COUNT+y] == 0)
            {
                num = 0;
                FindLargestMassRecur(&num, 0, check, x, y, m_pBoard[x][y]->GetType());
                iNumOfPieceOfLargestMass = (std::max)(iNumOfPieceOfLargestMass, num);
            }
        }
    }
    //CCLog("larget mass # = %d", iNumOfPieceOfLargestMass);
}

// 위 함수를 위한 floodfill recursion 함수.
void GameLayer::FindLargestMassRecur(int* num, int cnt, int* check, int x, int y, int type)
{
    *num = *num + 1;
    check[x*COLUMN_COUNT+y] = 1;
    
    if (x > 0 && check[(x-1)*COLUMN_COUNT+y] == 0 && m_pBoard[x-1][y]->GetType() == type)
        FindLargestMassRecur(num, cnt+1, check, x-1, y, type);
    if (x < COLUMN_COUNT-1 && check[(x+1)*COLUMN_COUNT+y] == 0 && m_pBoard[x+1][y]->GetType() == type)
        FindLargestMassRecur(num, cnt+1, check, x+1, y, type);
    if (y > 0 && check[x*COLUMN_COUNT+y-1] == 0 && m_pBoard[x][y-1]->GetType() == type)
        FindLargestMassRecur(num, cnt+1, check, x, y-1, type);
    if (y < ROW_COUNT-1 && check[x*COLUMN_COUNT+y+1] == 0 && m_pBoard[x][y+1]->GetType() == type)
        FindLargestMassRecur(num, cnt+1, check, x, y+1, type);
    
    if (x > 0 && y > 0 && check[(x-1)*COLUMN_COUNT+y-1] == 0 && m_pBoard[x-1][y-1]->GetType() == type &&
        m_pBoardSP[x][y]->GetType() != BLOCKED)
        FindLargestMassRecur(num, cnt+1, check, x-1, y-1, type);
    if (x < COLUMN_COUNT-1 && y > 0 && check[(x+1)*COLUMN_COUNT+y-1] == 0 && m_pBoard[x+1][y-1]->GetType() == type && m_pBoardSP[x+1][y]->GetType() != BLOCKED)
        FindLargestMassRecur(num, cnt+1, check, x+1, y-1, type);
    if (x > 0 && y < ROW_COUNT-1 && check[(x-1)*COLUMN_COUNT+y+1] == 0 && m_pBoard[x-1][y+1]->GetType() == type && m_pBoardSP[x][y+1]->GetType() != BLOCKED)
        FindLargestMassRecur(num, cnt+1, check, x-1, y+1, type);
    if (x < COLUMN_COUNT-1 && y < ROW_COUNT-1 && check[(x+1)*COLUMN_COUNT+y+1] == 0 && m_pBoard[x+1][y+1]->GetType() == type && m_pBoardSP[x+1][y+1]->GetType() != BLOCKED)
        FindLargestMassRecur(num, cnt+1, check, x+1, y+1, type);
}


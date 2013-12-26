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
    
    // ready timer가 0이 되면 true로 바꾼다.
    this->setTouchEnabled(false);
    
    // sound
    sound = new Sound();
    sound->PreLoadSound();
    
    // puzzle pieces & diamonds && specials & connectLine
    pPuzzlePiece = CCTextureCache::sharedTextureCache()->addImage("images/all_pieces.png");
    pPuzzleDia = CCTextureCache::sharedTextureCache()->addImage("images/all_diamond_pieces.png");
    pPuzzleSP.push_back(CCTextureCache::sharedTextureCache()->addImage("images/one_piece_sp.png"));
    pPuzzleSP.push_back(CCTextureCache::sharedTextureCache()->addImage("images/dia_item_5sec.png"));
    pPuzzleSP.push_back(CCTextureCache::sharedTextureCache()->addImage("images/dia_item_1type.png"));
    pPuzzleSP.push_back(CCTextureCache::sharedTextureCache()->addImage("images/dia_item_crossline.png"));
    pPuzzleConnLineDiagonal = CCTextureCache::sharedTextureCache()->addImage("images/connect_diagonal.png");
    pPuzzleConnLineStraight = CCTextureCache::sharedTextureCache()->addImage("images/connect.png");

    
    // background
	pBackgroundSprite = CCSprite::create("images/background.png");
	pBackgroundSprite->setPosition(CCPointZero);
	pBackgroundSprite->setAnchorPoint(ccp(0.0f, 0.0f));
	addChild(pBackgroundSprite, zBackground);
    
    sound->playBackgroundSound(false);

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
    addChild(scoreLabel, 2);
    
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
    addChild(progressTimer, 2);
    
    puzzleTime = CCLabelTTF::create("60", "Arial", 80);
    puzzleTime->setAnchorPoint(ccp(1.0f, 1.0f));
    puzzleTime->setPosition(ccp(m_winSize.width-OBJECT_WIDTH, OBJECT_HEIGHT+20));
    puzzleTime->setColor(ccc3(0, 0, 0));
    addChild(puzzleTime, 2);
    
    readyTime = CCLabelTTF::create("3", "Arial", 60);
    readyTime->setPosition(ccp(m_winSize.width/2, m_winSize.height+100));
    addChild(readyTime, 2);
    
    iStartTimer = 4;
    iRemainingPuzzleTime = 6000;
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
    //diaTypeCnt.push_back(0);
    //diaTypeCnt.push_back(0);
    //diaTypeCnt.push_back(0);
    
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
            
            //diaTypeCnt[m_pBoardSP[x][y]->GetType()]++;
        }
    }
    
    
	m_bTouchStarted = false;
    m_bIsBombing = false;
    m_bIsCycle = false;
    
    isCrushing = false;
}


void GameLayer::ReadyTimer(float f)
{
    iStartTimer--;
    
    char number[2];
    sprintf(number, "%d", iStartTimer);
    readyTime->setString(number);

    if (iStartTimer == 3)
    {
        // 3->2->1->0 숫자를 위에서 아래로 당겨와 보여준다.
        CCActionInterval* action = CCMoveBy::create(0.1f, ccp(0, -100-OBJECT_HEIGHT/2));
        readyTime->runAction(action);
    }
    else if (iStartTimer == 0)
    {
        // puzzle을 시작하도록 만든다.
        CCFiniteTimeAction* action =
            CCSpawn::create(CCScaleTo::create(0.15f, 1.5f), CCFadeOut::create(0.15f), NULL);
        readyTime->runAction(action);

       	this->setTouchEnabled(true);
        //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);

        this->unschedule(schedule_selector(GameLayer::ReadyTimer));
        this->schedule(schedule_selector(GameLayer::PuzzleTimer), 0.1f);
    }
}

void GameLayer::PuzzleTimer(float f)
{
    iRemainingPuzzleTime -= 10;

    if (iRemainingPuzzleTime % 100 == 0)
    {
        // 초가 정수일 때 (60, 59, 58, ...) 숫자를 바꿔준다.
        char time[3];
        sprintf(time, "%d", iRemainingPuzzleTime/100);
        puzzleTime->setString(time);
    }
    progressTimer->setPercentage(iRemainingPuzzleTime / 6000.0f * 100.0);
    
    if (iRemainingPuzzleTime == 0)
    {
        // 퍼즐을 끝내고 popup 형태로 결과 화면을 띄운다.
        this->unschedule(schedule_selector(GameLayer::PuzzleTimer));
        sound->StopBackgroundSound();
        isFinished = true;
        ShowPuzzleResult();
    }
}

void GameLayer::ComboTimer(float f)
{
    if (m_bIsBombing)
        return;
    
    iPassedComboTime += 100;
    
    if (isCrushing)
    {
        // crush time 중인데 3초 지나면 crush time 해제.
        if (iPassedComboTime >= 3000)
        {
            iPassedComboTime = 0;
            iNumOfCombo = 0;
            this->unschedule(schedule_selector(GameLayer::ComboTimer));
            EndCrushTime();
        }
    }
    else
    {
        // 일반 모드에서 2초 지나면 콤보 해제.
        if (iPassedComboTime >= 2000)
        {
            iPassedComboTime = 0;
            iNumOfCombo = 0;
            this->unschedule(schedule_selector(GameLayer::ComboTimer));
        }
    }
}

void GameLayer::ShowComboLabel()
{
    char n[10];
    sprintf(n, "%d Combo!", iNumOfCombo);
    comboLabel->setString(n);
    CCFiniteTimeAction* action = CCSequence::create(CCFadeIn::create(0.15f), CCFadeOut::create(0.15f), NULL);
    comboLabel->runAction(action);
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
    
    /*
    // make a circle
    int ldX = Common::ComputeX(x);
    int ldY = Common::ComputeY(y);
    int dx = abs(touchX - (ldX+OBJECT_WIDTH/2));
    int dy = abs(touchY - (ldY+OBJECT_HEIGHT/2));
    return dx*dx + dy*dy <= 47*47; // 47 = radius of the circle.
     */
    
    int range = 29;
    int ldX = Common::ComputeX(x);
    int ldY = Common::ComputeY(y);
    if (abs(touchX-ldX) + abs(touchY-ldY) <= range)
        return false;
    if (abs(touchX-ldX) + abs(touchY-(ldY+OBJECT_HEIGHT)) <= range)
        return false;
    if (abs(touchX-(ldX+OBJECT_WIDTH)) + abs(touchY-ldY) <= range)
        return false;
    if (abs(touchX-(ldX+OBJECT_WIDTH)) + abs(touchY-(ldY+OBJECT_HEIGHT)) <= range)
        return false;
    
    return true;
}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
    // CCLog("touch began");
    if (m_bIsBombing || isFinished)
        return;
    
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint point = pTouch->getLocation();
    
	if (!m_bTouchStarted)
	{
		m_gestureStartBoardX = Common::ComputeBoardX(point.x);
		m_gestureStartBoardY = Common::ComputeBoardY(point.y);
        
        // 이상한 위치 or piece의 인식 범위 밖이면 시작하지 않는다.
        if (!isValidPosition(m_gestureStartBoardX, m_gestureStartBoardY, point.x, point.y))
            return;
		if (m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY] == NULL)
			return;
        
        m_bTouchStarted = true;
        sound->playTouchSound();
        
        // 벡터 초기화
        octaPiece.clear();
        diaPiece.clear();
        connLines.clear();
        
        // 8각형 piece를 터뜨릴 목록에 추가
        octaPiece.push_back(ccp(m_gestureStartBoardX, m_gestureStartBoardY));
        
        // 0.9배 축소 action
        m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY]->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY]->setPosition(
		ccp(Common::ComputeX(m_gestureStartBoardX)+OBJECT_WIDTH/2, Common::ComputeY(m_gestureStartBoardY)+OBJECT_HEIGHT/2));
        CCActionInterval* action = CCScaleTo::create(0.05f, 0.9f);
        m_pBoard[m_gestureStartBoardX][m_gestureStartBoardY]->runAction(action);
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

        // 이상한 위치 or piece의 인식 범위 밖이면 시작하지 않는다.
        if (!isValidPosition(boardX, boardY, point.x, point.y))
            return;
		if (m_pBoard[boardX][boardY] == NULL)
            return;
        
        int beforeX = octaPiece[octaPiece.size()-1].x;
        int beforeY = octaPiece[octaPiece.size()-1].y;
        
        // 이전 조각과 같은 type이며,
        // 1) 가로 혹은 세로로 연결되었거나,
        // 2) connected diamond를 통해 대각선을 연결되었다면
        // 한붓그리기 대상에 추가되는지 알아보도록 한다.
        if (m_pBoard[beforeX][beforeY]->GetType() == m_pBoard[boardX][boardY]->GetType() &&
            ( abs(beforeX-boardX) + abs(beforeY-boardY) == 1 ||
              (abs(beforeX-boardX) == 1 && abs(beforeY-boardY) == 1 &&
               m_pBoardSP[(std::max)(beforeX, boardX)][(std::max)(beforeY, boardY)]->GetType() != BLOCKED)))
        {
            // 사이클이 아닌 상황에서 이미 선택된 piece일 경우 취소.
            if (!IsPossibleToSelect(boardX, boardY))
                return;
            
            // 대각선으로 연결을 시도한 경우
            if (abs(beforeX-boardX) + abs(beforeY-boardY) == 2)
            {
                int diaX = (beforeX > boardX) ? beforeX : boardX;
                int diaY = (beforeY > boardY) ? beforeY : boardY;
                
                // connect line sprite (diagonal) 화면에 추가
                CCSprite* conn = new CCSprite();
                conn->initWithTexture(pPuzzleConnLineDiagonal, CCRectMake(0, 0, 192, 192));
                if ((beforeX < boardX && beforeY < boardY) || (beforeX > boardX && beforeY > boardY))
                {
                    // 그림상, right-up이나 left-down방향이면 90도 틀도록 한다.
                    conn->setRotation(90);
                }
                conn->setPosition(Common::ComputeXY(diaX, diaY));
                addChild(conn, zPieceConn);
                // 나중에 지울 수 있도록 vector에 추가
                connLines.push_back(conn);
                
                // 터뜨릴 목록에 지금 대각선으로 지나치는 diamond 좌표 추가
                diaPiece.push_back(ccp(diaX, diaY));
            }
            
            // 가로로 연결 시도한 경우
            else if (abs(beforeX-boardX) == 1)
            {
                // connect line sprite (straight) 화면에 추가
                CCSprite* conn = new CCSprite();
                conn->initWithTexture(pPuzzleConnLineStraight, CCRectMake(0, 0, 192, 96));
                int X = (beforeX > boardX) ? beforeX : boardX;
                conn->setPosition(ccp(Common::ComputeX(X), Common::ComputeY(beforeY)+OBJECT_HEIGHT/2));
                addChild(conn, zPieceConn);
                // 나중에 지울 수 있도록 vector에 추가
                connLines.push_back(conn);
            }
            
            // 세로로 연결 시도한 경우
            else
            {
                // connect line sprite (straight) 화면에 추가
                CCSprite* conn = new CCSprite();
                conn->initWithTexture(pPuzzleConnLineStraight, CCRectMake(0, 0, 192, 96));
                int Y = (beforeY > boardY) ? beforeY : boardY;
                conn->setRotation(90);
                conn->setPosition(ccp(Common::ComputeX(beforeX)+OBJECT_HEIGHT/2, Common::ComputeY(Y)));
                addChild(conn, zPieceConn);
                // 나중에 지울 수 있도록 vector에 추가
                connLines.push_back(conn);
            }
            
            // 8각형 piece도 터뜨릴 목록에 추가
            octaPiece.push_back(ccp(boardX, boardY));
            
            // 0.9배 축소 action
            m_pBoard[boardX][boardY]->setAnchorPoint(ccp(0.5f, 0.5f));
            m_pBoard[boardX][boardY]->setPosition(
                ccp(Common::ComputeX(boardX)+OBJECT_WIDTH/2, Common::ComputeY(boardY)+OBJECT_HEIGHT/2));
            CCActionInterval* action = CCScaleTo::create(0.05f, 0.9f);
            m_pBoard[boardX][boardY]->runAction(action);
            
            sound->playTouchSound();
        }
	}
}

bool GameLayer::IsPossibleToSelect(int x, int y)
{
    // cycle이 되는 상황이라면 true
    if (x == octaPiece[0].x && y == octaPiece[0].y && octaPiece.size() >= 6)
    {
        m_bIsCycle = true;
        return true;
    }
    
    // cycle아닌데 이미 선택된 것이라면 false
    for (int i = 0 ; i < octaPiece.size() ; i++)
    {
        if (octaPiece[i].x == x && octaPiece[i].y == y)
            return false;
    }
    
    return true;
}


void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
    if (m_bIsBombing || isFinished)
        return;
    
    if (m_bTouchStarted)
    {
        isOctaItemGiven = false;
        isDiaItemGiven = false;
        isDiaExItemGiven = false;
        
        // connect lines 그림들을 제거한다
        for (int i = 0 ; i < connLines.size() ; i++)
        {
            removeChild(connLines[i], true);
        }
        connLines.clear();
        
    
        // 3개 이상 한붓그리기가 되었다면, 콤보 처리와 함께 bomb를 시작한다.
        if (octaPiece.size() >= 3)
        {
            // crush time 도중이 아니라면,
            // 콤보 수 증가, 콤보 sound play, 콤보타임 초기화, 콤보수에 따른 콤보scheduler handling 등을 행한다.
            if (!isCrushing)
            {
                iNumOfCombo++;
                sound->playComboSound(iNumOfCombo);
                iPassedComboTime = 0;
                
                if (iNumOfCombo == 1) // 1st combo
                {
                    this->schedule(schedule_selector(GameLayer::ComboTimer), 0.1f);
                }
                else if (iNumOfCombo >= 6) // 6th combo = crushCrushCrush~
                {
                    iNumOfCombo = 0;
                    // crush time에 대한 환경 세팅(화면 등)을 하고, crush mode로 들어간다.
                    StartCrushTime();
                }
                else
                {
                    ShowComboLabel(); // 2~5콤보일 때 label을 화면 중앙에 살짝 보여준다.
                }
            
                /* ITEM을 줘야 하는 한붓그리기인지 체크한다 */
                if (octaPiece.size() >= 6)
                {
                    // 6개 이상 터뜨리면서 사이클이면 랜덤한 위치에 '질 좋은' diamond item을 준다.
                    if (m_bIsCycle)
                    {
                        CCLog("Give CYCLE Item!!");
                        isDiaExItemGiven = true;
                    }
                    // 6개 이상 터뜨리면 랜덤한 위치에 diamond item을 준다.
                    else
                    {
                        isDiaItemGiven = true;
                        CCLog("Give DIAMOND item at random position.");
                    }
                }
                // 가장 큰덩어리 다 터뜨리면(7개 이상) 랜덤한 위치에 8각형 item을 준다.
                if (iNumOfPieceOfLargestMass >= LARGEST_MASS_LOWER_BOUND &&
                    octaPiece.size() >= iNumOfPieceOfLargestMass)
                {
                    isOctaItemGiven = true;
                    CCLog("Give OCTAGON item at random position.");
                }
            }
            
            // crush time 중이면, crush 횟수를 증가시킨다.
            // 5회 지나면 crush time을 끝낸다 (실제로 끝내는 행위는 falling이 끝났을 때)
            else
            {
                iNumOfCrush++;
                iPassedComboTime = 0;
            }
            
            // 8각형 piece들 터뜨리는 행위 시작
            BombObject();
        }
        
        // 3개 미만으로 한붓그리기가 되었다면, 원상태로 복구시킨다.
        else
        {
            for (int i = 0 ; i < octaPiece.size() ; i++)
            {
                int X = (int)octaPiece[i].x;
                int Y = (int)octaPiece[i].y;
                // 원래 크기로 확대 action
                CCFiniteTimeAction* action =
                    CCSequence::create(CCScaleTo::create(0.05f, 1.0f),
                        CCCallFunc::create(this, callfunc_selector(GameLayer::FailedHanbutCallback)), NULL);
                m_pBoard[X][Y]->runAction(action);
            }
            
            m_bTouchStarted = false;
        }
    }
}

void GameLayer::FailedHanbutCallback()
{
    for (int i = 0 ; i < octaPiece.size() ; i++)
    {
        int X = (int)octaPiece[i].x;
        int Y = (int)octaPiece[i].y;
        m_pBoard[X][Y]->setAnchorPoint(ccp(0.0f, 0.0f));
        m_pBoard[X][Y]->setPosition(ccp(Common::ComputeX(X), Common::ComputeY(Y)));
    }
    octaPiece.clear();
    diaPiece.clear();
}


bool GameLayer::IsNotAppliedToBomb(int x, int y)
{
    // 한붓그리기된 조각에 이미 포함되어 있으면 추가하지 마라.
    for (int j = 0 ; j < octaPiece.size() ; j++)
    {
        if (x == octaPiece[j].x && y == octaPiece[j].y)
        {
            return false;
        }
    }
    return true;
}

void GameLayer::BombObject()
{
    //CCLog("BombObject");
    m_bIsBombing = true;
    m_callbackCnt = 0;
    
    // 한붓그리기 상의 대각선(diamond)을 검사한다
    for (int i = 0 ; i < diaPiece.size() ; i++)
    {
        int X = (int)diaPiece[i].x;
        int Y = (int)diaPiece[i].y;
        
        // 그냥 connected든지 item이든지 어쩄든 화면에서는 지운다.
        bool replaceToBlocked = false;
        for (int i = 0 ; i < replaceToBlockedDia.size() ; i++)
        {
            int x = replaceToBlockedDia[i].x;
            int y = replaceToBlockedDia[i].y;
            if (x == X && y == Y)
            {
                m_pBoardSP[x][y]->SetType(CONNECTED);
                replaceToBlocked = true;
                break;
            }
        }
        m_pBoardSP[X][Y]->RemoveChildren();
        if (replaceToBlocked)
            m_pBoardSP[X][Y]->SetType(BLOCKED);
        
        // item이라면 그에 맞는 행위를 적용시킨다.
        if (m_pBoardSP[X][Y]->GetType() == SPECIAL)
        {
            ApplyItem(X, Y);
        
            if (!isCrushing)
            {
                // 일반 모드라면, 아이템 화면에서 없애고, CONNECTED diamond로 바꾼다.
                m_pBoardSP[X][Y]->SetType(CONNECTED);
                m_pBoardSP[X][Y]->SetTypeSP(-1);
            }
            else
            {
                // crush time중에는 아이템을 화면에서 없애고, 'bomb' item diamond로 바꾼다.
                m_pBoardSP[X][Y]->SetTypeSP(0);
            }
        }
    }
    diaPiece.clear();
    
    // 8각형 piece들을 터뜨린다.
    // 이 때의 octaPiece에는 위에서 item으로 인해 추가된 piece들도 포함되어 있다.
    for (int i = 0 ; i < octaPiece.size() ; i++)
    {
        int X = (int)octaPiece[i].x;
        int Y = (int)octaPiece[i].y;
        
        // 8각형 주변의 diamond중에 터지지 않은 diamond를 마저 지운다.)
        if (X > 0 && Y > 0 && X < COLUMN_COUNT && Y < ROW_COUNT)
            m_pBoardSP[X][Y]->RemoveChildren(false);
        if (X > 0 && Y+1 > 0 && X < COLUMN_COUNT && Y+1 < ROW_COUNT)
            m_pBoardSP[X][Y+1]->RemoveChildren(false);
        if (X+1 > 0 && Y > 0 && X+1 < COLUMN_COUNT && Y < ROW_COUNT)
            m_pBoardSP[X+1][Y]->RemoveChildren(false);
        if (X+1 > 0 && Y+1 > 0 && X+1 < COLUMN_COUNT && Y+1 < ROW_COUNT)
            m_pBoardSP[X+1][Y+1]->RemoveChildren(false);
        
        // 터뜨리는 action
        GameObject* pGameObject = m_pBoard[X][Y];
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
    
    // item 생성에 대한 부분
    if (isDiaItemGiven)
    {
        CreateDiaItem(false);
    }
    if (isDiaExItemGiven)
    {
        //CreateDiaItem(true);
    }
    
    // score update
    UpdateScore();
    
    sound->playBombSound();
}

void GameLayer:: ApplyItem(int diaX, int diaY)
{
    int type_sp = m_pBoardSP[diaX][diaY]->GetTypeSP();
    
    // 8각형(12piece) 형태 bomb
    if (type_sp == 0)
    {
        for (int x = diaX-2 ; x <= diaX+1 ; x++)
        {
            for (int y = diaY-2 ; y <= diaY+1 ; y++)
            {
                if ((x == diaX-2 || x == diaX+1) && (y == diaY-2 || y == diaY+1))
                    continue;
                if (x < 0 || x >= COLUMN_COUNT || y < 0 || y >= ROW_COUNT)
                    continue;
                
                if (IsNotAppliedToBomb(x, y))
                {
                    octaPiece.push_back(ccp(x, y));
                }
            }
        }
    }
    
    // 남은시간 5초 추가.
    else if (type_sp == 1)
    {
        iRemainingPuzzleTime += 500;
        
        char time[3];
        sprintf(time, "%d", iRemainingPuzzleTime/100);
        puzzleTime->setString(time);
    }
    
    // 랜덤하게 한가지 type piece 모두 bomb.
    else if (type_sp == 2)
    {
        int oneType = rand()%TYPE_COUNT;
        for (int x = 0 ; x < COLUMN_COUNT ; x++)
        {
            for (int y = 0 ; y < ROW_COUNT ; y++)
            {
                if (m_pBoard[x][y]->GetType() == oneType && IsNotAppliedToBomb(x, y))
                {
                    octaPiece.push_back(ccp(x, y));
                }
            }
        }
    }
    
    // 십자가 bomb
    else if (type_sp == 3)
    {
        std::vector<CCPoint> temp;
        for (int x = diaX-1, y = diaY ; x >= 0 && y < ROW_COUNT ; x--, y++) // left up
            temp.push_back(ccp(x, y));
        for (int x = diaX-1, y = diaY-1 ; x >= 0 && y >= 0 ; x--, y--) // left down
            temp.push_back(ccp(x, y));
        for (int x = diaX, y = diaY ; x < COLUMN_COUNT && y < ROW_COUNT ; x++, y++) // right up
            temp.push_back(ccp(x, y));
        for (int x = diaX , y = diaY-1 ; x < COLUMN_COUNT && y >= 0 ; x++, y--) // right down
            temp.push_back(ccp(x, y));
        
        for (int i = 0 ; i < temp.size() ; i++)
        {
            if (IsNotAppliedToBomb(temp[i].x, temp[i].y))
            {
                octaPiece.push_back(temp[i]);
            }
        }
        temp.clear();
    }
}

void GameLayer::CreateDiaItem(bool isValuable)
{
    // dia type별 개수 파악
    int diaTypeCnt[TYPE_DIA_COUNT+1] = {0,};
    for (int x = 1 ; x < COLUMN_COUNT ; x++)
    {
        for (int y = 1 ; y < ROW_COUNT ; y++)
        {
            diaTypeCnt[m_pBoardSP[x][y]->GetType()]++;
        }
    }
    
    // 아이템 생성할 위치 결정하고 만든다.
    while(1)
    {
        int x = rand()%(COLUMN_COUNT-1)+1;
        int y = rand()%(ROW_COUNT-1)+1;

        // CONNECTED 다이아가 존재한다면 그곳에 아이템을 만들고, 아니라면 BLOCKED 부분에 만든다.
        // 또한 6개 이상을 터뜨림과 동시에 CRUSH MODE가 시작되는 경우가 있는데, 이때는 폭탄인 부분이면 그곳에 아이템 만든다.
        if ( (!isCrushing && ((m_pBoardSP[x][y]->GetType() == CONNECTED && diaTypeCnt[CONNECTED] > 0) ||
            (m_pBoardSP[x][y]->GetType() == BLOCKED))) ||
            (isCrushing && m_pBoardSP[x][y]->GetTypeSP() == 0))
        {
            CCLog("%d , %d", x, y);
            m_pBoardSP[x][y]->RemoveChildren();

            int type_sp = rand()%TYPE_DIA_COUNT + 1;
            m_pBoardSP[x][y]->SetType(SPECIAL);
            m_pBoardSP[x][y]->SetTypeSP(type_sp);
            
            m_pBoardSP[x][y]->CreateSpriteDia(this, -1, -2, -3, -4, type_sp);
            m_pBoardSP[x][y]->SetPositions(x, y);
            m_pBoardSP[x][y]->AddChildrenWithAction(this, zGameObjectSP);
            
            sound->playItemSound();
            
            break;
        }
    }
    
    
}


void GameLayer::BombCallback()
{
    m_callbackCnt++;
    
    if (m_callbackCnt == octaPiece.size())
    {
        // bomb action이 모두 끝나면 한붓그리기 상의 모든 조각을 제거한다.
        for (int i = 0 ; i < octaPiece.size() ; i++)
        {
            GameObject* pGameObject = m_pBoard[(int)octaPiece[i].x][(int)octaPiece[i].y];
            // remove from layer, and free memories.
			removeChild(pGameObject, true);
   			m_pBoard[(int)octaPiece[i].x][(int)octaPiece[i].y] = NULL;
        }
        octaPiece.clear();
        
        // 그리고 새로운 8각형 piece들의 drop을 시작한다.
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
        // drop이 모두 끝나면, diamond들을 다시 검사해서 적절히 바꿔준다.
		for (int x = 1 ; x < COLUMN_COUNT ; x++)
        {
            for (int y = 1 ; y < ROW_COUNT ; y++)
            {
                if (m_pBoardSP[x][y]->GetType() != BLOCKED)
                {
                    int type_sp = -1;
                    if (m_pBoardSP[x][y]->GetType() == SPECIAL)
                        type_sp = m_pBoardSP[x][y]->GetTypeSP();
                    
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
        
        // crush time이 끝나면 환경 세팅을 원래대로 돌린다.
        if (iNumOfCrush >= 5)
            EndCrushTime();
        
        // 가장 큰 덩어리를 구성하는 piece 개수 구한다.
        FindLargestMass();
        
        // 터뜨릴 수 있는 게 없다면, 어떤 하나를 바꿔서 가능하도록 만들어준다.
        if (iNumOfPieceOfLargestMass < 3)
        {
            ReplaceOneOctaPiece();
        }
        else
        {
            m_bTouchStarted = false;
            m_bIsBombing = false;
            m_bIsCycle = false;
        }
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

CCTexture2D* GameLayer::GetPuzzleSP(int type_sp)
{
    return pPuzzleSP[type_sp];
}


void GameLayer::UpdateScore()
{
    // 3개 = 기본 10점 * 3 = 30점
    // 4개 = 기본 11점 * 4 = 44점
    // 5개 = 기본 12점 * 5 = 60점
    // ...
    iScore += octaPiece.size() * (10+octaPiece.size()-3);
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

void GameLayer::ReplaceOneOctaPiece()
{
    std::vector<CCPoint> point;
    point.clear();
    int x, y, type;
    
    while (1)
    {
        x = rand()%COLUMN_COUNT;
        y = rand()%ROW_COUNT;
        
        if ((y-2 >= 0 && m_pBoard[x][y-1]->GetType() == m_pBoard[x][y-2]->GetType()))
            type = m_pBoard[x][y-1]->GetType();
        else if (y+2 < ROW_COUNT && m_pBoard[x][y+1]->GetType() == m_pBoard[x][y+2]->GetType())
            type = m_pBoard[x][y+1]->GetType();
        else if (x-2 >= 0 && m_pBoard[x-1][y]->GetType() == m_pBoard[x-2][y]->GetType())
            type = m_pBoard[x-1][y]->GetType();
        else if (x+2 > COLUMN_COUNT && m_pBoard[x+2][y]->GetType() == m_pBoard[x+1][y]->GetType())
            type = m_pBoard[x+1][y]->GetType();
        else if (x-1 >= 0 && x+1 < COLUMN_COUNT && m_pBoard[x-1][y]->GetType() == m_pBoard[x+1][y]->GetType())
            type = m_pBoard[x-1][y]->GetType();
        else if (y-1 >= 0 && y+1 < ROW_COUNT && m_pBoard[x][y-1]->GetType() == m_pBoard[x][y+1]->GetType())
            type = m_pBoard[x][y-1]->GetType();
        else if (x-1 >= 0 && y-1 >= 0 && y+1 < ROW_COUNT && m_pBoard[x-1][y+1]->GetType() == m_pBoard[x][y-1]->GetType() && m_pBoardSP[x][y+1]->GetType() != BLOCKED)
            type = m_pBoard[x-1][y+1]->GetType();
        else if (x-1 >= 0 && x+1 < COLUMN_COUNT && y+1 < ROW_COUNT && m_pBoard[x-1][y+1]->GetType() == m_pBoard[x+1][y]->GetType() && m_pBoardSP[x][y+1]->GetType() != BLOCKED)
            type = m_pBoard[x-1][y+1]->GetType();
        else if (x+1 < COLUMN_COUNT && y-1 >= 0 && y+1 < ROW_COUNT && m_pBoard[x+1][y+1]->GetType() == m_pBoard[x][y-1]->GetType() && m_pBoardSP[x+1][y+1]->GetType() != BLOCKED)
            type = m_pBoard[x+1][y+1]->GetType();
        else if (x-1 >= 0 && x+1 < COLUMN_COUNT && y+1 < ROW_COUNT && m_pBoard[x+1][y+1]->GetType() == m_pBoard[x-1][y]->GetType() && m_pBoardSP[x+1][y+1]->GetType() != BLOCKED)
            type = m_pBoard[x+1][y+1]->GetType();
        else if (x-1 >= 0 && y-1 >= 0 && y+1 < ROW_COUNT && m_pBoard[x-1][y-1]->GetType() == m_pBoard[x][y+1]->GetType() && m_pBoardSP[x][y]->GetType() != BLOCKED)
            type = m_pBoard[x-1][y-1]->GetType();
        else if (x-1 >= 0 && x+1 < COLUMN_COUNT && y-1 >= 0 && m_pBoard[x-1][y-1]->GetType() == m_pBoard[x+1][y]->GetType() && m_pBoardSP[x][y]->GetType() != BLOCKED)
            type = m_pBoard[x-1][y-1]->GetType();
        else if (x-1 >= 0 && x+1 < COLUMN_COUNT && y-1 >= 0 && m_pBoard[x+1][y-1]->GetType() == m_pBoard[x-1][y]->GetType() && m_pBoardSP[x+1][y]->GetType() != BLOCKED)
            type = m_pBoard[x+1][y-1]->GetType();
        else if (x+1 < COLUMN_COUNT && y-1 >= 0 && y+1 < ROW_COUNT && m_pBoard[x+1][y-1]->GetType() == m_pBoard[x][y+1]->GetType() && m_pBoardSP[x+1][y]->GetType() != BLOCKED)
            type = m_pBoard[x+1][y-1]->GetType();
        else
            continue;
        
        // action start
        replaceXY = ccp(x, y);
        replaceType = type;
        m_pBoard[x][y]->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pBoard[x][y]->setPosition(
                        ccp(Common::ComputeX(x)+OBJECT_WIDTH/2, Common::ComputeY(y)+OBJECT_HEIGHT/2));
        CCActionInterval* action = CCSequence::create(CCScaleTo::create(0.5f, 0),
                                    CCCallFunc::create(this, callfunc_selector(GameLayer::Replace1)), NULL);
        m_pBoard[x][y]->runAction(action);
        
        break;
    }
}

void GameLayer::Replace1()
{
    int x = (int)replaceXY.x;
    int y = (int)replaceXY.y;
    
    removeChild(m_pBoard[x][y]);
    m_pBoard[x][y] = NULL;

    m_pBoard[x][y] = GameObject::Create(replaceType, this);
    m_pBoard[x][y]->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pBoard[x][y]->setPosition(
                    ccp(Common::ComputeX(x)+OBJECT_WIDTH/2, Common::ComputeY(y)+OBJECT_HEIGHT/2));
    m_pBoard[x][y]->SetTargetBoardX(x);
    m_pBoard[x][y]->SetTargetBoardY(y);
    m_pBoard[x][y]->setScale(0);
    addChild(m_pBoard[x][y], zGameObject);

    CCActionInterval* action = CCSequence::create(CCScaleTo::create(0.5f, 1),
                                    CCCallFunc::create(this, callfunc_selector(GameLayer::Replace2)), NULL);
    m_pBoard[x][y]->runAction(action);
}

void GameLayer::Replace2()
{
    m_pBoard[(int)replaceXY.x][(int)replaceXY.y]->setAnchorPoint(ccp(0.0f, 0.0f));
    m_pBoard[(int)replaceXY.x][(int)replaceXY.y]->setPosition(
                    ccp(Common::ComputeX((int)replaceXY.x), Common::ComputeY((int)replaceXY.y)));
    
    m_bTouchStarted = false;
    m_bIsBombing = false;
    m_bIsCycle = false;
}


// crush time 시작하는 함수
void GameLayer::StartCrushTime()
{
    sound->playBackgroundSound(true);
    
    isCrushing = true;
    iNumOfCrush = 0;

    // 배경 추가
    crushTimeBackground = CCSprite::create("images/progressbar.png",
                                           CCRectMake(0, 0, m_winSize.width, m_winSize.height));
    crushTimeBackground->setAnchorPoint(ccp(0, 0));
    crushTimeBackground->setPosition(ccp(0, 0));
    addChild(crushTimeBackground, zBackground);
    
    // 추가한 배경을 깜빡이게 (흰색<->검은색) 만드는 action
    CCFiniteTimeAction* seq = CCSequence::create(
                                                 CCTintTo::create(0.1f, 255, 255, 255),
                                                 CCTintTo::create(0.1f, 0, 0, 0),
                                                 NULL);
    CCAction* rep = CCRepeatForever::create((CCActionInterval*)seq);
    crushTimeBackground->runAction(rep);
    
    // item이 아닌 diamond를 죄다 8각bomb diamond로 바꾼다.
    for (int x = 1 ; x < COLUMN_COUNT ; x++)
    {
        for (int y = 1 ; y < ROW_COUNT ; y++)
        {
            if (m_pBoardSP[x][y]->GetType() != SPECIAL)
            {
                m_pBoardSP[x][y]->RemoveChildren();
                
                m_pBoardSP[x][y]->SetType(SPECIAL);
                m_pBoardSP[x][y]->SetTypeSP(0);
                m_pBoardSP[x][y]->CreateSpriteDia(this,
                                                  m_pBoard[x-1][y]->GetType(),
                                                  m_pBoard[x][y]->GetType(),
                                                  m_pBoard[x-1][y-1]->GetType(),
                                                  m_pBoard[x][y-1]->GetType(),
                                                  0); // 폭탄 type으로 변경.
                m_pBoardSP[x][y]->SetPositions(x, y);
                m_pBoardSP[x][y]->AddChildren(this, zGameObjectSP);
            }
        }
    }
}

// crush time 끝내는 함수
void GameLayer::EndCrushTime()
{
    iNumOfCrush = -1;
    isCrushing = false;
    removeChild(crushTimeBackground, true);
    
    // 한붓그리기 도중에 crush time이 끝나면, 연결선에 걸쳐진 diamond들은 connected 상태로 유지한다.
    replaceToBlockedDia.clear();
    failedCrushedDiaPiece.clear();
    failedCrushedDiaPiece = diaPiece;

    // 다시 item diamond를 제외하고, CONNECTED/BLOCKED로 바꾼다.
    for (int x = 1 ; x < COLUMN_COUNT ; x++)
    {
        for (int y = 1 ; y < ROW_COUNT ; y++)
        {
            // 이 때는 무조건 special 이므로, bomb일 때만 없애면 된다.
            if (m_pBoardSP[x][y]->GetTypeSP() == 0)
            {
                m_pBoardSP[x][y]->RemoveChildren();
                
                int type = (rand()%100 < 60) ? BLOCKED : CONNECTED;
                m_pBoardSP[x][y]->SetType(type);
                m_pBoardSP[x][y]->SetTypeSP(-1);
                // 현재 한붓그리기가 진행중이었다면, 연결선에 걸친 diamond는 그림만 그대로 유지한다.
                for (int i = 0 ; i < failedCrushedDiaPiece.size() ; i++)
                {
                    if (failedCrushedDiaPiece[i].x == x && failedCrushedDiaPiece[i].y == y)
                    {
                        // blocked되어야 할 diamond는 따로 저장해 두었다가, 나중에 bomb하고 처리한다.
                        if (type == BLOCKED)
                        {
                            replaceToBlockedDia.push_back(ccp(x, y));
                        }
                        m_pBoardSP[x][y]->SetType(CONNECTED); // 잠시 connected인 척한다.
                        break;
                    }
                }
                
                m_pBoardSP[x][y]->CreateSpriteDia(this,
                                                  m_pBoard[x-1][y]->GetType(),
                                                  m_pBoard[x][y]->GetType(),
                                                  m_pBoard[x-1][y-1]->GetType(),
                                                  m_pBoard[x][y-1]->GetType(),
                                                  -1); // 일반 diamond로 변경.
                m_pBoardSP[x][y]->SetPositions(x, y);
                m_pBoardSP[x][y]->AddChildren(this, zGameObjectSP);
            }
        }
    }
    
    failedCrushedDiaPiece.clear();
    
    sound->playBackgroundSound(false);
}


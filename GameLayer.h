#ifndef PuzzleGame_GameLayer
#define PuzzleGame_GameLayer

#include "Common.h"
#include "GameObject.h"
#include "GameObjectSP.h"
#include "Sound.h"

class GameObject;
class GameObjectSP;

class GameLayer : public CCLayer
{
public:
	bool init();
    
    virtual void keyBackClicked();

	void StartGame();

	bool IsStreak(int x, int y);
	int StreakHorz(int x, int y);
	int StreakVert(int x, int y);
	bool CheckType(int type, int x, int y);

	//bool IsAdjacent(int x1, int y1, int x2, int y2);
	//void SwapObjects(int x1, int y1, int x2, int y2, bool bRollback = false);
	//void RemoveObject(int x, int y);

	//void SlidingFinished(int x1, int y1, int x2, int y2);

	void CheckStreaks();

	void ProcessFalling();
	void FallingFinished(int x1, int y1, int x2, int y2);

	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    inline bool AlreadySelected(int x, int y);
    void TouchCallback();
    
    void BombObject();
    void BombCallback();
    
    CCTexture2D* GetPuzzlePiece();
    CCTexture2D* GetPuzzleDia();
    CCTexture2D* GetPuzzleSP();
	
	static CCScene* scene();

	CREATE_FUNC(GameLayer);

protected:
	GameObject* m_pBoard[COLUMN_COUNT][ROW_COUNT];
    GameObjectSP* m_pBoardSP[COLUMN_COUNT][ROW_COUNT];

	CCSize m_winSize;

	bool m_bTouchStarted;
    bool m_bTouchEnded;

	int m_gestureStartBoardX;
	int m_gestureStartBoardY;

	int m_numOfSlidingObjects;
	int m_numOfFallingObjects;
	Sound* sound;
    
    std::vector<CCPoint> hanbut;
    std::vector<CCSprite*> connect;
    int m_callbackCnt;
    
    CCTexture2D* pPuzzlePiece;
    CCTexture2D* pPuzzleDia;
    CCTexture2D* pPuzzleSP;
    CCSprite* pBackgroundSprite;
};

#endif
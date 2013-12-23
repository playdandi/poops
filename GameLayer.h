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
    
    void SetOpacities(int alpha);
    void ReadyTimer(float f);
    void PuzzleTimer(float f);
    void ComboTimer(float f);

    //void doNotification(CCObject* obj);
    void ShowPuzzleResult();

	void ProcessFalling();
	void FallingFinished(int x1, int y1, int x2, int y2);

	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
    inline bool AlreadySelected(int x, int y);
    void ShowComboLabel();
    void TouchCallback();
    
    void BombObject();
    void BombCallback();
    
    bool IsNotAppliedToBomb(int x, int y);
    
    void StartCrushTime();
    void EndCrushTime();
    //void CrushTimer(float f);
    
    CCTexture2D* GetPuzzlePiece();
    CCTexture2D* GetPuzzleDia();
    CCTexture2D* GetPuzzleSP(int type_sp);
    
    void UpdateScore();
	int GetScore();
    
    void FindLargestMass();
    void FindLargestMassRecur(int* num, int cnt, int* check, int x, int y, int type);
    
    static CCScene* scene();
	CREATE_FUNC(GameLayer);

protected:
	GameObject* m_pBoard[COLUMN_COUNT][ROW_COUNT];
    GameObjectSP* m_pBoardSP[COLUMN_COUNT][ROW_COUNT];

	CCSize m_winSize;

	bool m_bTouchStarted;
    bool m_bIsBombing;
    bool m_bIsCycle;

	int m_gestureStartBoardX;
	int m_gestureStartBoardY;

	int m_numOfSlidingObjects;
	int m_numOfFallingObjects;
	Sound* sound;
    
    std::vector<CCPoint> hanbut;
    std::vector<CCPoint> connectDia;
    std::vector<CCSprite*> connect;
    std::vector<CCPoint> special;
    int m_callbackCnt;
    
    CCTexture2D* pPuzzlePiece;
    CCTexture2D* pPuzzleDia;
    std::vector<CCTexture2D*> pPuzzleSP;
    CCSprite* pBackgroundSprite;
    CCSprite* pRefreshSprite;
    
    CCLabelTTF* readyTime;
    CCLabelTTF* puzzleTime;
    CCLabelTTF* scoreLabel;
    CCProgressTimer* progressTimer;
    CCLabelTTF* comboLabel;
    
    CCSprite* crushTimeBackground;
    
private:
    bool isFinished;
    
    int iStartTimer;
    int iRemainingPuzzleTime;
    
    int iScore;
    int iLevelBonus;
    int iTypeBonus;
    int iAcquiredWeight;
    std::vector<int> vMaterial;
    
    int iPassedComboTime;
    int iNumOfCombo;
    
    int iNumOfPieceOfLargestMass;
    
    int iNumOfCrush;
    bool isCrushing;
};

#endif
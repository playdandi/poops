#ifndef PuzzleGame_GameObject
#define PuzzleGame_GameObject

#include "Common.h"

class GameLayer;

class GameObject : public CCSprite
{
public:
	GameObject();
	~GameObject();

protected:
	//static GameObject* create(const char* pszFileName, const CCRect& rect);

public:
	static GameObject* Create(int type, GameLayer* gameLayer);

	int GetType();
	void SetType(int type);

	int GetTargetBoardX();
	void SetTargetBoardX(int x);
	int GetTargetBoardY();
	void SetTargetBoardY(int y);

	void ProcessFalling();
	void FallingCompleteHandler();
    
	void SetGameLayer(GameLayer* pGameLayer);

private:
    int m_type;

	int m_prevBoardX;
	int m_prevBoardY;

	int m_targetBoardX;
	int m_targetBoardY;

	GameLayer* m_pGameLayer;
};

#endif
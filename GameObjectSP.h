#ifndef __poops8__GameObjectSP__
#define __poops8__GameObjectSP__

#include "Common.h"

#define BLOCKED 0
#define CONNECTED 1
#define SPECIAL 2

class GameLayer;

class GameObjectSP
{
public:
	GameObjectSP();
	~GameObjectSP();
    
public:
	static GameObjectSP* Create(GameLayer* gameLayer, int lu, int ru, int ld, int rd);
	int GetType();
	void SetType(int type);
    int GetTypeSP();
    void SetTypeSP(int type_sp);
    
	int GetTargetBoardX();
	void SetTargetBoardX(int x);
	int GetTargetBoardY();
	void SetTargetBoardY(int y);
    
	void SetGameLayer(GameLayer* pGameLayer);
    GameLayer* GetGameLayer();
    
    //void CreateSpriteSP(int type_sp, GameLayer* gameLayer);
    void CreateSpriteDia(GameLayer* gameLayer, int lu, int ru, int ld, int rd, int type_sp);
    
    void SetPositions(int x, int y);
    void AddChildren(GameLayer* gameLayer, int zOrder);
    void RemoveChildren(bool isItemRemoved = true);
    
private:
    int m_type; // 0 : blocked, 1 : connected , 2 : special
    int m_type_sp; // special type -> 0 : 4*4 bomb , ......
    
    CCSprite* m_leftup;
    CCSprite* m_rightup;
    CCSprite* m_leftdown;
    CCSprite* m_rightdown;
    CCSprite* m_special;
    
	int m_targetBoardX;
	int m_targetBoardY;
    
	GameLayer* m_pGameLayer;
};

#endif

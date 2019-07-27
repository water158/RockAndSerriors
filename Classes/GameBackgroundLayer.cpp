#include "GameBackgroundLayer.h"

GameBackgroundLayer::GameBackgroundLayer()
{
    m_pCells = NULL;
}

GameBackgroundLayer::~GameBackgroundLayer()
{
    CC_SAFE_RELEASE_NULL(m_pCells);
}

bool GameBackgroundLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_pCells = CCSpriteBatchNode::create("gameui/hex/hex_back.png", 1000);
	m_pCells->retain();
    addChild(m_pCells);
    
    
    
    int indexMax = BoardLogic::sharedBoardLogic()->getMaxIndex();
    float scale = BoardLogic::sharedBoardLogic()->getCellScale();
    
    for (int j = 0; j <= indexMax; j++)
    {
        for (int i = 0; i <= indexMax; i++)
        {

            if (BoardLogic::sharedBoardLogic()->isItemOutOfRange(i, j))
            {
                continue;
            }
            
            CCPoint pos = BoardLogic::sharedBoardLogic()->getCellPosition(i, j);
            
            //CCLog("Cell %d, %d pos %f %f", i, j, pos.x, pos.y);
            
            CCSprite *sp = CCSprite::create("gameui/hex/hex_back.png");
            sp->setScale(scale);
            sp->setPosition(pos);
            
            m_pCells->addChild(sp);
        }
        
    }
    
    
    return true;
}


GameBackgroundLayer* GameBackgroundLayer::create()
{
    GameBackgroundLayer *pRet = new GameBackgroundLayer();
    
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void GameBackgroundLayer::draw()
{
    CCPoint pos = CCPointZero;
    ccDrawColor4B(255, 0, 0, 255);
    ccPointSize(3);
    ccDrawPoints(&pos, 1);
}
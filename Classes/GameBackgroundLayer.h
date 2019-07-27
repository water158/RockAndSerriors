#ifndef __GameBackgroundLayer_H__
#define __GameBackgroundLayer_H__

#include "cocos2d.h"
#include "BoardLogic.h"

USING_NS_CC;
using namespace std;


class GameBackgroundLayer : public CCLayer
{
public:
    virtual ~GameBackgroundLayer();
    
    virtual bool init();
    
    static GameBackgroundLayer* create();
    
    virtual void draw();
    
private:
    GameBackgroundLayer();
    //
    CCSpriteBatchNode *m_pCells;
};

#endif // __GameBackgroundLayer_SCENE_H__

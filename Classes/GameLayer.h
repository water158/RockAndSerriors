//
//  GameLayer.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-4.
//
//

#ifndef __RockPaperScissors__GameLayer__
#define __RockPaperScissors__GameLayer__

#include <iostream>
#include "cocos2d.h"
#include "CardSprite.h"
#include "ConfigureLayer.h"
#include "ArchieveLayer.h"
#include "UserData.h"
#include "Tools.h"
#include "GsoundManager.h"
#include "MyXmlParser.h"
#include "MobClickCpp.h"
#include "MyShieldLayer.h"
#include "SmartRes.h"
#include "MyMenu.h"

USING_NS_CC;
using namespace std;

class GameLayer : public CCLayer
{
public:
	virtual ~GameLayer();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();

	static GameLayer* create();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event);
    
    //滑向上下左右的方法
    bool doUp();
    bool doDown();
    bool doLeft();
    bool doRight();
    
    void createCardSprite(CCSize size);
    
    //CREATE_FUNC(GameLayer);
    
    //自动生成卡片
    void autoCreateCardNumber();
    
    //判断游戏是否还能继续
    void doCheckGameOver();
    
    //步数
    int stepCount;
    
    //步数标志
    CCLabelTTF* labelStep;
    //难度
    CCMenuItemFont* labelDifficulty;
    //难度数
    int numDifficulty;
    
    //创建卡片二维数组
    CardSprite* cardArr[9][9];
    //开始按钮
    CCMenuItemFont* btnStart;
    //游戏开始回调
    void startCallback(CCObject* pSender);
    //更改游戏难度回调
    void changeDifficulty(CCObject* pSender);
    //成就回调
    void achiveCallback(CCObject* pSender);
    //游戏配置回调
    void configCallback(CCObject* pSender);
    //确定胜利回调函数
    void sureCallback(CCObject* pSender);
    //临时用来测试两种移动方式互换
    void changeMoveType(CCObject* pSender);
private:
	GameLayer();
    //点击元素
    int firstX,firstY,endX,endY;
    
    XMLParser* pXmlparser;
};

#endif /* defined(__RockPaperScissors__GameLayer__) */

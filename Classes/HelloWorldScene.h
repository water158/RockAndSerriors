#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

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
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;


class HelloWorld : public cocos2d::CCLayer
{
public:
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
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
    
    virtual void onEnter();
    virtual void onExit();
    
    void createCardSprite(CCSize size);
    
    CREATE_FUNC(HelloWorld);
    
   
    
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
    
    //只做移动
    void ShiftByDir(Shift dir);
    //消除
    void KillByDir(Shift dir);
    //临时用来测试两种移动方式互换
    void changeMoveType(CCObject* pSender);
    
private:
    //点击元素
    int firstX,firstY,endX,endY;
    //是否产生新的
    bool isCreated;
    
    XMLParser* pXmlparser;
};

#endif // __HELLOWORLD_SCENE_H__

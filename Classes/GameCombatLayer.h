#ifndef __GameCombatLayer_H__
#define __GameCombatLayer_H__

#include "cocos2d.h"

#include "GlobalDelegate.h"
#include "GlobalEnum.h"

#include "SimpleAudioEngine.h"
#include "GSoundManager.h"

#include "ConfigureLayer.h"
#include "ArchieveLayer.h"
#include "GamePlayLayer.h"
#include "GameBackgroundLayer.h"

#include "StringLibrary.h"
#include "UserData.h"
#include "Tools.h"
#include "SmartRes.h"

#include "MyMenu.h"
#include "MyShieldLayer.h"

#include "MobClickCpp.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

#define BOTTOMMARGIN 60
#define DIFFICULTYTAG 10086

//返回按钮坐标
#define BTN_BACK_X _s_left + 60
#define BTN_BACK_Y _s_top - 60

//成就按钮坐标
#define BTN_ROCK_X _s_right - 120
#define BTN_PAPER_X _s_right - 120
#define BTN_SCISSORS_X _s_right - 120
#define BTN_ACHIEVE_Y _s_top - 60

//配置按钮坐标
#define BTN_CONFIG_X _s_right - 80
#define BTN_CONFIG_Y _s_center.y + 400

//难度按钮坐标
#define BTN_DIF_X _s_center.x
#define BTN_DIF_Y BTN_CONFIG_Y

//步数坐标
#define SP_STEPS_X _s_center.x - 100
#define SP_STEPS_Y _s_center.y + 300

//下一步坐标
#define SP_NEXTITEM_X _s_center.x + 100
#define SP_NEXTITEM_Y SP_STEPS_Y

//成就按钮宽高
#define BTN_ACHIEVE_WIDTH 54
#define BTN_ACHIEVE_HEIGHT 112

class GameCombatLayer : public CCLayer, public GameUIDelegate
{
    /***************************
     * 初始化 构造 什么的
     ***************************/
public:
    virtual bool init();

    static CCScene* scene();
    
    // CREATE marco
    CREATE_FUNC(GameCombatLayer);
   
private:
    
    CCSprite* spNextItem;
    
    //石头成就按钮
    CCMenuItemImage* btnRock;
    //剪刀成就按钮
    CCMenuItemImage* btnScissors;
    //布成就按钮
    CCMenuItemImage* btnPaper;
    //更改地图样式按钮
    CCMenuItemImage* btnChangMapType;
    
    //更新成就按钮图标
    void updateAchieveBtn();
    
    /***************************
     * UI
     ***************************/
public:
    
    //步数标志
    CCLabelTTF* labelStep;

    GamePlayLayer *layerPlay;
    
    GameBackgroundLayer* layerBackground;
    
    MyShieldLayer* layerShield;
    
    /***************************
     * UI 回调
     ***************************/
public:
    //开始按钮回调
    void callbackStart(CCObject* pSender);
    //更改游戏难度回调
    void callbackChangeDifficulty(CCObject* pSender);
    //成就回调
    void callbackOpenAchievement(CCObject* pSender);
    //游戏配置回调
    void callbackOpenConfig(CCObject* pSender);
    //确定胜利回调函数
    void callbackWinSure(CCObject* pSender);
    //临时用来测试两种移动方式互换
    void callbackUseMagic(CCObject* pSender);
    //更改游戏地图样式回调
    void callbackChangeMapType(CCObject* pSender);
    
    //
    void callbackChangeNextItem();
    
    /***************************
     * GameUIDelegate 回调
     ***************************/
    //游戏结束
    void callbackCheckGameOver();
    
    //刷新步数
    void callbackRefreshSteps();
    
    /***************************
     * 逻辑
     ***************************/
public:
    //难度数
    int m_nDifficulty;
    
    int m_nDifficultySelected;
};

#endif // __GameCombatLayer_SCENE_H__

#ifndef __GamePlayLayer_H__
#define __GamePlayLayer_H__

#include "cocos2d.h"

#include "GlobalDelegate.h"

#include "BoardLogic.h"
#include "Tools.h"

USING_NS_CC;
using namespace std;


class GamePlayLayer : public CCLayer
{
    /***************************
     * 基本方法
     ***************************/
public:
    static GamePlayLayer* create();
    
    GamePlayLayer();
    
    virtual ~GamePlayLayer();
    
    //初始化
    virtual bool init();
    
    //绘制
    virtual void draw();
    
    //进入层
    virtual void onEnter();
    
    //退出层
    virtual void onExit();
    
    /***************************
     * 代理
     ***************************/
    
    GameUIDelegate *delegateGameUI;
    
    /***************************
     * 核心方法
     ***************************/
public:
    //滑动手指之后 去掉空格 开始吃喝 再次去掉空格
    void slide(FingerDirection dir);
    
    //随机产生一个新的元素 返回值为false则不会产生
    bool random();
    
    void action();
    
    //刷新棋盘
    void refresh();
    
    void refreshZOrder();
    
private:
    //棋盘
    CCSpriteBatchNode *m_pBatchNodeCells;
    
    //在Dict中放置棋子
    CCDictionary *m_pDictCells;
    
    /***************************
     * 触摸
     ***************************/
public:
    //触摸开始
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event);
    
    //触摸结束
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event);
    
private:
    //上一次点击的位置
    CCPoint pointBegin;
    
    bool m_bFingerActionEnable;
    
    /***************************
     * 执行动作
     ***************************/
private:
    //放大
    void runRandomAction(CCSprite *sp, ItemHex *item);
    
    //放大之后
    void callbackAfterRandom(CCNode* pTarget, void* data);
    
    //移动
    void runShiftAction(ItemHex *item_to);
    
    //移动之后
    void callbackAfterShift(CCNode* pTarget, void* data);
    
    //吃
    void runEatAction(ItemHex *item_to);
    
    //吃之后
    void callbackAfterEat(CCNode* pTarget, void* data);
    
    //根据元素的类型 改变元素
    void changeItemByItemType(CCSprite *sp, ItemType type);
};

#endif // __GamePlayLayer_SCENE_H__

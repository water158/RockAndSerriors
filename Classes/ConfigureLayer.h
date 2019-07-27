//
//  ConfigureLayer.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-4.
//
//

#ifndef __RockPaperScissors__ConfigureLayer__
#define __RockPaperScissors__ConfigureLayer__

#include <iostream>
#include "cocos2d.h"
#include "UserData.h"
#include "MyXmlParser.h"
#include "GameHelp.h"
#include "GameAbout.h"
#include "GsoundManager.h"
#include "MyMenu.h"
#include "SmartRes.h"

enum GameInfo
{
    eHelp,
    eAbout,
};

USING_NS_CC;

class ConfigureLayer : public CCLayer
{
public:
    
    virtual bool init();
    
    static CCScene* scene();
    
    CREATE_FUNC(ConfigureLayer);
    
    //更改设置的回调
    void changeConfigCallback(CCObject* pSender);
    //关于和帮助的回调
    void helpAndAboutCallback(CCObject* pSender);
    //返回的回调
    void backCallback(CCObject* pSender);
    //触摸函数
    virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch,CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
    virtual void onEnter();
private:
    //音效控制按钮背景
    CCSprite* contrSoundBG;
    //音效控制按钮
    CCSprite* contrSound ;
    //音乐控制按钮背景
    CCSprite* contrMsicBG;
    //音乐控制按钮
    CCSprite* contrMsic;
    //点击的起始点
    CCPoint beginPoint;
    
    XMLParser* pXmlParser;
};

#endif /* defined(__RockPaperScissors__ConfigureLayer__) */

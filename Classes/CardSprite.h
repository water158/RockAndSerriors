//
//  CardSprite.h
//  stone
//
//  Created by mini01 on 14-4-28.
//
//

#ifndef __stone__CardSprite__
#define __stone__CardSprite__

#include <iostream>
#include "cocos2d.h"

#define ROCK 2
#define PAPPER 10
#define SCISSOS 5
#define NOTHING 0
#define CARDSPRITETAG 15


USING_NS_CC;

class CardSprite : public cocos2d::CCSprite
{
public:
    //
    static CardSprite* createCardSprite(int numbers,int width,int height,float CardSpriteX,float CardSpriteY);
    virtual bool init();
    
    CREATE_FUNC(CardSprite);
    
    int getNumber();
    
    void setNumber(int num, bool isKilled,bool isCreated);
    
    int difficulty;
    
private:
    //卡片数字  代表该卡片的种类
    int number;
    void enemyInit(int numbers,int width,int height,float CardSpriteX,float CardSpriteY);
    
    CCLabelTTF* labelCardNumber;
    //图片的宽度
    int spriteWidth;
public:
    CCLayerColor* layerColorBG;
    CCSprite* icon;
};

#endif /* defined(__stone__CardSprite__) */

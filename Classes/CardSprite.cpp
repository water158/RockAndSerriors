//
//  CardSprite.cpp
//  stone
//
//  Created by mini01 on 14-4-28.
//
//

#include "CardSprite.h"


CardSprite* CardSprite::createCardSprite(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
    CardSprite* enemy = new CardSprite();
    
    if (enemy&&enemy->init())
    {
        enemy->autorelease();
        enemy->enemyInit(numbers, width, height, CardSpriteX, CardSpriteY);
        
        return enemy;
    }
    
    CC_SAFE_DELETE(enemy);
    return NULL;
}

bool CardSprite::init()
{
    return CCSprite::init();
}

void CardSprite::enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
    number = numbers;
    
    spriteWidth = width-15;
    
    layerColorBG = CCLayerColor::create(ccc4(204,192,180,255),width-15,height-15);
    layerColorBG->setPosition(ccp(CardSpriteX,CardSpriteY));
    
    //添加大背景框
    CCLayerColor* layercolorBigBg = CCLayerColor::create(ccc4(187, 173, 160, 255),width,height);
    layercolorBigBg->setPosition(ccp(CardSpriteX-7.5,CardSpriteY-7.5));
    
    this->addChild(layercolorBigBg);
    this->addChild(layerColorBG);
}

int CardSprite::getNumber()
{
    return number;
}

void CardSprite::setNumber(int num, bool isKilled,bool isCreated)
{
    number = num;
    if (number != NOTHING)
    {
        //如果有图标 则
        if (icon)
        {
            layerColorBG->removeChildByTag(CARDSPRITETAG);
            icon = NULL;
        }
        
        {
            switch (number)
            {
                case ROCK:
                    icon = CCSprite::create("gameui/shitou.png");
                    break;
                case SCISSOS:
                    icon = CCSprite::create("gameui/jianzi.png");
                    break;
                case PAPPER:
                    icon = CCSprite::create("gameui/bu.png");
                    break;
                    
                default:
                    break;
            }
            if (icon)
            {
                icon->setPosition(ccp(layerColorBG->getContentSize().width/2,layerColorBG->getContentSize().height/2));
                //确定图片的比例
                float sca = spriteWidth / icon->getContentSize().width;
                if (isKilled)
                {
                    icon->setScale(sca);
                    CCActionInterval * scaleby = CCScaleTo::create(0.1f, 1.5f * sca);
                    CCActionInterval * scalenback = CCScaleTo::create(0.1f, sca);

                    CCSequence* seq = CCSequence::create(scaleby, scalenback,NULL);
                    icon->runAction(seq);
                }
                if (isCreated)
                {
                    icon->setScale(0.001f);
                    
                    icon->runAction(CCScaleTo::create(0.1f, sca));
                }
                if (!isCreated && !isKilled)
                {
                    icon->setScale(sca);
                }
                
                icon->setTag(CARDSPRITETAG);
                layerColorBG->addChild(icon);
            }
        }

    }
    else
    {
        if (icon)
        {
            layerColorBG->removeChildByTag(CARDSPRITETAG);
            icon = NULL;
        }
    }
}





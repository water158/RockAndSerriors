//
//  AchievePage.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-7.
//
//

#include "AchievePage.h"

AchievePage* AchievePage::createWithDifficulty(int difficulty)
{
    AchievePage* ach = new AchievePage();
    
    if (ach&&ach->init())
    {
        ach->autorelease();
        ach->m_difficulty = difficulty;
        ach->initWithDiff(difficulty);
        
        return ach;
    }
    
    CC_SAFE_DELETE(ach);
    return NULL;
}

bool AchievePage::init()
{
    return CCNode::init();
}

bool AchievePage::initWithDiff(int diff)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //将节点设置与屏幕同等大小
    this->setContentSize(visibleSize);
    //将节点锚点设置为00
    this->setAnchorPoint(CCPointZero);
    //添加难度标志
    CCLabelTTF* labelDiff = CCLabelTTF::create(CCString::createWithFormat("%d X %d",diff,diff)->getCString(), "Arial", 40);
    labelDiff->setPosition(ccp(visibleSize.width / 2,visibleSize.height * 0.8));
    labelDiff->setColor(ccRED);
    this->addChild(labelDiff);

    //添加该成就下面石头成就的详细信息
    CCNode* achRockNode = getInfoByType(eRock);
    achRockNode->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.65f));
    this->addChild(achRockNode);
    
    //添加该成就下面剪刀成就的详细信息
    CCNode* achScissorsNode = getInfoByType(eScissors);
    achScissorsNode->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.45f));
    this->addChild(achScissorsNode);
    
    //添加该成就下布成就的详细信息
    CCNode* achPaperNode = getInfoByType(ePaper);
    achPaperNode->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.25f));
    this->addChild(achPaperNode);
    
    return true;
}

CCNode* AchievePage::getInfoByType(ItemType type)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCNode* node = CCNode::create();
    node->setAnchorPoint(ccp(0.5,0.5));
    node->setContentSize(CCSize(visibleSize.width * 0.9f,visibleSize.height * 0.2f));
    //为层添加颜色
    CCLayerColor* layer = NULL;
    if (type == eRock || type == ePaper)
    {
       layer = CCLayerColor::create(ccc4(120, 113, 107, 255),visibleSize.width * 0.9f,visibleSize.height * 0.2f);
    }
    else
    {
       layer = CCLayerColor::create(ccc4(70, 200, 107, 255),visibleSize.width * 0.9f,visibleSize.height * 0.2f);
    }
    
    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);
    node->addChild(layer);
    
    int times = UserData::sharedUserData()->getTimesByKey(m_difficulty, type);
    
    if (type == eRock)
    {
        CCSprite* iconRock = NULL;
        if (times < BRONZE_MEDAL)
        {
            iconRock = CCSprite::create("gameui/btnRock.png");
        }
        else if (times < SILVER_MEDAL)
        {
            iconRock = CCSprite::create("gameui/rock_bronze.png");
        }
        else if (times < GOLD_MEDAL)
        {
            iconRock = CCSprite::create("gameui/rock_silver.png");
        }
        else if (times < PLATINUM_MEDAL)
        {
            iconRock = CCSprite::create("gameui/rock_gold.png");
        }
        else
        {
            iconRock = CCSprite::create("gameui/rock_platinum.png");
        }
        iconRock->setPosition(ccp(5 + iconRock->getContentSize().width / 2, layer->getContentSize().height / 2));
        layer->addChild(iconRock);
    }
    else if (type == eScissors)
    {
        CCSprite* iconScissors = NULL;
        if (times < BRONZE_MEDAL)
        {
            iconScissors = CCSprite::create("gameui/btnScissors.png");
        }
        else if (times < SILVER_MEDAL)
        {
            iconScissors = CCSprite::create("gameui/scissors_bronze.png");
        }
        else if (times < GOLD_MEDAL)
        {
            iconScissors = CCSprite::create("gameui/scissors_silver.png");
        }
        else if (times < PLATINUM_MEDAL)
        {
            iconScissors = CCSprite::create("gameui/scissors_gold.png");
        }
        else
        {
            iconScissors = CCSprite::create("gameui/scissors_platinum.png");
        }
        iconScissors->setPosition(ccp(5 + iconScissors->getContentSize().width / 2, layer->getContentSize().height / 2));
        layer->addChild(iconScissors);
    }
    else if (type == ePaper)
    {
        //添加布图标
        CCSprite* iconPaper = NULL;
        if (times < BRONZE_MEDAL)
        {
            iconPaper = CCSprite::create("gameui/btnPaper.png");
        }
        else if (times < SILVER_MEDAL)
        {
            iconPaper = CCSprite::create("gameui/paper_bronze.png");
        }
        else if (times < GOLD_MEDAL)
        {
            iconPaper = CCSprite::create("gameui/paper_silver.png");
        }
        else if (times < PLATINUM_MEDAL)
        {
            iconPaper = CCSprite::create("gameui/paper_gold.png");
        }
        else
        {
            iconPaper = CCSprite::create("gameui/paper_platinum.png");
        }
        iconPaper->setPosition(ccp(5 + iconPaper->getContentSize().width / 2, layer->getContentSize().height / 2));
        layer->addChild(iconPaper);
    }
    
    
    //添加前3名的步数信息
    for (int i = 1; i <= 3; i++)
    {
        //string key = RankDifficulty::makeKey(m_difficulty, type, i);
        CCLabelTTF* pos = CCLabelTTF::create(getStringFromInt(i).c_str(), "Arial", 40);
        pos->setColor(ccRED);
        pos->setPosition(ccp( 100, layer->getContentSize().height - i * 45));
        layer->addChild(pos);
        
        int stepNum = UserData::sharedUserData()->getStepByKey(m_difficulty, type, i);
        CCLabelTTF* labelStep = CCLabelTTF::create(getStringFromInt(stepNum).c_str(), "Arial", 30);
        labelStep->setPosition(ccp(160, layer->getContentSize().height - i * 45));
        layer->addChild(labelStep);
        
        CCLabelTTF* labelDate = CCLabelTTF::create(UserData::sharedUserData()->getDateByKey(m_difficulty, type, i), "Arial", 30);
        labelDate->setPosition(ccp(340, layer->getContentSize().height - i * 45));
        layer->addChild(labelDate);
    }
    
    return node;
}

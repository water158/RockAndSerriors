//
//  AchievePage.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-7.
//
//

#ifndef __RockPaperScissors__AchievePage__
#define __RockPaperScissors__AchievePage__

#include <iostream>
#include "cocos2d.h"
#include "UserData.h"
#include "AchieveData.h"
#include "Tools.h"
//定义一组宏用来确定成就信息的位置  该组宏用的是相对的间隔 不是真实的位置
#define RANKPOSX 100
#define RANKSTEPX 140
#define RANKDATEX 200
#define RANKFIRSTY -70

USING_NS_CC;

class AchievePage : public CCNode
{
public:
    static AchievePage* createWithDifficulty(int difficulty);
    
    virtual bool init();
    
    bool initWithDiff(int diff);
    
    CCNode* getInfoByType(ItemType type);
    
    int m_difficulty;
};

#endif /* defined(__RockPaperScissors__AchievePage__) */

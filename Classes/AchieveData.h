//
//  AchieveData.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-6.
//
//

#ifndef __RockPaperScissors__AchieveData__
#define __RockPaperScissors__AchieveData__

#include <iostream>
#include "cocos2d.h"
#include "GlobalEnum.h"
#include "tinyxml.h"

using namespace cocos2d;
using namespace std;

/*****************************
 * AchieveRank
 * 排名信息包括步数和日期
 *****************************/
class AchieveRank : public CCObject
{
public:
    AchieveRank();
    ~AchieveRank();
    bool init();
    CREATE_FUNC(AchieveRank);
    
    void loadData(TiXmlElement *elm);
    
private:
    //类型信息
    CC_SYNTHESIZE(string, m_sDate, Date)
    
    //通过次数
    CC_SYNTHESIZE(int, m_nStep, Step)
};

/*****************************
 * AchieveRank
 * 类型
 *****************************/
class AchieveType : public CCObject
{
public:
    AchieveType();
    ~AchieveType();
    bool init();
    CREATE_FUNC(AchieveType);
    
    void loadData(TiXmlElement *elm);
    
    void increaseTimes();
    
    AchieveRank* getAchieveRank(int rank);
    
private:
    
    //类型信息
    CC_SYNTHESIZE(ItemType, m_eType, Type)
    
    //通过次数
    CC_SYNTHESIZE(int, m_nTimes, Times)
    
    //第一名的数据
    CC_SYNTHESIZE(AchieveRank *, m_pRank1, Rank1);
    CC_SYNTHESIZE(AchieveRank *, m_pRank2, Rank2);
    CC_SYNTHESIZE(AchieveRank *, m_pRank3, Rank3);
};

/*****************************
 * AchieveRank
 * 难度
 *****************************/
class AchieveDifficulty : public CCObject
{
public:
    //static string makeKey(int difficulty, ItemType type, int pos);
    AchieveDifficulty();
    ~AchieveDifficulty();
    bool init();
    CREATE_FUNC(AchieveDifficulty);
    
    void loadData(TiXmlElement *elm);
    
    AchieveType *getAchieveType(ItemType type);
    
private:
    //难度数
    CC_SYNTHESIZE(int, m_nDifficulty, Difficulty);
    
    //
    CC_SYNTHESIZE(AchieveType *, m_pAchievePaper, AchievePaper);
    CC_SYNTHESIZE(AchieveType *, m_pAchieveScissors, AchieveScissors);
    CC_SYNTHESIZE(AchieveType *, m_pAchieveRock, AchieveRock);
};

#endif /* defined(__RockPaperScissors__AchieveData__) */

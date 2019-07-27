//
//  AchieveData.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-6.
//
//

#include "AchieveData.h"

/*****************************
 * AchieveRank
 *****************************/
AchieveRank::AchieveRank()
{
    CCLog("AchieveRank Created!");
    m_sDate = "";
    m_nStep = 0;
}

AchieveRank::~AchieveRank()
{
    CCLog("AchieveRank Deleted!");
}

bool AchieveRank::init()
{
    return true;
}

void AchieveRank::loadData(TiXmlElement *elmAchieveRank)
{
    //设置步数
    if (elmAchieveRank->Attribute("step"))
    {
        m_nStep = atoi(elmAchieveRank->Attribute("step"));
    }
    else
    {
        m_nStep = 0;
    }
    
    //设置时间
    if (elmAchieveRank->Attribute("date"))
    {
        m_sDate = elmAchieveRank->Attribute("date");
    }
    else
    {
        m_sDate = "";
    }
}

/*****************************
 * AchieveType
 *****************************/
AchieveType::AchieveType()
{
}

AchieveType::~AchieveType()
{
    CC_SAFE_RELEASE_NULL(m_pRank1);
    CC_SAFE_RELEASE_NULL(m_pRank2);
    CC_SAFE_RELEASE_NULL(m_pRank3);
}

bool AchieveType::init()
{
    m_eType = eEmpty;
    m_nTimes = 0;
    
    m_pRank1 = AchieveRank::create();
    m_pRank1->retain();
    
    m_pRank2 = AchieveRank::create();
    m_pRank2->retain();
    
    m_pRank3 = AchieveRank::create();
    m_pRank3->retain();
    
    return true;
}

void AchieveType::loadData(TiXmlElement *elmAchieveType)
{
    switch (m_eType)
    {
        case ePaper:
            CCLog("AchieveType::loadData m_eType = Paper");
            break;
        case eScissors:
            CCLog("AchieveType::loadData m_eType = Scissors");
            break;
        case eRock:
            CCLog("AchieveType::loadData m_eType = Rock");
            break;
    }
    
    //获得Times
    if (elmAchieveType->Attribute("times"))
    {
        m_nTimes = atoi(elmAchieveType->Attribute("times"));
    }
    
    //获取排名第1的成就信息
    TiXmlElement *elmAchieveRank = elmAchieveType->FirstChildElement();
    if (elmAchieveRank)
    {
        m_pRank1->loadData(elmAchieveRank);
    }
    
    //获取排名第2的成就信息
    elmAchieveRank = elmAchieveRank->NextSiblingElement();
    if (elmAchieveRank)
    {
        m_pRank2->loadData(elmAchieveRank);
    }
    
    //获取排名第3的成就信息
    elmAchieveRank = elmAchieveRank->NextSiblingElement();
    if (elmAchieveRank)
    {
        m_pRank3->loadData(elmAchieveRank);
    }
}

void AchieveType::increaseTimes()
{
    m_nTimes ++;
}

AchieveRank* AchieveType::getAchieveRank(int rank)
{
    if (rank == 1)
    {
        return m_pRank1;
    }
    else if (rank == 2)
    {
        return m_pRank2;
    }
    
    return m_pRank3;
}

/*****************************
 * AchieveDifficulty
 *****************************/
AchieveDifficulty::AchieveDifficulty()
{
}

AchieveDifficulty::~AchieveDifficulty()
{
    CC_SAFE_RELEASE_NULL(m_pAchievePaper);
    CC_SAFE_RELEASE_NULL(m_pAchieveScissors);
    CC_SAFE_RELEASE_NULL(m_pAchieveRock);
}

bool AchieveDifficulty::init()
{
    m_nDifficulty = 0;
    
    m_pAchievePaper = AchieveType::create();
    m_pAchievePaper->retain();
    
    m_pAchieveScissors = AchieveType::create();
    m_pAchieveScissors->retain();
    
    m_pAchieveRock = AchieveType::create();
    m_pAchieveRock->retain();
    
    return true;
}

void AchieveDifficulty::loadData(TiXmlElement *elmDifficulty)
{
    CCLog("AchieveDifficulty::loadData m_nDifficulty = %d", m_nDifficulty);
    
    //获取Paper类型的成就信息
    TiXmlElement *elmAchieveType = elmDifficulty->FirstChildElement();
    if (elmAchieveType)
    {
        m_pAchievePaper->loadData(elmAchieveType);
    }
    
    //获取Scissors类型的成就信息
    elmAchieveType = elmAchieveType->NextSiblingElement();
    if (elmAchieveType)
    {
        m_pAchieveScissors->loadData(elmAchieveType);
    }
    
    //获取Rock类型的成就信息
    elmAchieveType = elmAchieveType->NextSiblingElement();
    if (elmAchieveType)
    {
        m_pAchieveRock->loadData(elmAchieveType);
    }
}

AchieveType* AchieveDifficulty::getAchieveType(ItemType type)
{
    if (type == ePaper)
    {
        return m_pAchievePaper;
    }
    else if (type == eScissors)
    {
        return m_pAchieveScissors;
    }
    
    return m_pAchieveRock;
}

/*
string AchieveDifficulty::getKey()
{
    return AchieveDifficulty::makeKey(m_nDifficulty, m_eType, m_nPos);
}

string AchieveDifficulty::makeKey(int difficulty, ItemType type, int pos)
{
    return CCString::createWithFormat("%d_%d_%d", difficulty, (int)type, pos)->getCString();
}
*/

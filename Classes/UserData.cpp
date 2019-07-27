#include "UserData.h"


UserData::UserData()
{
    ////CCLog("UserData Constructed");
}

UserData::~UserData()
{
    ////CCLog("UserData Destroyed");
}

UserData* UserData::s_sharedUserData = NULL;

UserData* UserData::sharedUserData(void)
{
    if (!s_sharedUserData)
    {
        s_sharedUserData = new UserData();
        s_sharedUserData->init();
    }
    return s_sharedUserData;
}

void UserData::init()
{
    m_pDictDifficulty = CCDictionary::create();
    m_pDictDifficulty->retain();
    
    load();
}

void UserData::check()
{
	m_sUserDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "userdata_out.xml";

	bool exsit = isFileExist(m_sUserDataPath.c_str());
	if (!exsit)
	{
		copyData("userdata/userdata.xml", "userdata_out.xml");
	}
}

void UserData::load()
{
    check();
    
    CCLog("----------------1----------------");
    TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;
    
	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //UserAchieve  成就
    TiXmlElement *elmUserAchieve = elmRoot->FirstChildElement();
    if (!elmUserAchieve)
    {
        CCMessageBox("getUserAchieveData wrong", "error");
    }
    
    CCLog("----------------2----------------");
    
    //获取难度系数成就信息
    TiXmlElement *elmDifficulty = elmUserAchieve->FirstChildElement();
    
    //初始化难度系数
    for (int difficulty = 3; difficulty <= 9; difficulty++)
    {
        AchieveDifficulty* achieveDifficulty = AchieveDifficulty::create();
        
        achieveDifficulty->setDifficulty(difficulty);
        achieveDifficulty->loadData(elmDifficulty);
        
        m_pDictDifficulty->setObject(achieveDifficulty, difficulty);
    }
    
    CCLog("----------------3----------------");
    
    //UserStatus 初始难度和初始选择难度
    TiXmlElement *elmUserStatus = elmUserAchieve->NextSiblingElement();
    
    //UserStatus 当前关卡难度
    TiXmlElement* elmCursor = elmUserStatus->FirstChildElement();
    this->setOriDifficulty(atoi(elmCursor->FirstChild()->Value()));
    
    //UserStatus 选择的难度
    elmCursor = elmCursor->NextSiblingElement();
    this->setSelectDifficulty(atoi(elmCursor->FirstChild()->Value()));
    
    CCLog("----------------4----------------");
    
    //UserSetting
    TiXmlElement *elmUserSetting = elmUserStatus->NextSiblingElement();
    
    //UserSetting 是否允许音效
    elmCursor = elmUserSetting->FirstChildElement();
    this->setIsSoundEnable(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserSetting 是否允许音乐
    elmCursor = elmCursor->NextSiblingElement();
    this->setIsMusicEnable(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    delete xmlDoc;
}

void UserData::save()
{
    check();

    TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;
    
	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //UserAchieve  成就
    TiXmlElement *elmUserAchieve = elmRoot->FirstChildElement();
    if (!elmUserAchieve)
    {
        CCMessageBox("getUserAchieveData wrong", "error");
    }
    
    //获取难度系数成就信息
    TiXmlElement *elmDifficulty = elmUserAchieve->FirstChildElement();
    
    //初始化难度系数
    for (int difficulty = 3; difficulty <= 9; difficulty++)
    {
        AchieveDifficulty* achieveDifficulty = (AchieveDifficulty *)m_pDictDifficulty->objectForKey(difficulty);
        
        //获取Paper类型的成就信息
        TiXmlElement *elmAchieveType = elmDifficulty->FirstChildElement();
        if (elmAchieveType)
        {
            //Times
            elmAchieveType->SetAttribute("times", achieveDifficulty->getAchievePaper()->getTimes());
            
            //Rank1
            TiXmlElement* elmRank = elmAchieveType->FirstChildElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchievePaper()->getRank1()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchievePaper()->getRank1()->getStep());
            
            //CCLog("achieveDifficulty->getAchievePaper()->getRank1()->getDate().c_str() = %s", achieveDifficulty->getAchievePaper()->getRank1()->getDate().c_str());
            
            //Rank2
            elmRank = elmRank->NextSiblingElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchievePaper()->getRank2()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchievePaper()->getRank2()->getStep());
            
            //CCLog("achieveDifficulty->getAchievePaper()->getRank2()->getDate().c_str() = %s", achieveDifficulty->getAchievePaper()->getRank2()->getDate().c_str());
            
            //Rank3
            elmRank = elmRank->NextSiblingElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchievePaper()->getRank3()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchievePaper()->getRank3()->getStep());
            
            //CCLog("achieveDifficulty->getAchievePaper()->getRank3()->getDate().c_str() = %s", achieveDifficulty->getAchievePaper()->getRank3()->getDate().c_str());
        }
        
        //获取Scissors类型的成就信息
        elmAchieveType = elmAchieveType->NextSiblingElement();
        if (elmAchieveType)
        {
            //Times
            elmAchieveType->SetAttribute("times", achieveDifficulty->getAchieveScissors()->getTimes());
            
            //Rank1
            TiXmlElement* elmRank = elmAchieveType->FirstChildElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchieveScissors()->getRank1()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchieveScissors()->getRank1()->getStep());
            
            //Rank2
            elmRank = elmRank->NextSiblingElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchieveScissors()->getRank2()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchieveScissors()->getRank2()->getStep());
            
            //Rank3
            elmRank = elmRank->NextSiblingElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchieveScissors()->getRank3()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchieveScissors()->getRank3()->getStep());
        }
        
        //获取Rock类型的成就信息
        elmAchieveType = elmAchieveType->NextSiblingElement();
        if (elmAchieveType)
        {
            //Times
            elmAchieveType->SetAttribute("times", achieveDifficulty->getAchieveRock()->getTimes());
            
            //Rank1
            TiXmlElement* elmRank = elmAchieveType->FirstChildElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchieveRock()->getRank1()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchieveRock()->getRank1()->getStep());
            
            //Rank2
            elmRank = elmRank->NextSiblingElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchieveRock()->getRank2()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchieveRock()->getRank2()->getStep());
            
            //Rank3
            elmRank = elmRank->NextSiblingElement();
            elmRank->SetAttribute("date", achieveDifficulty->getAchieveRock()->getRank3()->getDate());
            elmRank->SetAttribute("step", achieveDifficulty->getAchieveRock()->getRank3()->getStep());
        }
        
        elmDifficulty = elmDifficulty->NextSiblingElement();
    }

    //UserStatus
    TiXmlElement *elmUserStatus = elmUserAchieve->NextSiblingElement();
    
    //UserStatus 初始化难度
    TiXmlElement *elmCursor = elmUserStatus->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOriDifficulty()).c_str());
    
    //UserStatus 用户选择难度
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSelectDifficulty()).c_str());

    //UserSetting
    TiXmlElement *elmUserSetting = elmUserStatus->NextSiblingElement();
    
    //UserStatus 是否允许音效
    elmCursor = elmUserSetting->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getIsSoundEnable() ? 1 : 0).c_str());
    
    //UserStatus 是否允许音乐
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getIsMusicEnable() ? 1 : 0).c_str());
   
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    //CCLog("strContent=[%s]", strContent.c_str());
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sUserDataPath);

    delete xmlDoc;
}

int UserData::addUserRankInfo(int difficulty, ItemType type, AchieveRank* rank)
{
    AchieveDifficulty *achieveDifficulty = (AchieveDifficulty *)m_pDictDifficulty->objectForKey(difficulty);
    
    if (achieveDifficulty == NULL)
    {
        return -1;
    }
    
    AchieveType *achieveType = achieveDifficulty->getAchieveType(type);
    
    if (achieveType == NULL)
    {
        return -1;
    }
    
    AchieveRank *rank_1 = achieveType->getRank1();
    AchieveRank *rank_2 = achieveType->getRank2();
    AchieveRank *rank_3 = achieveType->getRank3();
    
    /********************
     *排名信息未完整  前3名未占满  有0值存在
     ********************/
    //如果第一名的值为0 说明还未有人排行 直接排第一
    if (rank_1->getStep() == 0)
    {
        rank_1->setDate(rank->getDate());
        rank_1->setStep(rank->getStep());
        
        save();
        return 1;
    }
    //如果第一名 不为0 第二名为0
    else if (rank_2->getStep() == 0)
    {
        //如果步数大于等于当前第一名 排名第二
        if (rank->getStep() >= rank_1->getStep())
        {
            rank_2->setDate(rank->getDate());
            rank_2->setStep(rank->getStep());
            
            save();
            return 2;
        }
        //如果步数小于当前第一名  排名第一 将原来的第一名排第二
        else
        {
            rank_2->setDate(rank_1->getDate());
            rank_2->setStep(rank_1->getStep());
            
            rank_1->setDate(rank->getDate());
            rank_1->setStep(rank->getStep());
            
            save();
            return 1;
        }
    }
    //如果第一名和第二名都不为0 第三名为0
    else if (rank_3->getStep() == 0)
    {
        //如果步数大于等于当前第二名 排第三
        if (rank->getStep() >= rank_2->getStep())
        {
            rank_3->setDate(rank->getDate());
            rank_3->setStep(rank->getStep());
            
            save();
            return 3;
        }
        //如果步数小于当前第二名但大于等于当前第一名 排名第二 原来第二排第三
        else if (rank->getStep() >= rank_1->getStep())
        {
            rank_3->setDate(rank_2->getDate());
            rank_3->setStep(rank_2->getStep());
            
            rank_2->setDate(rank->getDate());
            rank_2->setStep(rank->getStep());
            
            save();
            return 2;
        }
        //如果步数小于当前第一名 排第一 原来第一排第二 第二排第三
        else
        {
            rank_3->setDate(rank_2->getDate());
            rank_3->setStep(rank_2->getStep());
            
            rank_2->setDate(rank_1->getDate());
            rank_2->setStep(rank_1->getStep());
            
            rank_1->setDate(rank->getDate());
            rank_1->setStep(rank->getStep());
            
            save();
            return 1;
        }
    }
    /***********
     *属于前3种排名都有值的情况
     ***********/
    //步数大于第三名
    else if (rank->getStep() >= rank_3->getStep())
    {
        //未能排名
        return 0;
    }
    //步数小于第三名 大于等于第二名 将排名第三的信息替换
    else if (rank->getStep() >= rank_2->getStep())
    {
        rank_3->setDate(rank->getDate());
        rank_3->setStep(rank->getStep());
        
        save();
        return 3;
    }
    //步数小于第二名 大于第一名 排名第二
    //原来排名第二的排名第三
    else if (rank->getStep() >= rank_1->getStep())
    {
        rank_3->setDate(rank_2->getDate());
        rank_3->setStep(rank_2->getStep());
        
        rank_2->setDate(rank->getDate());
        rank_2->setStep(rank->getStep());
        
        save();
        return 2;
    }
    //步数小于第一名 排名第一
    //原来排名第一的排第二  原来排名第二的排第三
    else if (rank->getStep() < rank_1->getStep())
    {
        rank_3->setDate(rank_2->getDate());
        rank_3->setStep(rank_2->getStep());
        
        rank_2->setDate(rank_1->getDate());
        rank_2->setStep(rank_1->getStep());
        
        rank_1->setDate(rank->getDate());
        rank_1->setStep(rank->getStep());
        
        save();
        return 1;
    }
    else
    {
        return -1;
    }
}

const char* UserData::getDateByKey(int difficulty, ItemType type, int rank)
{
    AchieveDifficulty *achieveDifficulty = (AchieveDifficulty *)m_pDictDifficulty->objectForKey(difficulty);
    
    if (achieveDifficulty == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveDifficulty failed by difficulty %d", difficulty)->getCString(), "error");
        return "";
    }
    
    AchieveType *achieveType = achieveDifficulty->getAchieveType(type);
    
    if (achieveType == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveType failed by difficulty %d Type %d", difficulty, type)->getCString(), "error");
        return "";
    }
    
    AchieveRank *achieveRank = achieveType->getAchieveRank(rank);
    
    if (achieveRank == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveRank failed by difficulty %d Type %d Rank %d", difficulty, type, rank)->getCString(), "error");
        return "";
    }
    
    return achieveRank->getDate().c_str();
}

int UserData::getStepByKey(int difficulty, ItemType type, int rank)
{
    AchieveDifficulty *achieveDifficulty = (AchieveDifficulty *)m_pDictDifficulty->objectForKey(difficulty);
    
    if (achieveDifficulty == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveDifficulty failed by difficulty %d", difficulty)->getCString(), "error");
        return -1;
    }
    
    AchieveType *achieveType = achieveDifficulty->getAchieveType(type);
    
    if (achieveType == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveType failed by difficulty %d Type %d", difficulty, type)->getCString(), "error");
        return -1;
    }
    
    AchieveRank *achieveRank = achieveType->getAchieveRank(rank);
    
    if (achieveRank == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveRank failed by difficulty %d Type %d Rank %d", difficulty, type, rank)->getCString(), "error");
        return -1;
    }
    
    return achieveRank->getStep();
}

int UserData::getTimesByKey(int difficulty, ItemType type)
{
    AchieveDifficulty *achieveDifficulty = (AchieveDifficulty *)m_pDictDifficulty->objectForKey(difficulty);
    
    if (achieveDifficulty == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveDifficulty failed by difficulty %d", difficulty)->getCString(), "error");
        return -1;
    }
    
    AchieveType *achieveType = achieveDifficulty->getAchieveType(type);
    
    if (achieveType == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveType failed by difficulty %d Type %d", difficulty, type)->getCString(), "error");
        return -1;
    }
    
    return achieveType->getTimes();
}

void UserData::addTimesByKey(int difficulty, ItemType type)
{
    AchieveDifficulty *achieveDifficulty = (AchieveDifficulty *)m_pDictDifficulty->objectForKey(difficulty);
    
    if (achieveDifficulty == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveDifficulty failed by difficulty %d", difficulty)->getCString(), "error");
        return;
    }
    
    AchieveType *achieveType = achieveDifficulty->getAchieveType(type);
    
    if (achieveType == NULL)
    {
        CCMessageBox(CCString::createWithFormat("get achieveType failed by difficulty %d Type %d", difficulty, type)->getCString(), "error");
        return;
    }
    
    achieveType->increaseTimes();
    
    save();
}

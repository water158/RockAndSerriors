#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "Tools.h"
#include "AchieveData.h"

//铜牌
#define BRONZE_MEDAL 1
//银牌
#define SILVER_MEDAL 10
//金牌
#define GOLD_MEDAL 20
//白金牌
#define PLATINUM_MEDAL 30

USING_NS_CC;
using namespace std;

class UserData
{
public:
    UserData(void);
	virtual ~UserData(void);
    
    static UserData* sharedUserData(void);
    
    //初始化数据
    void init();
    
    void check();
    
    //保存数据
    void save();
    
    //加载
    void load();
    
    //路径
    string m_sUserDataPath;

    CCDictionary* m_pDictDifficulty;
    
    /*****************
     *添加新的数据数据  如果返回值为-1 添加信息失败
     *返回值为0，排名信息未上榜
     *返回值为1，排名第一
     *返回值为2，排名第二
     *返回值为3，排名第三
     ******************/
    int addUserRankInfo(int difficulty, ItemType type, AchieveRank* rank);
    
    //通过key值获取该成就的步数
    int getStepByKey(int difficulty, ItemType type, int rank);
    
    //通过key值获取该成就的日期
    const char* getDateByKey(int difficulty, ItemType type, int rank);
    
    //通过key值获得该成就通过的次数
    int getTimesByKey(int difficulty, ItemType type);
    
    //通过key值增加该成就通过的次数
    void addTimesByKey(int difficulty, ItemType type);
    
private:
	static UserData* s_sharedUserData;
    //===========游戏设定===========
    CC_SYNTHESIZE(bool, is_sound_enable, IsSoundEnable);
    
    CC_SYNTHESIZE(bool, is_music_enable, IsMusicEnable);
    
    //初始游戏难度
    CC_SYNTHESIZE(int, oriDifficulty, OriDifficulty);
    
    //用户选择的游戏难度
    CC_SYNTHESIZE(int, selectDifficulty, SelectDifficulty);
};

#endif
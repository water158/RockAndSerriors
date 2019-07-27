#ifndef __BoardLogic_H__
#define __BoardLogic_H__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "ItemHex.h"

USING_NS_CC;
using namespace std;


class BoardLogic
{
    /*******************************************
     * 创建 构造 单例 初始化
     *******************************************/
public:
    BoardLogic(void);
	virtual ~BoardLogic(void);
    static BoardLogic* sharedBoardLogic(void);
    void init(int difficulty);
    
    //重玩一把
    void clear(int difficulty);
    
    //手动设定六边形3圈的棋盘
    void setBoardItemManually();

private:
    //单例
	static BoardLogic* s_sharedBoardLogic;
    
    //难度
    CC_SYNTHESIZE(int, m_nDifficulty, Difficulty);
    
    //是否增加步数
    CC_SYNTHESIZE(bool, m_bIsAddStep, IsAddStep);
    
    /*******************************************
     * 下标 范围
     *******************************************/
public:
    bool isItemOutOfRange(int x, int y);
    
    bool isItemEmpty(int x, int y);
    
    int getMinXByY(int y);
    
    int getMaxXByY(int y);
    
    int getMinYByX(int x);
    
    int getMaxYByX(int x);
    
    //获得棋盘单元的个数
    int calculateMaxCount();
    
    //获得棋盘最大的下标
    int calculateMaxIndex();
    
    //获得棋盘中间的下标
    int calculateMidIndex();
    
private:
    //中间的下标
    CC_SYNTHESIZE(int, m_nMidIndex, MidIndex);
    
    //最大的下标
    CC_SYNTHESIZE(int, m_nMaxIndex, MaxIndex);
    
    //总共的个数
    CC_SYNTHESIZE(int, m_nMaxCount, MaxCount);
    
    
    /*******************************************
     * 尺寸 位置
     *******************************************/
public:
    //获得棋子单元的缩放比例
    float calculateCellScale();
    
    //获得棋子单元的尺寸
    CCSize calculateCellSize();
    
    //获得棋子单元的位置
    CCPoint getCellPosition(int x, int y);
    
private:
    CC_SYNTHESIZE(float, m_fCellScale, CellScale);
    
    CC_SYNTHESIZE(CCSize, m_oCellSize, CellSize);
    
    CC_SYNTHESIZE(CCDictionary *, m_pDictItemCache, DictItemCache);
    
    /*******************************************
     * 玩的过程中的数据
     *******************************************/
public:
    void readyForNextStep();
    
    void increaseSteps();
    
private:
    //步数
    CC_SYNTHESIZE(int, m_nSteps, Steps);
    
    /*******************************************
     * 魔法
     *******************************************/
public:
    void increaseMagicTimes(int times);
    
    void decreaseMagicTimes();
    
    ItemType useMagic();
    
    static ItemType getReverseItemType(ItemType type);
    
    //步数
    CC_SYNTHESIZE(int, m_nMagicTimes, MagicTimes);
    
    /*******************************************
     * 棋盘
     *******************************************/
public:
    
    /*******************
     * 设定
     *******************/
    
    ItemHex* getItem(int x, int y);
    
    void setItem(ItemHex* item);
    
    /*******************
     * 随机
     *******************/
    
    //获得 随机的 空的 棋盘元素
    ItemHex* getRandomItemHex();
    
    //将某个空的元素 随机的颜色
    ItemHex* setRandomEmptyItemHex();
    
    ItemType getRandomItemType();
    
    CC_SYNTHESIZE(ItemType, m_oNextRandomItemType, NextRandomItemType);
    
    /*******************
     * 判断游戏是否结束
     *******************/
    
    bool isGameOver(ItemType &typeWin);
    
    /*******************
     * Shift Eat
     *
     * Shift 是偏移去空格的操作
     * Eat 是吃掉对方的操作
     *******************/
    
    void shift(SearchDirection dir_search);
    
    void eat(SearchDirection dir_search);
    
    void processItem2EatItem1(SearchDirection dir, int x1, int y1);
    
    /*******************
     * 找到当前层的所有非空元素
     *******************/
    
    CCArray* getAllItemsNotEmpty(SearchDirection dir, int x, int y);
    
    CCArray* getAllItemsNotEmpty(SearchDirection dir, ItemHex* item);
    
    /*******************
     * 获得下一个元素
     *******************/
    
    ItemHex* getNextItem(SearchDirection dir, int x, int y);
    
    ItemHex* getNextItem(SearchDirection dir, ItemHex *item);
    
    ItemHex* getNextNotEmptyItemReclusive(SearchDirection dir, int x, int y);
    
    /*******************
     * 获得顶头的元素
     *******************/
    
    ItemHex* getHeadItem(SearchDirection dir);
    
    ItemHex* getNextHeadItem(SearchDirection dir, ItemHex *item);

    /*******************
     * 获得相反的方向
     *******************/
    
    SearchDirection getReverseDirection(SearchDirection dir_search);
    
    /*******************************************
     * 调优 Benchmark
     *******************************************/
public:
    void refreshComplexity();
    
    void increaseComplexity();
    
    CC_SYNTHESIZE(int, m_Complexity, Complexity);
    
    //时间
    
    void showDeltaTime(string tag);
    
    bool m_bNextDeltaTimeZero;
    
    CC_SYNTHESIZE(float, m_fDeltaTime, DeltaTime);
    
    struct cc_timeval *m_pLastUpdate;
};

#endif // __BoardLogic_SCENE_H__

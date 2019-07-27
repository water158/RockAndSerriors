#ifndef _ItemHex_H_
#define _ItemHex_H_

#include "cocos2d.h"
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class ItemHex : public CCObject
{

public:
    static ItemHex* create(ItemType type, int x, int y);
    ItemHex();
    virtual ~ItemHex();
    
public:
    static string makeKey(int x, int y);
    bool eatItem(ItemHex *target);
    string getKey();
    string getShiftFromItemKey();
    string getEatFromItemKey();
    string toString();
    
    void resetShiftFromAndEatFrom();
    
    bool isMove();
    bool isEat();
    
public:
    //类型
    CC_SYNTHESIZE(ItemType, m_eType, Type);
    
    //X的位置
    CC_SYNTHESIZE(int, m_nX, X);
    
    //Y的位置
    CC_SYNTHESIZE(int, m_nY, Y);
    
    //是否需要动画
    CC_SYNTHESIZE(bool, m_bNeedAction, NeedAction);
    
    //FROM_X的位置
    CC_SYNTHESIZE(int, m_nXShiftFrom, XShiftFrom);
    
    //FROM_Y的位置
    CC_SYNTHESIZE(int, m_nYShiftFrom, YShiftFrom);
    
    //EAT_X的位置
    CC_SYNTHESIZE(int, m_nXEatFrom, XEatFrom);
    
    //EAT_Y的位置
    CC_SYNTHESIZE(int, m_nYEatFrom, YEatFrom);
    
private:
    void init(ItemType type, int x, int y);
};

#endif  // _GUN_DETAIL_H_
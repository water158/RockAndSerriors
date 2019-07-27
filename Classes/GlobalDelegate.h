#ifndef _GLOBAL_DELEGATE_H_
#define _GLOBAL_DELEGATE_H_

#include "cocos2d.h"
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;


class GameUIDelegate
{
public:
    virtual void callbackCheckGameOver() = 0;
    
    virtual void callbackRefreshSteps() = 0;
    
    virtual void callbackChangeNextItem() = 0;
};

#endif
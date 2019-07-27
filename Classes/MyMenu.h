//
//  mMenu.h
//  plane
//
//  Created by mini02 on 14-1-2.
//
//

#ifndef __plane__mMenu__
#define __plane__mMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class MyMenu :public CCMenu
{
public:
    CCMenuItem* itemForTouch(CCTouch *touch);
    static MyMenu* createWithItems(CCMenuItem *firstItem, va_list args);
    static MyMenu* create(float s,CCMenuItem* item, ...);
    static MyMenu* createWithArray(CCArray* pArrayOfItems);
    float scale_;
};
#endif /* defined(__plane__mMenu__) */

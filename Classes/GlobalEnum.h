#ifndef __GlobalEnum_H__
#define __GlobalEnum_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define BOARD_WIDTH_PADDING 20
#define CELL_WIDTH 196
#define CELL_HEIGHT 225
#define BACKGROUND_Y_OFFSET -400

#define ACTION_DELAY 1

#define ZORDER_BOARD_ITEM_LOW 0
#define ZORDER_BOARD_ITEM_HIGH 1

enum Shift
{
    eLeft,
    eRight,
    eUp,
    eDown,
};

typedef enum
{
    ePaper = 0,
    eScissors,
    eRock,
    eEmpty,
} ItemType;

typedef enum
{
    eSearchLeft,
    eSearchRight,
    eSearchUp,
    eSearchDown,
    eSearchLeftUp,
    eSearchRightDown,
} SearchDirection;

typedef enum
{
    eFromLeftToRight,
    eFromRightToLeft,
    eFromTopToBottom,
    eFromBottomToTop,
    eFromLeftTopToRightBottom,
    eFromRightBottomToLeftTop
} FingerDirection;

typedef enum
{
    kAlgorithmCompareNear,
    kAlgorithmCompareFar
} GameAlgorithm;

enum
{
    kGameOrderBackground = 0,
    kGameOrderPlay,
    kGameOrderShield
};

typedef enum
{
    kMapTypeHex,
    kMapTypeRect,
} GameMapType;

#endif // __GlobalEnum_SCENE_H__

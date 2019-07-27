#include "GamePlayLayer.h"

GamePlayLayer::GamePlayLayer()
{
    m_pBatchNodeCells = CCSpriteBatchNode::create("gameui/hex/hex.png", 1000);
	m_pBatchNodeCells->retain();
    
    m_pDictCells = CCDictionary::create();
    m_pDictCells->retain();
}

GamePlayLayer::~GamePlayLayer()
{
    CC_SAFE_RELEASE_NULL(m_pBatchNodeCells);
    CC_SAFE_RELEASE_NULL(m_pDictCells);
}

bool GamePlayLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    addChild(m_pBatchNodeCells);
    
    
    int indexMax = BoardLogic::sharedBoardLogic()->getMaxIndex();
    float scale = BoardLogic::sharedBoardLogic()->getCellScale();
    
    for (int j = 0; j <= indexMax; j++)
    {
        for (int i = 0; i <= indexMax; i++)
        {

            if (BoardLogic::sharedBoardLogic()->isItemOutOfRange(i, j))
            {
                continue;
            }
            
            CCPoint pos = BoardLogic::sharedBoardLogic()->getCellPosition(i, j);
            
            //CCLog("Cell %d, %d pos %f %f", i, j, pos.x, pos.y);
            
            CCSprite *sp = CCSprite::create();
            sp->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_empty.png"));
            sp->setScale(scale);
            sp->setPosition(pos);
            
            m_pBatchNodeCells->addChild(sp);
            
            string key = ItemHex::makeKey(i, j);
            m_pDictCells->setObject(sp, key);
        }
    }
    
    refresh();
    
    m_bFingerActionEnable = true;   //init
    
    return true;
}

GamePlayLayer* GamePlayLayer::create()
{
    GamePlayLayer *pRet = new GamePlayLayer();
    
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void GamePlayLayer::draw()
{
    CCPoint pos = CCPointZero;
    ccDrawColor4B(255, 0, 0, 255);
    ccPointSize(3);
    ccDrawPoints(&pos, 1);
}

void GamePlayLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GamePlayLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

/***************************
 * 核心方法
 ***************************/

//滑动手指之后 去掉空格 开始吃喝 再次去掉空格
void GamePlayLayer::slide(FingerDirection dir)
{
    BoardLogic::sharedBoardLogic()->readyForNextStep();
    
    switch (dir)
    {
        case eFromLeftToRight:
            
            BoardLogic::sharedBoardLogic()->shift(eSearchLeft);
            
            BoardLogic::sharedBoardLogic()->eat(eSearchLeft);
            
            BoardLogic::sharedBoardLogic()->shift(eSearchLeft);
            
            break;
            
        case eFromRightToLeft:
            
            BoardLogic::sharedBoardLogic()->shift(eSearchRight);
            
            BoardLogic::sharedBoardLogic()->eat(eSearchRight);
            
            BoardLogic::sharedBoardLogic()->shift(eSearchRight);
            
            break;
            
        case eFromTopToBottom:
            
            BoardLogic::sharedBoardLogic()->shift(eSearchUp);
            
            BoardLogic::sharedBoardLogic()->eat(eSearchUp);
            
            BoardLogic::sharedBoardLogic()->shift(eSearchUp);
            
            break;
            
        case eFromBottomToTop:
            
            BoardLogic::sharedBoardLogic()->shift(eSearchDown);
            
            BoardLogic::sharedBoardLogic()->eat(eSearchDown);
            
            BoardLogic::sharedBoardLogic()->shift(eSearchDown);
            
            break;
            
        case eFromLeftTopToRightBottom:
            
            BoardLogic::sharedBoardLogic()->shift(eSearchLeftUp);
            
            BoardLogic::sharedBoardLogic()->eat(eSearchLeftUp);
            
            BoardLogic::sharedBoardLogic()->shift(eSearchLeftUp);
            
            break;
            
        case eFromRightBottomToLeftTop:
            
            BoardLogic::sharedBoardLogic()->shift(eSearchRightDown);
            
            BoardLogic::sharedBoardLogic()->eat(eSearchRightDown);
            
            BoardLogic::sharedBoardLogic()->shift(eSearchRightDown);
            
            break;
    }
}

//随机产生一个新的元素 返回值为false则不会产生
bool GamePlayLayer::random()
{
    bool ret = false;
    
    ItemHex *item = BoardLogic::sharedBoardLogic()->setRandomEmptyItemHex();
    
    if (item != NULL)
    {
        CCLog("random begin at (%d, %d)", item->getX(), item->getY());
        
        CCSprite *sp = (CCSprite *)m_pDictCells->objectForKey(item->getKey());
        
        if (sp != NULL)
        {
            runRandomAction(sp, item);
        }
        
        ret = true;
    }
    
    return ret;
}

void GamePlayLayer::action()
{
    //m_pBatchNodeCells->reorderBatch(ZORDER_BOARD_ITEM_LOW);
    refreshZOrder();
    
    CCDictionary *pDict = BoardLogic::sharedBoardLogic()->getDictItemCache();
    
    CCDictElement *pEle = NULL;
    CCDICT_FOREACH(pDict, pEle)
    {
        ItemHex *item = (ItemHex *)pEle->getObject();
        
        if (item != NULL)
        {
            if (item->isMove())
            {
                runShiftAction(item);
            }
            
            if (item->isEat())
            {
                runEatAction(item);
            }
        }
    }
}

//刷新棋盘
void GamePlayLayer::refresh()
{
    CCDictionary *pDict = BoardLogic::sharedBoardLogic()->getDictItemCache();

    CCDictElement *pEle = NULL;
    CCDICT_FOREACH(pDict, pEle)
    {
        ItemHex *item = (ItemHex *)pEle->getObject();
        
        CCSprite *sp = (CCSprite *)m_pDictCells->objectForKey(item->getKey());
        changeItemByItemType(sp, item->getType());
    }
}

void GamePlayLayer::refreshZOrder()
{
    CCDictElement *pEle = NULL;
    CCDICT_FOREACH(m_pDictCells, pEle)
    {
        CCSprite *sp = (CCSprite *)pEle->getObject();
        
        m_pBatchNodeCells->reorderChild(sp, ZORDER_BOARD_ITEM_LOW);
    }
}

/***************************
 * 触摸
 ***************************/

//事件监听回调：触摸开始
bool GamePlayLayer::ccTouchBegan(CCTouch *touch, CCEvent *unused_event)
{
    if (!m_bFingerActionEnable)
    {
        return false;
    }
    
    //获取触摸的X轴和Y轴
    CCPoint touchPoint = touch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
    touch->getLocationInView();
    
    //获取棋盘的最大和最小值
    int mid = BoardLogic::sharedBoardLogic()->getMidIndex();
    int max = BoardLogic::sharedBoardLogic()->getMaxYByX(mid);
    int min = BoardLogic::sharedBoardLogic()->getMinYByX(mid);
    
    //获取棋盘最高点和最低点的位置
    CCPoint upLimit = BoardLogic::sharedBoardLogic()->getCellPosition(min, min);
    CCPoint dnLimit = BoardLogic::sharedBoardLogic()->getCellPosition(max, max);
    
    //获取单元格的大小
    CCSize cellSize = BoardLogic::sharedBoardLogic()->getCellSize();
    
    //如果点击点在棋盘范围内  返回true 否则返回false
    if (touchPoint.y >= dnLimit.y && touchPoint.y <= upLimit.y + cellSize.height)
    {
        pointBegin = touchPoint;
        return true;
    }
    
    return false;
}

//事件监听回调：触摸结束
void GamePlayLayer::ccTouchEnded(CCTouch *touch, CCEvent *unused_event)
{
    if (!m_bFingerActionEnable)
    {
        return;
    }
    
    m_bFingerActionEnable = false;
    
    //获取X轴和Y轴的移动范围
    //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
    CCPoint pointEnd = touch->getLocation();
    
    if (pointBegin.x == pointEnd.x && pointBegin.y == pointEnd.y)
    {
        m_bFingerActionEnable = true;   //point begin = end
        return;
    }
    
    if (calcTwoPointLength(pointBegin, pointEnd) < 20)
    {
        m_bFingerActionEnable = true;   //point begin - end < 20
        return;
    }
    
    float angle = calcTwoPointAngle(pointBegin, pointEnd);
    //CCLog("Toucn Angle %f", angle);
    
    if (angle > 30 && angle <= 90)
    {
        //朝右上
        CCLog("================ From Bottom To Top");
        slide(eFromBottomToTop);
    }
    else if (angle > 90 && angle <= 150)
    {
        //朝左上
        CCLog("================ From Right Bottom To Left Top");
        slide(eFromRightBottomToLeftTop);
    }
    else if (angle > 150 && angle <= 210)
    {
        //朝左
        //shift
        CCLog("================ From Right To Left");
        slide(eFromRightToLeft);
    }
    else if (angle > 210 && angle <= 270)
    {
        //朝左下
        CCLog("================ From Top To Bottom");
        slide(eFromTopToBottom);
    }
    else if (angle > -90 && angle <= -30)
    {
        //朝右下
        CCLog("================ From Left Top To Right Bottom");
        slide(eFromLeftTopToRightBottom);
    }
    else
    {
        CCLog("================ From Left To Right");
        //朝右
        slide(eFromLeftToRight);
    }
    
    BoardLogic::sharedBoardLogic()->increaseSteps();
    
    action();
    
    if (BoardLogic::sharedBoardLogic()->getIsAddStep())
    {
        random();
        
        delegateGameUI->callbackChangeNextItem();
    }
    else
    {
        m_bFingerActionEnable = true;   //no random
    }
    
    delegateGameUI->callbackRefreshSteps();
    
    delegateGameUI->callbackCheckGameOver();
}

/***************************
 * 执行动作
 ***************************/

//放大
void GamePlayLayer::runRandomAction(CCSprite *sp, ItemHex *item)
{
    CCSequence *seq = CCSequence::create(CCDelayTime::create(ACTION_DELAY * 2),
                                         CCCallFuncND::create(this, callfuncND_selector(GamePlayLayer::callbackAfterRandom), (void *)item),
                                         CCScaleTo::create(0.1, BoardLogic::sharedBoardLogic()->getCellScale() * 2),
                                         CCScaleTo::create(0.1, BoardLogic::sharedBoardLogic()->getCellScale()),
                                         NULL);
    sp->runAction(seq);
}

//放大之后
void GamePlayLayer::callbackAfterRandom(CCNode *sp, void *data)
{
    ItemHex *item = (ItemHex *)data;
    changeItemByItemType((CCSprite *)sp, item->getType());
    
    m_bFingerActionEnable = true;   //random completed
    
    CCLog("Random Over");
}


void GamePlayLayer::runShiftAction(ItemHex *item_to)
{
    CCSprite *sp_shift_from = (CCSprite *)m_pDictCells->objectForKey(item_to->getShiftFromItemKey());
    
    CCPoint pos_to = BoardLogic::sharedBoardLogic()->getCellPosition(item_to->getX(), item_to->getY());
    
    CCSequence *seq = CCSequence::create(CCMoveTo::create(ACTION_DELAY, pos_to),
                                         CCCallFuncND::create(this, callfuncND_selector(GamePlayLayer::callbackAfterShift), (void *)item_to),
                                         NULL);
    
    sp_shift_from->runAction(seq);
    
    m_pBatchNodeCells->reorderChild(sp_shift_from, ZORDER_BOARD_ITEM_HIGH);
}

void GamePlayLayer::callbackAfterShift(CCNode *sp_from, void *data)
{
    ItemHex *item_to = (ItemHex *)data;
    ItemHex *item_from = BoardLogic::sharedBoardLogic()->getItem(item_to->getXShiftFrom(), item_to->getYShiftFrom());
    
    changeItemByItemType((CCSprite *)sp_from, item_from->getType());
    
    CCPoint pos_from = BoardLogic::sharedBoardLogic()->getCellPosition(item_to->getXShiftFrom(), item_to->getYShiftFrom());
    sp_from->setPosition(pos_from);
    
    CCSprite *sp_to = (CCSprite *)m_pDictCells->objectForKey(item_to->getKey());
    changeItemByItemType(sp_to, item_to->getType());
}

void GamePlayLayer::runEatAction(ItemHex *item_to)
{
    CCSprite *sp_eat_from = (CCSprite *)m_pDictCells->objectForKey(item_to->getEatFromItemKey());
    
    CCPoint pos_to = BoardLogic::sharedBoardLogic()->getCellPosition(item_to->getX(), item_to->getY());
    
    CCSequence *seq = CCSequence::create(CCMoveTo::create(ACTION_DELAY, pos_to),
                                         CCCallFuncND::create(this, callfuncND_selector(GamePlayLayer::callbackAfterEat), (void *)item_to),
                                         NULL);
    
    sp_eat_from->runAction(seq);
    
    m_pBatchNodeCells->reorderChild(sp_eat_from, ZORDER_BOARD_ITEM_HIGH);
}

void GamePlayLayer::callbackAfterEat(CCNode *sp_from, void *data)
{
    ItemHex *item_to = (ItemHex *)data;
    ItemHex *item_from = BoardLogic::sharedBoardLogic()->getItem(item_to->getXEatFrom(), item_to->getYEatFrom());
    
    CCLog("sp_from (%d, %d) location (%.2f, %.2f)", item_to->getXEatFrom(), item_to->getYEatFrom(), sp_from->getPositionX(), sp_from->getPositionY());
    
    changeItemByItemType((CCSprite *)sp_from, item_from->getType());
    
    CCPoint pos_from = BoardLogic::sharedBoardLogic()->getCellPosition(item_to->getXEatFrom(), item_to->getYEatFrom());
    sp_from->setPosition(pos_from);
    
    CCSprite *sp_to = (CCSprite *)m_pDictCells->objectForKey(item_to->getKey());
    changeItemByItemType(sp_to, item_to->getType());
}

void GamePlayLayer::changeItemByItemType(CCSprite *sp, ItemType type)
{
    switch (type)
    {
        case ePaper:
            sp->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_paper.png"));
            
            break;
        case eRock:
            sp->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_rock.png"));
            
            break;
        case eScissors:
            sp->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_scissors.png"));
            
            break;
        case eEmpty:
            sp->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_empty.png"));
            break;
    }
}
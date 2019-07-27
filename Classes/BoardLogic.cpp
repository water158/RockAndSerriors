#include "BoardLogic.h"

/*******************************************
 * 创建 构造 单例 初始化
 *******************************************/

BoardLogic* BoardLogic::s_sharedBoardLogic = NULL;

BoardLogic* BoardLogic::sharedBoardLogic(void)
{
    if (!s_sharedBoardLogic)
    {
        s_sharedBoardLogic = new BoardLogic();
    }
    
    
    return s_sharedBoardLogic;
}

BoardLogic::BoardLogic()
{
    m_pDictItemCache = CCDictionary::create();
    m_pDictItemCache->retain();
}

BoardLogic::~BoardLogic()
{
    CC_SAFE_RELEASE_NULL(m_pDictItemCache);
    CC_SAFE_DELETE(m_pLastUpdate);
}

void BoardLogic::init(int difficulty)
{
    m_pLastUpdate = new struct cc_timeval();
    
    m_bNextDeltaTimeZero = true;
    
    clear(difficulty);
}

void BoardLogic::clear(int difficulty)
{
    this->setSteps(0);
    this->setMagicTimes(3);
    this->setDifficulty(difficulty);
    
    this->setMaxIndex(calculateMaxIndex());
    this->setMidIndex(calculateMidIndex());
    this->setMaxCount(calculateMaxCount());
    
    this->setCellScale(calculateCellScale());
    this->setCellSize(calculateCellSize());
    
    CCLog("BoardLogic::init(%d) Scale[%f] Size[w,h]=[%f, %f]", difficulty,m_fCellScale, m_oCellSize.width, m_oCellSize.height);
    
    m_pDictItemCache->removeAllObjects();
    
    for (int j = 0; j <= m_nMaxIndex; j++)
    {
        for (int i = 0; i <= m_nMaxIndex; i++)
        {
            if (isItemOutOfRange(i, j))
            {
                //CCLog("BoardLogic::init Key = [%s] Is OUT OF RANGE", ItemHex::makeKey(i, j).c_str());
                continue;
            }
            
            ItemHex *item = ItemHex::create(eEmpty, i, j);
            string key = item->getKey();
            
            //CCLog("BoardLogic::init Key = [%s]", key.c_str());
            
            m_pDictItemCache->setObject(item, key);
        }
    }
    
    m_oNextRandomItemType = getRandomItemType();
    
    setBoardItemManually();
}

void BoardLogic::setBoardItemManually()
{
    ItemHex *item = NULL;
    string key = "";
    
    //===================
    key = "0_0";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "1_0";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "2_0";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eScissors);
    m_pDictItemCache->setObject(item, key);
    
    //===================
    key = "0_1";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "1_1";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "2_1";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(ePaper);
    m_pDictItemCache->setObject(item, key);
    
    key = "3_1";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    //===================
    key = "0_2";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "1_2";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "2_2";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(ePaper);
    m_pDictItemCache->setObject(item, key);
    
    key = "3_2";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "4_2";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    //===================
    key = "1_3";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "2_3";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(ePaper);
    m_pDictItemCache->setObject(item, key);
    
    key = "3_3";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "4_3";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    //===================
    key = "2_4";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eRock);
    m_pDictItemCache->setObject(item, key);
    
    key = "3_4";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
    
    key = "4_4";
    item = (ItemHex *)m_pDictItemCache->objectForKey(key);
    item->setType(eEmpty);
    m_pDictItemCache->setObject(item, key);
}

/*******************************************
 * 下标 范围
 *******************************************/

bool BoardLogic::isItemOutOfRange(int x, int y)
{
    if (y < getMinYByX(x) || y > getMaxYByX(x))
    {
        return true;
    }
    
    return false;
}

bool BoardLogic::isItemEmpty(int x, int y)
{
    if (isItemOutOfRange(x, y))
    {
        return false;
    }
    else
    {
        ItemHex *item = getItem(x, y);
        
        if (item->getType() == eEmpty)
        {
            return true;
        }
    }
    
    return false;
}

//根据输入的X获得最小的Y值 这个值和BoardSize相关
int BoardLogic::getMinYByX(int x)
{
    if (x <= m_nMidIndex)
    {
        return 0;
    }
    else
    {
        return x - m_nMidIndex;
    }
}

//根据输入的X获得最大的Y值 这个值和BoardSize相关
int BoardLogic::getMaxYByX(int x)
{
    if (x <= m_nMidIndex)
    {
        return m_nMidIndex + x;
    }
    else
    {
        return m_nMaxIndex;
    }
}

int BoardLogic::getMinXByY(int y)
{
    if (y <= m_nMidIndex)
    {
        return 0;
    }
    else
    {
        return y - m_nMidIndex;
    }
}

int BoardLogic::getMaxXByY(int y)
{
    if (y <= m_nMidIndex)
    {
        return m_nMidIndex + y;
    }
    else
    {
        return m_nMaxIndex;
    }
}

//获得棋盘单元的个数
//当difficulty=1 棋盘是1*1
//当difficulty=2 棋盘是3*3
//当difficulty=3 棋盘是5*5
//当difficulty=4 棋盘是7*7
//当difficulty=5 棋盘是9*9
int BoardLogic::calculateMaxCount()
{
    return m_nDifficulty * 2 - 1;
}

//获得棋盘最大的下标
//当difficulty=1 棋盘是1*1 返回0
//当difficulty=2 棋盘是3*3 返回2
//当difficulty=3 棋盘是5*5 返回4
//当difficulty=4 棋盘是7*7 返回6
//当difficulty=5 棋盘是9*9 返回8
int BoardLogic::calculateMaxIndex()
{
    return (m_nDifficulty - 1) * 2;
}

//获得棋盘中间的下标
//当difficulty=1 棋盘是1*1 返回0
//当difficulty=2 棋盘是3*3 返回1
//当difficulty=3 棋盘是5*5 返回2
//当difficulty=4 棋盘是7*7 返回3
//当difficulty=5 棋盘是9*9 返回4
int BoardLogic::calculateMidIndex()
{
    return (m_nDifficulty - 1);
}

/*******************************************
 * 尺寸 位置
 *******************************************/

float BoardLogic::calculateCellScale()
{
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    float width_board = size.width - BOARD_WIDTH_PADDING * 2.0f;
    
    float width_cell = width_board / m_nMaxCount;
    
    CCLog("width_board: %f", width_board);
    CCLog("width_cell : %f", width_cell);
    CCLog("scale      : %f", width_cell / CELL_WIDTH);
    
    return width_cell / CELL_WIDTH;
}

CCSize BoardLogic::calculateCellSize()
{
    CCLog("cell width : %f", CELL_WIDTH * m_fCellScale);
    CCLog("cell height: %f", CELL_HEIGHT * m_fCellScale);
    
    return CCSize(CELL_WIDTH * m_fCellScale, CELL_HEIGHT * m_fCellScale);
}

CCPoint BoardLogic::getCellPosition(int x, int y)
{
    CCPoint p = CCPointZero;
    float left_offset = BOARD_WIDTH_PADDING + (m_oCellSize.width / 2);
    
    if (y <= m_nMidIndex)
    {
        p.x = left_offset + fabs(m_nMidIndex - y) * (m_oCellSize.width / 2) + x * m_oCellSize.width;
    }
    else
    {
        p.x = left_offset + fabs(m_nMidIndex - y) * (m_oCellSize.width / 2) + (x - fabs(m_nMidIndex - y)) * m_oCellSize.width;
    }
    
    p.y = (m_nMaxIndex - y + 1) * (m_oCellSize.height * 3 / 4);
    
    return p;
}

/*******************************************
 * 玩的过程中的数据
 *******************************************/

void BoardLogic::readyForNextStep()
{
    //设定是否要增加Step
    setIsAddStep(false);
    
    //在做Shift和Eat操作的时候 先要清理所有的动画FLAG
    CCDictElement *ele = NULL;
    CCDICT_FOREACH(m_pDictItemCache, ele)
    {
        ItemHex *item = (ItemHex *)ele->getObject();
        
        item->setNeedAction(false);
        
        item->resetShiftFromAndEatFrom();
    }
}

void BoardLogic::increaseSteps()
{
    if (this->getIsAddStep())
    {
        m_nSteps ++;
    }
}

/*******************************************
 * 魔法
 *******************************************/

void BoardLogic::increaseMagicTimes(int times)
{
    m_nMagicTimes += times;
}

void BoardLogic::decreaseMagicTimes()
{
    if (m_nMagicTimes > 0)
    {
        m_nMagicTimes --;
    }
}

ItemType BoardLogic::getReverseItemType(ItemType type)
{
    ItemType ret;
    
    switch (type)
    {
        case ePaper:
            ret = eRock;
            break;
        case eRock:
            ret = eScissors;
            break;
        case eScissors:
            ret = ePaper;
            break;
        case eEmpty:
            break;
    }
    
    return ret;
}

ItemType BoardLogic::useMagic()
{
    ItemType type = getRandomItemType();
    ItemType type_reverse = getReverseItemType(type);
    
    CCDictElement *ele = NULL;
    CCDICT_FOREACH(m_pDictItemCache, ele)
    {
        ItemHex *item = (ItemHex *)ele->getObject();
        
        if (item->getType() == type)
        {
            item->setType(type_reverse);
        }
        else if (item->getType() == type_reverse)
        {
            item->setType(type);
        }
    }
    
    decreaseMagicTimes();
    
    return type;
}

/*******************************************
 * 棋盘
 *******************************************/

/*******************
 * 设定
 *******************/

void BoardLogic::setItem(ItemHex *item)
{
    string key = ItemHex::makeKey(item->getX(), item->getY());
    
    m_pDictItemCache->setObject(item, key);
}

ItemHex* BoardLogic::getItem(int x, int y)
{
    ItemHex *item = (ItemHex *)m_pDictItemCache->objectForKey(ItemHex::makeKey(x, y));
    
    return item;
}

/*******************
 * 随机
 *******************/

ItemHex* BoardLogic::getRandomItemHex()
{
    CCArray *arrayEmpty = CCArray::create();
    
    CCDictElement *ele = NULL;
    CCDICT_FOREACH(m_pDictItemCache, ele)
    {
        ItemHex *item = (ItemHex *)ele->getObject();
        
        if (item->getType() == eEmpty)
        {
            arrayEmpty->addObject(item);
        }
    }
    
    if (arrayEmpty->count() == 0)
    {
        return NULL;
    }
    
    
    int i = arc4random() % arrayEmpty->count();
    ItemHex *item = (ItemHex *)arrayEmpty->objectAtIndex(i);
    
    CC_SAFE_RELEASE_NULL(arrayEmpty);
    
    return item;
}

ItemHex* BoardLogic::setRandomEmptyItemHex()
{
    ItemHex *item = getRandomItemHex();
    
    if (item != NULL)
    {
        item->setType(m_oNextRandomItemType);
        
        m_oNextRandomItemType = getRandomItemType();
    }
    
    return item;
}

ItemType BoardLogic::getRandomItemType()
{
    return (ItemType)(arc4random() % 3);
}

/*******************
 * 判断游戏是否结束
 *******************/

bool BoardLogic::isGameOver(ItemType &typeWin)
{
    ItemType typeFirst = eEmpty;
    bool bFirst = true;
    
    CCDictElement *ele = NULL;
    CCDICT_FOREACH(m_pDictItemCache, ele)
    {
        ItemHex *item = (ItemHex *)ele->getObject();
        
        CCLog("isGameOver %d, %d = %s", item->getX(), item->getY(), item->toString().c_str());
        
        if (item->getType() == eEmpty)
        {
            CCLog("NOT GAME OVER 1");
            return false;
        }
        
        if (bFirst)
        {
            typeFirst = item->getType();
            bFirst = false;
        }
        else
        {
            if (item->getType() != typeFirst)
            {
                CCLog("NOT GAME OVER 2");
                return false;
            }
        }
        
        CCLog("isGameOver typefirst = %d", (int)typeFirst);
    }
    
    if (bFirst)
    {
        CCLog("NOT GAME OVER 3");
        return false;
    }
    
    CCLog("GAME OVER");
    typeWin = typeFirst;
    return true;
}

/*******************
 * Shift Eat
 *
 * Shift 是偏移去空格的操作
 * Eat 是吃掉对方的操作
 *******************/

void BoardLogic::shift(SearchDirection dir_search)
{
    CCLog("##### shift");
    
    ItemHex *item = getHeadItem(dir_search);
    
    while (item)
    {
        //当前层所有的非空元素 都准备 存到这个数组中
        CCArray *arrayItems = getAllItemsNotEmpty(dir_search, item);
        
        //将所有非空的元素重新填入到m_pDictItemCache中
        int indexOfItems = 0;
        if (arrayItems->count() != 0)
        {
            ItemHex *itemShiftTo = item;
            while (itemShiftTo)
            {
                if (indexOfItems >= arrayItems->count())
                {
                    //因为，非空的元素已经占领了前面的所有位置
                    //所以，把后面的所有元素都置为空
                    if (itemShiftTo->getType() != eEmpty)
                    {
                        itemShiftTo->setType(eEmpty);
                        itemShiftTo->resetShiftFromAndEatFrom();
                        
                        setItem(itemShiftTo);
                        
                        //如果有这个事情发生 就会增加新的一步
                        setIsAddStep(true);
                    }
                }
                else
                {
                    //非空的元素占领前面的所有位置
                    ItemHex *itemNotEmpty = (ItemHex *)arrayItems->objectAtIndex(indexOfItems);
                    
                    itemShiftTo->setType(itemNotEmpty->getType());
                    
                    itemShiftTo->setXShiftFrom(itemNotEmpty->getXShiftFrom());
                    itemShiftTo->setYShiftFrom(itemNotEmpty->getYShiftFrom());
                    
                    if (itemNotEmpty->isEat())
                    {
                        itemShiftTo->setXEatFrom(itemNotEmpty->getXEatFrom());
                        itemShiftTo->setYEatFrom(itemNotEmpty->getYEatFrom());
                    }
                    
                    
                    setItem(itemShiftTo);
                    
                    CCLog("(%d, %d) = %s --[SHIFT]--> (%d, %d) = %s", itemNotEmpty->getX(), itemNotEmpty->getY(), itemNotEmpty->toString().c_str(), itemShiftTo->getX(), itemShiftTo->getY(), itemShiftTo->toString().c_str());
                }
                
                indexOfItems++;
                
                itemShiftTo = getNextItem(dir_search, itemShiftTo);
            }
        }
        
        CC_SAFE_RELEASE_NULL(arrayItems);
        
        //Get Next Head Item
        item = getNextHeadItem(dir_search, item);
    }
}

void BoardLogic::eat(SearchDirection dir_search)
{
    CCLog("##### eat");
    
    //获得首个头部元素
    ItemHex *item = getHeadItem(dir_search);
    
    while (item)
    {
        ItemHex *itemEat = item;
        
        while (itemEat)
        {
            processItem2EatItem1(dir_search, itemEat->getX(), itemEat->getY());
            
            itemEat = getNextItem(dir_search, itemEat);
        }
        
        //下一个头部元素
        item = getNextHeadItem(dir_search, item);
    }
}

void BoardLogic::processItem2EatItem1(SearchDirection dir, int x1, int y1)
{
    ItemHex *itemMeet = getItem(x1, y1);
    
    if (itemMeet != NULL)
    {
        if (itemMeet->getType() != eEmpty)
        {
            //找到了第一个元素 然后找第二个元素
            
            //CCLog("Item1 (%d, %d) = %s", item1->getX(), item1->getY(), item1->toString().c_str());
            
            ItemHex *itemEatter = getNextNotEmptyItemReclusive(dir, itemMeet->getX(), itemMeet->getY());
            
            if (itemEatter != NULL)
            {
                //CCLog("Item2 (%d, %d) = %s", item2->getX(), item2->getY(), item2->toString().c_str());
                
                if (itemEatter->eatItem(itemMeet))
                {
                    CCLog("(%d, %d) = %s --[ EAT ]--> (%d, %d) = %s", itemEatter->getX(), itemEatter->getY(), itemEatter->toString().c_str(), itemMeet->getX(), itemMeet->getY(), itemMeet->toString().c_str());
                    
                    itemMeet->setType(itemEatter->getType());
                    itemEatter->setType(eEmpty);
                    
                    
                    
                    itemMeet->setNeedAction(true);
                    itemEatter->setNeedAction(false);
                    
                    setItem(itemMeet);
                    setItem(itemEatter);
                    
                    itemMeet->setXEatFrom(itemEatter->getXShiftFrom());
                    itemMeet->setYEatFrom(itemEatter->getYShiftFrom());
                    
                    itemEatter->resetShiftFromAndEatFrom();
                    
                    //如果有这个事情发生 就会增加新的一步
                    setIsAddStep(true);
                }
            }
        }
    }
}

/*******************
 * 找到当前层的所有非空元素
 *******************/

CCArray* BoardLogic::getAllItemsNotEmpty(SearchDirection dir_search, int x, int y)
{
    CCArray *arrayItems = CCArray::create();
    ItemHex *item = NULL;
    
    switch (dir_search)
    {
        case eSearchLeft:
            
            item = getItem(getMaxXByY(y), y);
            
            break;
            
        case eSearchRight:
            
            item = getItem(getMinXByY(y), y);
            
            break;
            
        case eSearchUp:
            
            item = getItem(x, getMaxYByX(x));
            
            break;
            
        case eSearchDown:
            
            item = getItem(x, getMinYByX(x));
            
            break;
            
        case eSearchLeftUp:
            
            item = getItem(x, y);
            
            break;
            
        case eSearchRightDown:
            
            item = getItem(x, y);
            
            break;
    }
    
    while (item != NULL)
    {
        if (item->getType() != eEmpty)
        {
            //找到所有的非空的元素
            arrayItems->addObject(item);
        }
        
        item = getNextItem(dir_search, item->getX(), item->getY());
    }
    
    
    return arrayItems;
}

CCArray* BoardLogic::getAllItemsNotEmpty(SearchDirection dir, ItemHex *item)
{
    return getAllItemsNotEmpty(dir, item->getX(), item->getY());
}

/*******************
 * 获得下一个元素
 *******************/

ItemHex* BoardLogic::getNextItem(SearchDirection dir, int x, int y)
{
    int offset_x = 0;
    int offset_y = 0;
    
    switch (dir)
    {
        case eSearchLeft:
            offset_x = -1;
            offset_y = 0;
            break;
        case eSearchRight:
            offset_x = 1;
            offset_y = 0;
            break;
        case eSearchUp:
            offset_x = 0;
            offset_y = -1;
            break;
        case eSearchDown:
            offset_x = 0;
            offset_y = 1;
            break;
        case eSearchLeftUp:
            offset_x = -1;
            offset_y = -1;
            break;
        case eSearchRightDown:
            offset_x = 1;
            offset_y = 1;
            break;
    }
    
    //CCLog("getNextItem (%d, %d) -> (%d, %d)", x, y, x + offset_x, y + offset_y);
    
    return getItem(x + offset_x, y + offset_y);
}

ItemHex* BoardLogic::getNextItem(SearchDirection dir, ItemHex *item)
{
    return getNextItem(dir, item->getX(), item->getY());
}

ItemHex* BoardLogic::getNextNotEmptyItemReclusive(SearchDirection dir, int x, int y)
{
    ItemHex *item = getNextItem(dir, x, y);
    
    increaseComplexity();
    
    //CCLog("Reclusive %d, %d | Complexity %d", x, y, getComplexity());
    
    if (item == NULL)
    {
        //CCLog("Reclusive (%d, %d) NULL", x, y);
        return NULL;
    }
    else if (item->getType() == eEmpty)
    {
        //CCLog("Reclusive (%d, %d) -> (%d, %d)", x, y, item->getX(), item->getY());
        return getNextNotEmptyItemReclusive(dir, item->getX(), item->getY());
    }
    
    //CCLog("Reclusive FOUND %d, %d", item->getX(), item->getY());
    return item;
}

/*******************
 * 获得顶头的元素
 *******************/

ItemHex* BoardLogic::getHeadItem(SearchDirection dir)
{
    ItemHex *itemRet = NULL;
    
    switch (dir)
    {
        case eSearchLeft:
            itemRet = getItem(getMaxXByY(0), 0);
            break;
        case eSearchRight:
            itemRet = getItem(getMinXByY(0), 0);
            break;
        case eSearchUp:
            itemRet = getItem(0, getMaxYByX(0));
            break;
        case eSearchDown:
            itemRet = getItem(0, getMinYByX(0));
            break;
        case eSearchLeftUp:
            itemRet = getItem(m_nMaxIndex, getMinYByX(m_nMaxIndex));
            break;
        case eSearchRightDown:
            itemRet = getItem(getMaxXByY(0), 0);
            break;
    }
    
    //CCLog("getHeadItem (%d, %d)", itemRet->getX(), itemRet->getY());
    
    return itemRet;
}

ItemHex* BoardLogic::getNextHeadItem(SearchDirection dir, ItemHex *item)
{
    ItemHex *itemRet = NULL;
    int x = 0, y = 0;
    
    switch (dir)
    {
        case eSearchLeft:
            y = item->getY() + 1;
            itemRet = getItem(getMaxXByY(y), y);
            break;
        case eSearchRight:
            y = item->getY() + 1;
            itemRet = getItem(getMinXByY(y), y);
            break;
        case eSearchUp:
            x = item->getX() + 1;
            itemRet = getItem(x, getMaxYByX(x));
            break;
        case eSearchDown:
            x = item->getX() + 1;
            itemRet = getItem(x, getMinYByX(x));
            break;
        case eSearchLeftUp:
            if (item->getY() < m_nMaxIndex)
            {
                //竖向的 第一部分
                itemRet = getItem(m_nMaxIndex, item->getY() + 1);
            }
            else
            {
                //横向的 第二部分
                itemRet = getItem(item->getX() - 1, m_nMaxIndex);
            }
            break;
        case eSearchRightDown:
            if (item->getX() > 0)
            {
                //横向的 第一部分
                itemRet = getItem(item->getX() - 1, 0);
            }
            else
            {
                //竖向的 第二部分
                itemRet = getItem(0, item->getY() + 1);
            }
            break;
    }
    
    if (itemRet)
    {
        //CCLog("getNextHeadItem (%d, %d)", itemRet->getX(), itemRet->getY());
    }
    
    return itemRet;
}

/*******************
 * 获得相反的方向
 *******************/

SearchDirection BoardLogic::getReverseDirection(SearchDirection dir_search)
{
    SearchDirection ret;
    
    switch (dir_search)
    {
        case eSearchLeft:
            ret = eSearchRight;
            break;
        case eSearchRight:
            ret = eSearchLeft;
            break;
        case eSearchUp:
            ret = eSearchDown;
            break;
        case eSearchDown:
            ret = eSearchUp;
            break;
        case eSearchLeftUp:
            ret = eSearchRightDown;
            break;
        case eSearchRightDown:
            ret = eSearchLeftUp;
            break;
    }
    
    return ret;
}

/*******************************************
 * 调优 Benchmark
 *******************************************/

void BoardLogic::refreshComplexity()
{
    m_Complexity = 0;
}

void BoardLogic::increaseComplexity()
{
    m_Complexity ++;
}

void BoardLogic::showDeltaTime(string tag)
{
    struct cc_timeval now;
    
    if (CCTime::gettimeofdayCocos2d(&now, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        m_fDeltaTime = 0;
        return;
    }
    
    // new delta time. Re-fixed issue #1277
    if (m_bNextDeltaTimeZero)
    {
        m_fDeltaTime = 0;
        m_bNextDeltaTimeZero = false;
    }
    else
    {
        m_fDeltaTime = (now.tv_sec - m_pLastUpdate->tv_sec) + (now.tv_usec - m_pLastUpdate->tv_usec) / 1000000.0f;
        m_fDeltaTime = MAX(0, m_fDeltaTime);
    }
    
    CCLog("[%s] Show Delta Time %f", tag.c_str(), m_fDeltaTime);
    
    *m_pLastUpdate = now;
}
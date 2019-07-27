#include "ItemHex.h"

ItemHex* ItemHex::create(ItemType type, int x, int y)
{
    ItemHex *pRet = new ItemHex();
    
    if (pRet)
    {
        pRet->init(type, x, y);
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

string ItemHex::makeKey(int x, int y)
{
    return CCString::createWithFormat("%d_%d", x, y)->getCString();
}

ItemHex::ItemHex()
{
    CCLog("ItemHex Created.");
}

ItemHex::~ItemHex()
{
    CCLog("ItemHex Deleted.");
}

string ItemHex::getKey()
{
    return ItemHex::makeKey(m_nX, m_nY);
}

string ItemHex::getShiftFromItemKey()
{
    return ItemHex::makeKey(m_nXShiftFrom, m_nYShiftFrom);
}

string ItemHex::getEatFromItemKey()
{
    return ItemHex::makeKey(m_nXEatFrom, m_nYEatFrom);
}

bool ItemHex::eatItem(ItemHex* other)
{
    if (m_eType == eRock && other->m_eType == eScissors)
    {
        return true;
    }
    
    if (m_eType == eScissors && other->m_eType == ePaper)
    {
        return true;
    }
    
    if (m_eType == ePaper && other->m_eType == eRock)
    {
        return true;
    }
    
    return false;
}

void ItemHex::init(ItemType type, int x, int y)
{
    m_eType = type;
    m_nX = x;
    m_nY = y;
    m_bNeedAction = false;
    
    resetShiftFromAndEatFrom();
}

void ItemHex::resetShiftFromAndEatFrom()
{
    m_nXShiftFrom = m_nX;
    m_nYShiftFrom = m_nY;
    m_nXEatFrom = m_nX;
    m_nYEatFrom = m_nY;
}

bool ItemHex::isMove()
{
    if (m_nXShiftFrom != m_nX || m_nYShiftFrom != m_nY)
    {
        CCLog("MOVE (%d, %d) -> (%d, %d)", m_nXShiftFrom, m_nYShiftFrom, m_nX, m_nY);
        
        return true;
    }
    
    return false;
}

bool ItemHex::isEat()
{
    if (m_nXEatFrom != m_nX || m_nYEatFrom != m_nY)
    {
        CCLog("EAT (%d, %d) -> (%d, %d)", m_nXEatFrom, m_nYEatFrom, m_nX, m_nY);
        
        return true;
    }
    
    return false;
}

string ItemHex::toString()
{
    switch (m_eType)
    {
        case ePaper:
            return "Paper";
            break;
        case eRock:
            return "Rock";
            break;
        case eScissors:
            return "Scissors";
            break;
        case eEmpty:
            return "Empty";
            break;
    }
    
    return "NONE";
}